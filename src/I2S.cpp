#include "I2S.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"

extern "C"
{
#include "soc/syscon_reg.h"
#include "soc/syscon_struct.h"
}

#define ADC_PATT_LEN_MAX (16)
#define ADC_CHECK_UNIT(unit) RTC_MODULE_CHECK(adc_unit < ADC_UNIT_2, "ADC unit error, only support ADC1 for now", ESP_ERR_INVALID_ARG)
#define RTC_MODULE_CHECK(a, str, ret_val)                                             \
  if (!(a))                                                                           \
  {                                                                                   \
    ESP_LOGE(RTC_MODULE_TAG, "%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
    return (ret_val);                                                                 \
  }

i2s_event_t i2s_evt;
static QueueHandle_t i2s_event_queue;

static esp_err_t adc_set_i2s_data_len(adc_unit_t adc_unit, int patt_len)
{
  ADC_CHECK_UNIT(adc_unit);
  RTC_MODULE_CHECK((patt_len < ADC_PATT_LEN_MAX) && (patt_len > 0), "ADC pattern length error", ESP_ERR_INVALID_ARG);
  // portENTER_CRITICAL(&rtc_spinlock);
  if (adc_unit & ADC_UNIT_1)
  {
    SYSCON.saradc_ctrl.sar1_patt_len = patt_len - 1;
  }
  if (adc_unit & ADC_UNIT_2)
  {
    SYSCON.saradc_ctrl.sar2_patt_len = patt_len - 1;
  }
  // portEXIT_CRITICAL(&rtc_spinlock);
  return ESP_OK;
}

static esp_err_t adc_set_i2s_data_pattern(adc_unit_t adc_unit, int seq_num, adc_channel_t channel, adc_bits_width_t bits, adc_atten_t atten)
{
  ADC_CHECK_UNIT(adc_unit);
  if (adc_unit & ADC_UNIT_1)
  {
    RTC_MODULE_CHECK((adc1_channel_t)channel < ADC1_CHANNEL_MAX, "ADC1 channel error", ESP_ERR_INVALID_ARG);
  }
  RTC_MODULE_CHECK(bits < ADC_WIDTH_MAX, "ADC bit width error", ESP_ERR_INVALID_ARG);
  RTC_MODULE_CHECK(atten < ADC_ATTEN_MAX, "ADC Atten Err", ESP_ERR_INVALID_ARG);

  // portENTER_CRITICAL(&rtc_spinlock);
  // Configure pattern table, each 8 bit defines one channel
  //[7:4]-channel [3:2]-bit width [1:0]- attenuation
  // BIT WIDTH: 3: 12BIT  2: 11BIT  1: 10BIT  0: 9BIT
  // ATTEN: 3: ATTEN = 11dB 2: 6dB 1: 2.5dB 0: 0dB
  uint8_t val = (channel << 4) | (bits << 2) | (atten << 0);
  if (adc_unit & ADC_UNIT_1)
  {
    SYSCON.saradc_sar1_patt_tab[seq_num / 4] &= (~(0xff << ((3 - (seq_num % 4)) * 8)));
    SYSCON.saradc_sar1_patt_tab[seq_num / 4] |= (val << ((3 - (seq_num % 4)) * 8));
  }
  if (adc_unit & ADC_UNIT_2)
  {
    SYSCON.saradc_sar2_patt_tab[seq_num / 4] &= (~(0xff << ((3 - (seq_num % 4)) * 8)));
    SYSCON.saradc_sar2_patt_tab[seq_num / 4] |= (val << ((3 - (seq_num % 4)) * 8));
  }
  // portEXIT_CRITICAL(&rtc_spinlock);
  return ESP_OK;
}

void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS)
{
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
	  .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S_MSB,
	  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL3, // lowest interrupt priority
      .dma_buf_count = 5,
      .dma_buf_len = 640,
	  .use_apll =0
    };

  if (MODE == I2S_MODE_RX || MODE == I2S_MODE_TX) {
    Serial.println("using I2S_MODE");
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = PIN_I2S_BCLK;
    pin_config.ws_io_num = PIN_I2S_LRC;
    if (MODE == I2S_MODE_RX) {
      pin_config.data_out_num = I2S_PIN_NO_CHANGE;
      pin_config.data_in_num = PIN_I2S_DIN;
    } 
    else if (MODE == I2S_MODE_TX) {
      pin_config.data_out_num = PIN_I2S_DOUT;
      pin_config.data_in_num = I2S_PIN_NO_CHANGE;
    }

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_MONO);
  } 
  else if (MODE == I2S_MODE_DAC_BUILT_IN || MODE == I2S_MODE_ADC_BUILT_IN) {
    Serial.println("using ADC_builtin");
	if (i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK) {
		Serial.println("ERROR: Unable to install I2S drives");
	}
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
    // i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_MONO);
    i2s_adc_enable(I2S_NUM_0);
    delay(500); // required for stability of ADC

    // ***IMPORTANT*** enable continuous adc sampling
    SYSCON.saradc_ctrl2.meas_num_limit = 0;
    // sample time SAR setting
    SYSCON.saradc_ctrl.sar_clk_div = 2;
    SYSCON.saradc_fsm.sample_cycle = 1;
    adc_set_i2s_data_pattern(ADC_UNIT_1, 0, ADC_CHANNEL_0, ADC_WIDTH_BIT_12, ADC_ATTEN_DB_2_5); // Input Vref 1.36V=4095,Offset 0.6324V=1744
    adc_set_i2s_data_len(ADC_UNIT_1, 1);

    i2s_set_pin(I2S_NUM_0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN); // IO26
    i2s_zero_dma_buffer(I2S_NUM_0);
    // i2s_start(I2S_NUM_0);
    //  dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    dac_i2s_enable();
  }
}

int I2S_Read(char *data, int numData)
{
  return i2s_read_bytes(I2S_NUM_0, (char *)data, numData, portMAX_DELAY);
}

void I2S_Write(char *data, int numData)
{
  i2s_write_bytes(I2S_NUM_0, (const char *)data, numData, portMAX_DELAY);
}

void MakeSampleStereo16(int16_t sample[2],char channels,char bps) {
	// Mono to "stereo" conversion
	if (channels == 1)
		sample[RIGHTCHANNEL] = sample[LEFTCHANNEL];
	if (bps == 8) {
		// Upsample from unsigned 8 bits to signed 16 bits
		sample[LEFTCHANNEL] = (((int16_t)(sample[LEFTCHANNEL] & 0xff)) - 128) << 8;
		sample[RIGHTCHANNEL] = (((int16_t)(sample[RIGHTCHANNEL] & 0xff)) - 128) << 8;
	}
};

int16_t Amplify(int16_t s,float gainF2P6) {
	int32_t v = (int32_t)(s * gainF2P6) >> 6;
	if (v < -32767) return -32767;
	else if (v > 32767) return 32767;
	else return (int16_t)(v & 0xffff);
}

bool ConsumeSample(int16_t sample[2],bool mono)
{

	int16_t ms[2];

	ms[0] = sample[0];
	ms[1] = sample[1];
	//MakeSampleStereo16(ms,0,16);

	if (mono) {
		// Average the two samples and overwrite
		int32_t ttl = ms[LEFTCHANNEL] + ms[RIGHTCHANNEL];
		ms[LEFTCHANNEL] = ms[RIGHTCHANNEL] = (ttl >> 1) & 0xffff;
	}

	uint32_t s32;
	//if (output_mode == INTERNAL_DAC)
	//{
		int16_t l = Amplify(ms[LEFTCHANNEL],1) + 0x8000;
		int16_t r = Amplify(ms[RIGHTCHANNEL],1) + 0x8000;
		s32 = (r << 16) | (l & 0xffff);
	//}
	//else
	//{
	//	s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
	//}
	//"i2s_write_bytes" has been removed in the ESP32 Arduino 2.0.0,  use "i2s_write" instead.
	//    return i2s_write_bytes((i2s_port_t)portNo, (const char *)&s32, sizeof(uint32_t), 0);

	size_t i2s_bytes_written;
	i2s_write(I2S_NUM_0, (const char*)&s32, sizeof(uint32_t), &i2s_bytes_written, 0);
	return i2s_bytes_written;
}



