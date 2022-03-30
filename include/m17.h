#include <Arduino.h>
#include <WiFiUdp.h>

#define M17CHARACTERS " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-/."

//ใช้ตัวแปรโกลบอลในไฟล์ main.cpp
extern Configuration config;
extern bool rxRef;
extern bool tx;
extern bool firstTX;
extern bool firstRX;
extern cppQueue	audioq;
extern cppQueue	pcmq;
//extern cppQueue	adcq;
extern int nsam, nbit, nbyte, i, frames, bits_proc, bit_errors, error_mode;
extern M17Flags connect_status;
extern int mode;
extern bool vox;

extern String srcCall,mycall, urCall, rptr1;

void beginM17();
uint16_t CRC_M17(uint16_t* crc_table, const uint8_t* message, uint16_t nBytes);
void readyReadM17();
void transmitM17();
void disconnect_from_host();
void M17encode_callsign(uint8_t* callsign);
void M17decode_callsign(uint8_t* callsign);
void process_ping();
void process_connect();
void terminateM17();
