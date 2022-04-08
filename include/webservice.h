#include <Update.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <time.h>
#include <TimeLib.h>

#include "main.h"

//ใช้ตัวแปรโกลบอลในไฟล์ main.cpp
extern Configuration config;
extern TaskHandle_t taskNetworkHandle;
extern TaskHandle_t taskDSPHandle;
extern TaskHandle_t taskUIHandle;
extern time_t systemUptime;
extern pkgListType pkgList[PKGLISTSIZE];

const float ctcss[] PROGMEM = {0,67,71.9,74.4,77,79.7,82.5,85.4,88.5,91.5,94.8,97.4,100,103.5,107.2,110.9,114.8,118.8,123,127.3,131.8,136.5,141.3,146.2,151.4,156.7,162.2,167.9,173.8,179.9,186.2,192.8,203.5,210.7,218.1,225.7,233.6,241.8,250.3};
const float wifiPwr[12][2] PROGMEM ={{-4,-1},{8,2},{20,5},{28,7},{34,8.5},{44,11},{52,13},{60,15},{68,17},{74,18.5},{76,19},{78,19.5}};

#ifdef __cplusplus
extern "C" {
#endif
	uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

void serviceHandle();
void setHTML(byte page);
void handle_root();
void handle_setting();
void handle_service();
void handle_system();
void handle_firmware() ;
void handle_default();
#ifdef SDCRAD
void handle_storage();
void handle_download();
void handle_delete();
void listDir(fs::FS& fs, const char* dirname, uint8_t levels);
#endif
void webService();
#ifdef SA818
void handle_radio();
extern void SA818_INIT(bool boot);
#endif
