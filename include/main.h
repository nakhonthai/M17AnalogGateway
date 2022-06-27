#ifndef MAIN_H
#define MAIN_H

#define VERSION "0.9"

//#define DEBUG

//#define I2S_INTERNAL

//#define SDCARD
#define OLED
//#define SA818
//#define SR_FRS

#ifdef SR_FRS
#ifndef SA818
#define SA818
#endif
#endif

//#define CONFIG_DISABLE_HAL_LOCKS 1

// #define MUTEX_LOCK()    do {} while (xSemaphoreTake(NULL, portMAX_DELAY) != pdPASS)
// #define MUTEX_UNLOCK()  xSemaphoreGive(NULL)
// xSemaphoreHandle _ledc_sys_lock = NULL;

#define WIFI_OFF_FIX 0
#define WIFI_AP_FIX 1
#define WIFI_STA_FIX 2
#define WIFI_AP_STA_FIX 3

#define IMPLEMENTATION FIFO

#define TZ 0	 // (utc+) TZ in hours
#define DST_MN 0 // use 60mn for summer time in some countries
#define TZ_MN ((TZ)*60)
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)

#define FORMAT_SPIFFS_IF_FAILED true

#define PKGLISTSIZE 10

#define CODEC2_BUFF 800
#define PCM_BUFF 4000

#include <Arduino.h>
// #include <FS.h>
// #include <SD.h>
// #include <SPIFFS.h>
#include <cppQueue.h>
#include <codec2.h>
#include "esp_ns.h"
#include "esp_agc.h"
//#include "esp_vad.h"
#include "soc/rtc_wdt.h"

#include "HardwareSerial.h"
#include "EEPROM.h"

enum M17Flags
{
	DISCONNECTED = 1 << 0,
	CONNECTING = 1 << 1,
	M17_AUTH = 1 << 2,
	M17_CONF = 1 << 3,
	M17_OPTS = 1 << 4,
	CONNECTED_RW = 1 << 5,
	CONNECTED_RO = 1 << 6
};

typedef struct Config_Struct
{
	char id[20];
	bool wifi_client;
	char wifi_ssid[32];
	char wifi_pass[64];
	char wifi_ap_ssid[32];
	char wifi_ap_pass[64];
	char wifi_power;
	bool aprs;
	char aprs_mycall[10];
	char aprs_host[20];
	char aprs_passcode[10];
	char aprs_filter[30];
	uint8_t aprs_ssid;
	uint16_t aprs_port;
	char authUser[20];
	char authPass[15];
	char reflector_host[30];
	char reflector_name[10];
	char reflector_module;
	uint16_t reflector_port;
	char mycall[10];
	char mymodule;
	float gps_lat;
	float gps_lon;
	float gps_alt;
	bool wifi;
	char wifi_mode; // WIFI_AP,WIFI_STA,WIFI_AP_STA,WIFI_OFF
	char wifi_ch;
	uint8_t volume;
	uint8_t mic;
	uint8_t vox_delay;
	uint8_t vox_level;
	bool sql;
	bool sql_active;
	int codec2_mode;
	bool noise;
	bool agc;
#ifdef SA818
	float freq_rx;
	float freq_tx;
	int offset_rx;
	int offset_tx;
	int tone_rx;
	int tone_tx;
	uint8_t band;
	uint8_t sql_level;
	bool rf_power;
#endif
	uint8_t wifi_protocol;
	bool dtmf;
	bool oled_enable;
	int oled_timeout;
	int8_t timeZone;
	bool vpn;
	bool modem;
	uint16_t wg_port;
	char wg_peer_address[16];
	char wg_local_address[16];
	char wg_netmask_address[16];
	char wg_gw_address[16];
	char wg_public_key[45];
	char wg_private_key[45];
} Configuration;

typedef struct pkgListStruct
{
	time_t time;
	char calsign[11];
	char ssid[5];
	unsigned int pkg;
	bool type;
	uint8_t symbol;
} pkgListType;

void saveEEPROM();
void defaultConfig();
String getValue(String data, char separator, int index);
boolean isValidNumber(String str);
void frmUpdate(String str);
void sendVoice(char *text);

#endif