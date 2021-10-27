/*
 Name:		M17 Analog Hotspot Gateway
 Created:	1-Nov-2021 14:27:23
 Author:	HS5TQA/Atten
 Reflector: https://m17.dprns.com
*/

#include <Arduino.h>
#include <driver/dac.h>
#include <SPI.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

#include <WiFiClientSecure.h>
#include <ESP32Ping.h>
#include <Preferences.h>
#include <ctype.h> // for isNumber check
#include <ButterworthFilter.h>	//In the codec2 folder in the library folder
//#include <FastAudioFIFO.h>		//In the codec2 folder in the library folder
#include <PubSubClient.h>

#include <time.h>
#include <TimeLib.h>

#include "main.h"
#include "webservice.h"
#include "m17.h"
#include "spiffs.h"


#define EEPROM_SIZE 512

#ifdef SDCARD
#define SDCARD_CS 13
#define SDCARD_CLK 14
#define SDCARD_MOSI 15
#define SDCARD_MISO 2
#endif

#define SPEAKER_PIN 26
#define MIC_PIN 36
#define PTT_PIN 32
#define RSSI_PIN 33

#define AGC_FRAME_BYTES     320
#define ADC_BUFFER_SIZE 320 //40ms of voice in 8KHz sampling frequency

M17Flags connect_status;
extern uint16_t txstreamid ;
extern uint16_t tx_cnt;
extern unsigned short streamid;
extern unsigned short frameid;
extern unsigned long RxTimeout;
extern unsigned long m17ConectTimeout;

int16_t adc_buffer[ADC_BUFFER_SIZE];

//FastAudioFIFO audio_fifo;

byte daysavetime = 1;

const int natural = 1;

//กรองความถี่สูงผ่าน >600Hz  HPF Butterworth Filter. 0-600Hz ช่วงความถี่ต่ำใช้กับโทน CTCSS/DCS ในวิทยุสื่อสารจะถูกรองทิ้ง
ButterworthFilter hp_filter(600, 8000, ButterworthFilter::ButterworthFilter::Highpass, 2);
//กรองความถี่ต่ำผ่าน <3KHz  LPF Butterworth Filter. ความถี่เสียงที่มากกว่า 3KHz ไม่ใช่ความถี่เสียงคนพูดจะถูกกรองทิ้ง
ButterworthFilter lp_filter(3000, 8000, ButterworthFilter::ButterworthFilter::Lowpass, 2);

CODEC2* codec2_3200;
CODEC2* codec2_1600;

//กำหนดค่าเริ่มต้นใช้โหมดของ Codec2
int mode = CODEC2_MODE_3200;

//Queue<char> audioq(300);
cppQueue	audioq(sizeof(uint8_t), 800, IMPLEMENTATION);	// Instantiate queue
cppQueue	pcmq(sizeof(int16_t), 8000, IMPLEMENTATION);	// Instantiate queue
cppQueue	adcq(sizeof(int16_t), 8000, IMPLEMENTATION);	// Instantiate queue

int nstart_bit, nend_bit, bit_rate;
short* buf;
unsigned char* bits;
float* softdec_bits;

unsigned char snd[20000];
int snd_count = 0;
int snd_rd, snd_wr, snd_idx;
bool snd_out = false;

unsigned char c2[1000];
int c2_offset = 0;
int nsam, nbit, nbyte, i, frames, bits_proc, bit_errors, error_mode;

String srcCall,mycall, urCall, rptr1;

SPIClass spiSD(HSPI);

// Set your Static IP address for wifi AP
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 254);
IPAddress subnet(255, 255, 255, 0);

uint8_t sndW;
int16_t adcR;
int ppm_Level = 0;
int ppm_temp = 0;


int vox_gain=25;
bool vox=false;
bool rxRef = false;
bool tx = false;
bool firstTX = false;
bool firstRX = false;

Configuration config;
unsigned long NTP_Timeout;
unsigned long pingTimeout;

//TaskHandle_t taskSensorHandle;
TaskHandle_t taskNetworkHandle;
TaskHandle_t taskDSPHandle;
TaskHandle_t taskUIHandle;


time_t systemUptime = 0;
time_t wifiUptime = 0;

uint8_t checkSum(uint8_t* ptr, uint16_t count)
{
	uint8_t lrc, tmp;
	uint16_t i;
	lrc = 0;
	for (i = 0; i < count; i++) {
		tmp = ptr[i];
		lrc = lrc ^ tmp;
	}
	return lrc;
}

void saveEEPROM()
{
	uint8_t chkSum = 0;
	byte* ptr;
	ptr = (byte*)&config;
	EEPROM.writeBytes(1, ptr, sizeof(Configuration));
	chkSum = checkSum(ptr, sizeof(Configuration));
	EEPROM.write(0, chkSum);
	EEPROM.commit();
}

//กำหนดค่าคอนฟิกซ์เริ่มต้น
void defaultConfig() {
	Serial.println(F("Default configure mode!"));
	sprintf(config.aprs_mycall, "APRSTH");
	config.aprs_ssid = 5;
	sprintf(config.aprs_host, "aprs.dprns.com");
	config.aprs_port = 14580;
	config.aprs_passcode[0]=0;
	sprintf(config.aprs_filter, "b/HS*/E2*");
	sprintf(config.id, "M5DV");
	sprintf(config.wifi_ssid, "APRSTH");
	sprintf(config.wifi_pass, "aprsthnetwork");
	sprintf(config.wifi_ap_ssid, "M17Hotspot");
	config.wifi_ap_pass[0] = 0;
	//	sprintf(config.wifi_ap_pass, "");
	sprintf(config.reflector_host, "203.150.19.24");
	sprintf(config.reflector_name, "M17-THA");
	config.reflector_port = 17000;
	config.reflector_module = 'C';
	sprintf(config.authUser, "admin");
	sprintf(config.authPass, "admin");
	sprintf(config.mycall, "mycall");
	config.mymodule = 'M';
	config.aprs = false;
	config.wifi = true;
	config.wifi_mode = WIFI_AP_STA;
	config.wifi_ch = 1;
	config.gps_lat = 13.78310;
	config.gps_lon = 100.40920;
	config.gps_alt = 5;
	config.volume = 10;
	config.mic = 10;
  config.vox_delay=10;
  config.vox_level=10;
  config.codec2_mode=CODEC2_MODE_3200;
  saveEEPROM();
}

pkgListType pkgList[PKGLISTSIZE];
unsigned char pkgList_index = 0;

void sort(pkgListType a[], int size);
void sortPkgDesc(pkgListType a[], int size);

void taskDSP(void* pvParameters);
void taskNetwork(void* pvParameters);
void taskUI(void* pvParameters);


//Bandpass Filter
void bp_filter(float *h,int n){
  int i=0;
  for(i=0;i<n;i++){
    h[i]=lp_filter.Update(h[i]);
  }
  for(i=0;i<n;i++){
    h[i]=hp_filter.Update(h[i]);
  }
}

//เข้ารหัสและถอดรหัสเสียง Codec2
void process_audio()
{
	uint8_t c2Buf[8];
	short audioBuf[320];
  float audiof[320];
	
	int16_t adc;

	int pcmWidth = 160;

	if (mode == CODEC2_MODE_1600)
		pcmWidth = 320;
	else
		pcmWidth = 160;

	if (tx) {
			if (adcq.getCount() >= pcmWidth) {
				//Serial.print("PCM: " );
				for (int x = 0; x < pcmWidth; x++) {
					if (!adcq.pop(&adc)) break;
          audiof[x]=(float)adc;

					ppm_temp = (int)abs((int16_t)audiof[x]);
					if (ppm_temp > ppm_Level)
						ppm_Level = ((ppm_Level * 255) + ppm_temp) >> 8;
					else
						ppm_Level = (ppm_Level * 16383) >> 14;
					//Serial.printf("%02x ", (unsigned char)lsb);
				}
        bp_filter(audiof,pcmWidth);
        for(int i=0;i<pcmWidth;i++) audioBuf[i]=(int16_t)(audiof[i]*((float)config.mic/10));
				//Serial.println();
				if(mode == CODEC2_MODE_1600)
					codec2_encode(codec2_1600, c2Buf, audioBuf);
				else
					codec2_encode(codec2_3200, c2Buf, audioBuf);

				//Serial.print("Encode: ");
				for (int x = 0; x < 8; x++)
        {
					audioq.push(&c2Buf[x]);
					//Serial.printf("%02x ", (unsigned char)c2Buf[x]);
				}
				//timeUse = millis() - msTimer;
				//Serial.println();
				//Serial.printf("Encode Time: %d ms.\n", (int)timeUse);
			}
	}
	else {
		int packetSize = audioq.getCount();
		if (packetSize >= 8) {
			//Serial.printf("nbyte= %d\n", nbyte);
			//Serial.printf("nsam= %d\n", nsam);
			snd_count = 0;

			//Serial.print("CODEC2: " + frame);
			for (int x = 0; x < 8; x++) {
				if (!audioq.pop(&c2Buf[x])) break;
				//Serial.printf("%02x ", (unsigned char)c2Buf[x]);
			}
			//Serial.println();

			if (mode == CODEC2_MODE_1600)
				codec2_decode(codec2_1600, audioBuf, c2Buf);
			else
				codec2_decode(codec2_3200, audioBuf, c2Buf);

			for (int x = 0; x < pcmWidth; x++)
			{
				//float auf = (float)audioBuf[x]*((float)config.volume/10);				
				//audioBuf[x] = (int16_t)hp_filter.Update(auf);      
				pcmq.push(&audioBuf[x]);

				// ppm_temp = (int)abs(audioBuf[x]);
				// if (ppm_temp > ppm_Level)
				// 	ppm_Level = ((ppm_Level * 255) + ppm_temp) >> 8;
				// else
				// 	ppm_Level = (ppm_Level * 16383) >> 14;
			}
			//Serial.println("Recitation complete.");
			//Serial.printf("Decode Time: %d ms.\n", (int)timeUse);
		}
	}
}

hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Code with critica section
int offset=29760,offset_new=0,adc_count=0;

//แซมปลิ้งเสียง 8,000 ครั้งต่อวินาที ทั้งเข้า ADC และออก DAC
void IRAM_ATTR onTime() {
	portENTER_CRITICAL_ISR(&timerMux);
	//len = pcmq.getCount();
  int sample;
	if (tx) {
    // Detect ADC overflow
    sample=(int)analogRead(MIC_PIN);
    if(sample>4095) sample=4095;
    adcR = (int16_t)((sample<<4)-offset); //12bit -> 16bit,convert to sign,29760
		adcq.push(&adcR);
	}else {
    //รับเสียงจากเซิร์ฟเวอร์ M17 ออกลำโพง
		if (pcmq.getCount() > 0) {
			pcmq.pop(&adcR);
			sndW = (uint8_t)((((int)adcR + 32768) >> 8) & 0xFF);
			dacWrite(SPEAKER_PIN, sndW);
		}else{
      //โหมด stanby แต่ไม่มีข้อมูลเสียง
      sample=(int)analogRead(MIC_PIN);
      adcR = (int16_t)((sample<<4)-offset); //12bit -> 16bit,convert to sign
      ppm_temp = (int)abs(adcR);
      ppm_Level = ((ppm_Level * 255) + ppm_temp) >> 8;
    }
	}
	portEXIT_CRITICAL_ISR(&timerMux);
}


const char* lastTitle = "LAST HERT";

char pkgList_Find(char* call) {
	char i;
	for (i = 0; i < PKGLISTSIZE; i++) {
		if (strstr(pkgList[(int)i].calsign, call) != NULL) return i;
	}
	return -1;
}

char pkgListOld() {
	char i, ret = 0;
	time_t minimum = pkgList[0].time;
	for (i = 1; i < PKGLISTSIZE; i++) {
		if (pkgList[(int)i].time < minimum) {
			minimum = pkgList[(int)i].time;
			ret = i;
		}
	}
	return ret;
}

void sort(pkgListType a[], int size) {
	pkgListType t;
	char* ptr1;
	char* ptr2;
	char* ptr3;
	ptr1 = (char*)& t;
	for (int i = 0; i < (size - 1); i++) {
		for (int o = 0; o < (size - (i + 1)); o++) {
			if (a[o].time < a[o + 1].time) {
				ptr2 = (char*)& a[o];
				ptr3 = (char*)& a[o + 1];
				memcpy(ptr1, ptr2, sizeof(pkgListType));
				memcpy(ptr2, ptr3, sizeof(pkgListType));
				memcpy(ptr3, ptr1, sizeof(pkgListType));
			}
		}
	}
}

void sortPkgDesc(pkgListType a[], int size) {
	pkgListType t;
	char* ptr1;
	char* ptr2;
	char* ptr3;
	ptr1 = (char*)& t;
	for (int i = 0; i < (size - 1); i++) {
		for (int o = 0; o < (size - (i + 1)); o++) {
			if (a[o].pkg < a[o + 1].pkg) {
				ptr2 = (char*)& a[o];
				ptr3 = (char*)& a[o + 1];
				memcpy(ptr1, ptr2, sizeof(pkgListType));
				memcpy(ptr2, ptr3, sizeof(pkgListType));
				memcpy(ptr3, ptr1, sizeof(pkgListType));
			}
		}
	}
}

void pkgListUpdate(char* call, bool type) 
{
	char i = pkgList_Find(call);
	if (i != 255) { //Found call in old pkg
		pkgList[(uint)i].time = now();
		pkgList[(uint)i].pkg++;
		pkgList[(uint)i].type = type;
		//Serial.print("Update: ");
	}
	else {
		i = pkgListOld();
		pkgList[(uint)i].time = now();
		pkgList[(uint)i].pkg = 1;
		pkgList[(uint)i].type = type;
		strcpy(pkgList[(uint)i].calsign, call);
		//strcpy(pkgList[(uint)i].ssid, &ssid[0]);
		pkgList[(uint)i].calsign[10] = 0;
		//Serial.print("NEW: ");
	}
}

// Define meter size
#define M_SIZE 0.5

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = M_SIZE * 120, osy = M_SIZE * 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update
int old_analog = -999; // Value last displayed
int old_digital = -999; // Value last displayed
uint16_t vuOffsetX = 2;
uint16_t vuOffsetY = 23;


String getValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length();

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}  // END

boolean isValidNumber(String str) 
{
	for (byte i = 0; i < str.length(); i++)
	{
		if (isDigit(str.charAt(i))) return true;
	}
	return false;
}

void setup() {
  int mode;
	byte* ptr;
	pinMode(32, INPUT_PULLUP);
	pinMode(RSSI_PIN, INPUT_PULLUP);
	pinMode(PTT_PIN, OUTPUT);

  digitalWrite(PTT_PIN,LOW);

	analogReadResolution(12);             // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
	analogSetWidth(12);                   // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
	analogSetPinAttenuation(MIC_PIN, ADC_11db); //Pressure 0-3.3V
  
  enableLoopWDT();
  enableCore0WDT();
  enableCore1WDT();
  //esp_task_wdt_init(TWDT_TIMEOUT_S, false);

	Serial.begin(115200);

	if (!EEPROM.begin(EEPROM_SIZE))
	{
		Serial.println(F("failed to initialise EEPROM")); //delay(100000);
	}

	delay(100);

  //ตรวจสอบคอนฟิกซ์ผิดพลาด
  ptr = (byte*)&config;
	EEPROM.readBytes(1, ptr, sizeof(Configuration));
	uint8_t chkSum = checkSum(ptr, sizeof(Configuration));
	if (EEPROM.read(0) != chkSum) {
    Serial.println("Config EEPROM Error!");
		defaultConfig();    
	}

	// if (digitalRead(32) == LOW) {
	// 	defaultConfig();
  //   Serial.println("Default configure!");
	// 	delay(3000);
	// }
	// else {
		ptr = (byte*)&config;
		Serial.println("Load configure!");
		EEPROM.readBytes(1, ptr, sizeof(Configuration));
	//}

	connect_status = DISCONNECTED;

	
#ifdef SDCARD
	spiSD.begin(SDCARD_CLK, SDCARD_MISO, SDCARD_MOSI, -1);//SCK,MISO,MOSI,ss 

	if (!SD.begin(SDCARD_CS, spiSD, 4000000U)) {
		Serial.println(F("SD CARD Initialization failed!"));
	}
	uint8_t cardType = SD.cardType();

	if (cardType == CARD_NONE) {
		Serial.println(F("No SD card attached"));
	}

	Serial.print(F("SD Card Type: "));
	if (cardType == CARD_MMC) {
		Serial.println(F("MMC"));

	}
	else if (cardType == CARD_SD) {
		Serial.println(F("SDSC"));
	}
	else if (cardType == CARD_SDHC) {
		Serial.println(F("SDHC"));
	}
	else {
		Serial.println(F("UNKNOWN"));
	}

	Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
	Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

#endif

	//Start a timer at 8kHz to sample the ADC and play the audio on the DAC.
	timer = timerBegin(3, 500, true); // 80 MHz / 500 = 160KHz MHz hardware clock
	timerAttachInterrupt(timer, &onTime, true); // Attaches the handler function to the timer 
	timerAlarmWrite(timer, 20, true); // Interrupts when counter == 20, 8.000 times a second
	//timerAlarmEnable(adcTimer); //Activate it
		
	//Init codec2
	mode = CODEC2_MODE_1600;
	codec2_1600 = codec2_create(mode);
	nsam = codec2_samples_per_frame(codec2_1600);
	nbit = codec2_bits_per_frame(codec2_1600);
	nbyte = (nbit + 7) / 8;
	//frames = bit_errors = bits_proc = 0;
	//nstart_bit = 0;
	//nend_bit = nbit - 1;

	codec2_set_natural_or_gray(codec2_1600, !natural);
	codec2_set_lpc_post_filter(codec2_1600, 1, 0, 0.8, 0.2);
	Serial.printf("Create CODEC2_1600 : nsam=%d ,nbit=%d ,nbyte=%d\n", nsam, nbit, nbyte);

	mode = CODEC2_MODE_3200;
	codec2_3200 = codec2_create(mode);
	nsam = codec2_samples_per_frame(codec2_3200);
	nbit = codec2_bits_per_frame(codec2_3200);
	nbyte = (nbit + 7) / 8;

	codec2_set_natural_or_gray(codec2_3200, !natural);
	codec2_set_lpc_post_filter(codec2_3200, 1, 0, 0.9, 0.3);
	Serial.printf("Create CODEC2_3200 : nsam=%d ,nbit=%d ,nbyte=%d\n", nsam, nbit, nbyte);
  
	xTaskCreatePinnedToCore(
		taskNetwork,     /* Function to implement the task */
		"taskNetwork",   /* Name of the task */
		8192,      /* Stack size in words */
		NULL,      /* Task input parameter */
		2,         /* Priority of the task */
		&taskNetworkHandle,      /* Task handle. */
		1);        /* Core where the task should run */

	xTaskCreatePinnedToCore(
		taskDSP,     /* Function to implement the task */
		"taskDSP",   /* Name of the task */
		32768,      /* Stack size in words */
		NULL,      /* Task input parameter */
		1,         /* Priority of the task */
		&taskDSPHandle,      /* Task handle. */
		0);        /* Core where the task should run */
	xTaskCreatePinnedToCore(
		taskUI,     /* Function to implement the task */
		"taskUI",   /* Name of the task */
		4096,      /* Stack size in words */
		NULL,      /* Task input parameter */
		1,         /* Priority of the task */
		&taskUIHandle,      /* Task handle. */
		1);        /* Core where the task should run */
}

bool pressed(const int pin)
{
	if (digitalRead(pin) == LOW)
	{
		delay(500);
		return true;
	}
	return false;
}

bool firstRxDisp = false;

void loop() {
	vTaskDelay(1 / portTICK_PERIOD_MS);
	serviceHandle();
}

int timeHalfSec = 0;
long idleTimeout = 0;
bool firstIdle = true;
// uint8_t micCur = 0;
// uint8_t volCur = 0;
void taskUI(void* pvParameters) {
  int voxCount=0;
	String info = "";

	idleTimeout = 0;
	// micCur = config.mic;
	// volCur = config.volume;

  vox=false;
  tx=false;
  //timerAlarmEnable(timer);
  int mic_level;
  long voxtime=millis()+10;

	for (;;) {
		//long now = millis();
		//wdtDisplayTimer = now;
		//Serial.print("task1 Uptime (ms): ");
		//Serial.println(millis());
		vTaskDelay(10 / portTICK_PERIOD_MS);
  
    if(voxtime<millis()){
      voxtime=millis()+10;
      mic_level=ppm_Level>>6;          
      if(mic_level>config.vox_level){        
        if(voxCount<config.vox_delay) {
          voxCount++;
        }else{
          //Serial.printf("Vox Active ppm=%d\n",mic_level);
          vox=true;
        }
      }else{
        if(voxCount>0){
          voxCount--;
        }else{
          vox=false;
          //Serial.printf("Vox- ppm=%d\n",ppm_Level);
        }
      }
    }

			//PTT KEY
			//if ((digitalRead(37) == LOW) && (!rxRef)) {
			if (vox) {
				//Start Transmit
				if (tx == false) {
					tx = true;
					firstTX = true;
					firstIdle = true;
					tx_cnt = 0;
					if (config.codec2_mode == CODEC2_MODE_3200)
						mode = CODEC2_MODE_3200;
					else
						mode = CODEC2_MODE_1600;
					audioq.clean();
					pcmq.clean();
					adcq.clean();
					txstreamid++;

					dac_output_disable(DAC_CHANNEL_1);
					dac_output_disable(DAC_CHANNEL_2);
					pinMode(SPEAKER_PIN, OUTPUT);
					digitalWrite(SPEAKER_PIN, 0);
					timerAlarmEnable(timer);
					Serial.println("<Start TX to Ref.>");	
          digitalWrite(PTT_PIN,LOW);	
				}
				tx = true;
				//dac_output_disable(DAC_CHANNEL_1);
				idleTimeout = millis();
			}
			else {
				//End Transmit
				if (tx) {
					//timerAlarmDisable(timer);
					Serial.println("<END TX>");
				}
				tx = false;
				tx_cnt = 0x8000;
			}

			//End RX
			if (millis() > (RxTimeout + 800)) {
				if (rxRef) {
					rxRef = false;
					firstRX = false;
					firstIdle = true;
					Serial.println("<END RX>");
          digitalWrite(PTT_PIN,LOW); //PTT Key end to radio
					dac_output_disable(DAC_CHANNEL_1);
					dac_output_disable(DAC_CHANNEL_2);
				}
			}

			//Start RX
			if (firstRX) {
				firstRX = false;
				firstRxDisp = true;
				timerAlarmEnable(timer);
				if(SPEAKER_PIN==25)
					dac_output_enable(DAC_CHANNEL_1);
				else
					dac_output_enable(DAC_CHANNEL_2);
				Serial.println("<Start RX From Ref.>");
				pkgListUpdate((char*)srcCall.c_str(), 0);
        digitalWrite(PTT_PIN,HIGH); //PTT Key talk to radio
			}


		if (rxRef) {
			idleTimeout = millis();
      //Stop Vox in RX Mode.
      vox=false;
      voxtime=millis()+500;
			if (firstRxDisp) {
				firstRxDisp = false;				
        info = "DstID: " + urCall;
        info+="\nType: "+ rptr1;
        info += "\nFrameID: " + String(frameid);
        info += "\nStreamID: " + String(streamid);
        Serial.println(info);        
      }
		}
	}
}

void taskDSP(void* pvParameters) {
	Serial.println("Task DSP has been start");
	for (;;) {
		vTaskDelay(2 / portTICK_PERIOD_MS);
		process_audio();
	}
}

unsigned long int wifiTTL = 0;
void taskNetwork(void* pvParameters) {
	int c = 0;
	//chipid = ESP.getEfuseMac();
	Serial.println("Task Network has been start");
  if (config.wifi_mode == WIFI_AP_STA_FIX || config.wifi_mode == WIFI_AP_FIX) { //AP=false
		//WiFi.mode(config.wifi_mode);
		if (config.wifi_mode == WIFI_AP_STA_FIX) {
			WiFi.mode(WIFI_AP_STA);
		}
		else if (config.wifi_mode == WIFI_AP_FIX) {
			WiFi.mode(WIFI_AP);
		}
    //กำหนดค่าการทำงานไวไฟเป็นแอสเซสพ้อย
		WiFi.softAP(config.wifi_ap_ssid, config.wifi_ap_pass);  //Start HOTspot removing password will disable security
		WiFi.softAPConfig(local_IP, gateway, subnet);
		Serial.print("Access point running. IP address: ");
		Serial.print(WiFi.softAPIP());
		Serial.println("");
	}
	else if (config.wifi_mode == WIFI_STA_FIX) {
		WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println(F("WiFi Station Only mode."));
	}
	else {
		WiFi.mode(WIFI_OFF);
    WiFi.disconnect(true);
    delay(100);
    Serial.println(F("WiFi OFF All mode."));
	}

	webService();
 
	for (;;) {
		vTaskDelay(10 / portTICK_PERIOD_MS);
		//long now = millis();
		//wdtNetworkTimer = now;

		if ((config.wifi) && (config.wifi_mode == WIFI_AP_STA_FIX || config.wifi_mode == WIFI_STA_FIX)) {
			if (WiFi.status() != WL_CONNECTED) {
				unsigned long int tw = millis();
				if (tw > wifiTTL) {
					wifiTTL = tw + 60000;
          Serial.println(F("WiFi Connecting.."));
					WiFi.disconnect();
					WiFi.setHostname("M17Hotspot");
          WiFi.setTxPower(WIFI_POWER_11dBm);
					WiFi.begin(config.wifi_ssid, config.wifi_pass);
					// Wait up to 1 minute for connection...
					for (c = 0; (c < 30) && (WiFi.status() != WL_CONNECTED); c++) {
						Serial.write('.');
						vTaskDelay(1000 / portTICK_PERIOD_MS);
            rtc_wdt_feed();
						//for (t = millis(); (millis() - t) < 1000; refresh());
					}

					if (c >= 30) { // If it didn't connect within 1 min
						Serial.println(F("Failed. Will retry..."));
					}
					else {		
						beginM17();
						m17ConectTimeout = millis() + 1000;
#ifdef DEBUG
						Serial.println(F("WiFi connected"));
						Serial.print(F("Host IP address: "));
						Serial.println(WiFi.localIP());
						vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif			
            Serial.println(F("### You can config by websevice ###"));
            if(config.wifi_mode == WIFI_AP_STA){
              Serial.print(F("WiFi AP URL: http://"));
              Serial.println(local_IP);
            }
            Serial.print(F("WiFi STA URL: http://"));
            Serial.println(WiFi.localIP());
					}
				}
			}
			else {

				if (connect_status == DISCONNECTED) {
					if (millis() > (m17ConectTimeout + 10000)) {
            Serial.println(F("M17 Connecting.."));
						process_connect();
            timerAlarmEnable(timer); //Start sample audio when M17 First Connected.
					}else{
            timerAlarmDisable(timer); //Stop sample audio when M17 Disconnect
          }
				}
				else {
						readyReadM17();
						transmitM17();
				}

				if (millis() > NTP_Timeout) {
					NTP_Timeout = millis() + 86400000;
					//Serial.println(F("Config NTP"));
					//setSyncProvider(getNtpTime);
					configTime(3600 * timeZone, 0, "203.150.19.19", "0.pool.ntp.org", "1.pool.ntp.org");
					//topBar(WiFi.RSSI());
					vTaskDelay(3000 / portTICK_PERIOD_MS);
          if(systemUptime==0) systemUptime = now();
					// struct tm tmstruct;
					// getLocalTime(&tmstruct, 5000);
				}

				
				if (millis() > pingTimeout) {
					pingTimeout = millis() + 600000;
#ifdef DEBUG
					Serial.println("Ping Gateway to " + WiFi.gatewayIP().toString());
#endif
					if (ping_start(WiFi.gatewayIP(), 2, 0, 0, 5) == true) {
#ifdef DEBUG
						Serial.println(F("Ping Success!!"));
#endif
					}
					else {
#ifdef DEBUG
						Serial.println(F("Ping Fail!"));
#endif
						WiFi.disconnect();
						wifiTTL = 0;
					}
				}
			}
		}//wifi config
	}//Loop for
}