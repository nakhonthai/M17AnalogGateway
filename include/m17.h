#ifndef M17_H
#define M17_H

#include <Arduino.h>
#include <WiFiUdp.h>

#define M17CHARACTERS " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-/."

const unsigned int M17_CALLSIGN_LENGTH PROGMEM = 9U;

const unsigned int M17_NETWORK_FRAME_LENGTH PROGMEM = 54U;

const unsigned int M17_LSF_LENGTH_BITS PROGMEM = 240U;
const unsigned int M17_LSF_LENGTH_BYTES PROGMEM = M17_LSF_LENGTH_BITS / 8U;

const unsigned int M17_LICH_LENGTH_BITS  PROGMEM = 240U;
const unsigned int M17_LICH_LENGTH_BYTES PROGMEM = M17_LICH_LENGTH_BITS / 8U;

const unsigned int M17_LICH_FRAGMENT_LENGTH_BITS  PROGMEM = M17_LICH_LENGTH_BITS / 5U;
const unsigned int M17_LICH_FRAGMENT_LENGTH_BYTES PROGMEM = M17_LICH_FRAGMENT_LENGTH_BITS / 8U;

const unsigned int M17_LICH_FRAGMENT_FEC_LENGTH_BITS  PROGMEM = M17_LICH_FRAGMENT_LENGTH_BITS * 2U;
const unsigned int M17_LICH_FRAGMENT_FEC_LENGTH_BYTES PROGMEM = M17_LICH_FRAGMENT_FEC_LENGTH_BITS / 8U;

const unsigned int M17_PAYLOAD_LENGTH_BITS  PROGMEM = 128U;
const unsigned int M17_PAYLOAD_LENGTH_BYTES PROGMEM = M17_PAYLOAD_LENGTH_BITS / 8U;

const unsigned char M17_NULL_NONCE[] PROGMEM = { 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };
const unsigned int M17_META_LENGTH_BITS PROGMEM = 112U;
const unsigned int M17_META_LENGTH_BYTES PROGMEM = M17_META_LENGTH_BITS / 8U;

const unsigned int M17_FN_LENGTH_BITS  PROGMEM = 16U;
const unsigned int M17_FN_LENGTH_BYTES PROGMEM = M17_FN_LENGTH_BITS / 8U;

const unsigned int M17_CRC_LENGTH_BITS  PROGMEM = 16U;
const unsigned int M17_CRC_LENGTH_BYTES PROGMEM = M17_CRC_LENGTH_BITS / 8U;

const unsigned int M17_FRAME_TIME PROGMEM = 40U;

const unsigned int M17_3200_LENGTH_BYTES PROGMEM = 8U;
const unsigned char M17_3200_SILENCE[] PROGMEM = { 0x01U, 0x00U, 0x09U, 0x43U, 0x9CU, 0xE4U, 0x21U, 0x08U, 0x01U, 0x00U, 0x09U, 0x43U, 0x9CU, 0xE4U, 0x21U, 0x08U };

const unsigned char M17_PACKET_TYPE PROGMEM = 0U;
const unsigned char M17_STREAM_TYPE PROGMEM = 1U;

const unsigned char M17_DATA_TYPE_DATA       PROGMEM = 0x01U;
const unsigned char M17_DATA_TYPE_VOICE      PROGMEM = 0x02U;
const unsigned char M17_DATA_TYPE_VOICE_DATA PROGMEM = 0x03U;

const unsigned char M17_ENCRYPTION_TYPE_NONE     PROGMEM = 0x00U;
const unsigned char M17_ENCRYPTION_TYPE_AES      PROGMEM = 0x01U;
const unsigned char M17_ENCRYPTION_TYPE_SCRAMBLE PROGMEM = 0x02U;

const unsigned char M17_ENCRYPTION_SUB_TYPE_TEXT      PROGMEM = 0x00U;
const unsigned char M17_ENCRYPTION_SUB_TYPE_GPS       PROGMEM = 0x01U;
const unsigned char M17_ENCRYPTION_SUB_TYPE_CALLSIGNS PROGMEM = 0x02U;

const unsigned char M17_GPS_TYPE_FIXED    PROGMEM = 0x00U;
const unsigned char M17_GPS_TYPE_MOBILE   PROGMEM = 0x01U;
const unsigned char M17_GPS_TYPE_HANDHELD PROGMEM = 0x02U;

const unsigned char M17_GPS_CLIENT_M17CLIENT PROGMEM = 0x00U;
const unsigned char M17_GPS_CLIENT_OPENRTX   PROGMEM = 0x01U;

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

#endif