/*
 Created:	1-Nov-2021 14:27:23
 Author:	HS5TQA/Atten
*/

#include <Arduino.h>
#include "main.h"
#include "m17.h"

extern bool voicTx;
extern bool linkToFlage;
extern bool notLinkFlage;
extern bool voiceIPFlage;

extern char current_module;

// The udp library class
WiFiUDP udp;

int ping_cnt = 0;

uint16_t txstreamid = 0;
uint16_t tx_cnt = 0;
unsigned short streamid = 0;
unsigned short frameid = 0;
unsigned long RxTimeout = 0;
unsigned long m17ConectTimeout = 0;

uint16_t CRC_M17(uint16_t *crc_table, const uint8_t *message, uint16_t nBytes)
{
	uint8_t data;
	uint16_t remainder = 0xFFFF;

	for (uint16_t byte = 0; byte < nBytes; byte++)
	{
		data = message[byte] ^ (remainder >> 8);
		remainder = crc_table[data] ^ (remainder << 8);
	}

	return (remainder);
}

void beginM17()
{
	uint16_t lport = 18000;
	lport += random(1, 10000);
	udp.begin(lport);
}

//อ่านแพ็คเก็จโปรโตคอลรีแฟล็กเตอร์ mref
void readyReadM17()
{
	uint8_t buf[100];
	int size = udp.parsePacket();
	if (size > 3)
	{
		udp.readBytes(buf, size);
		// Serial.print("RECV: ");
		// for (int i = 0; i < size; ++i) {
		//	Serial.printf("%02x ", (unsigned char)buf[i]);
		// }
		// Serial.println();
		if ((size == 4) && (memcmp(buf, "DISC", 4U) == 0))
		{
			notLinkFlage = true;
			pcmq.clean();
			audioq.clean();
			Serial.println("DISCONNECT Host: " + udp.remoteIP().toString() + ":" + String(udp.remotePort()));
			delay(500);
			connect_status = DISCONNECTED;
			m17ConectTimeout = millis();
		}
		if ((size == 4) && (memcmp(buf, "NACK", 4U) == 0))
		{
			Serial.println("NACK CONNECTED Host: " + udp.remoteIP().toString() + ":" + String(udp.remotePort()));
			m17ConectTimeout = millis();
		}
		if ((size == 4) && (memcmp(buf, "ACKN", 4U) == 0))
		{
			if (connect_status == CONNECTING)
			{
				pcmq.clean();
				audioq.clean();
				connect_status = CONNECTED_RW;
				Serial.println("CONNECTING Host: " + udp.remoteIP().toString() + ":" + String(udp.remotePort()) + " Ping: " + String(ping_cnt++));
			}
		}
		if ((size == 10) && (memcmp(buf, "PING", 4U) == 0))
		{
			m17ConectTimeout = millis();
			connect_status = CONNECTED_RW;
			process_ping();
			Serial.println("PONG Host: " + udp.remoteIP().toString() + ":" + String(udp.remotePort()) + " CNT: " + String(ping_cnt++));
		}
		if ((size == 54) && (memcmp(buf, "M17 ", 4U) == 0) && !tx && !vox)
		{
			uint8_t cs[10];
			uint8_t sz;

			RxTimeout = millis();
			memcpy(cs, &buf[12], 6);
			M17decode_callsign(cs);
			// ui->mycall->setText(QString((char*)cs));
			srcCall = String((char *)cs);
			memcpy(cs, &buf[6], 6);
			M17decode_callsign(cs);
			urCall = String((char *)cs);
			// ui->urcall->setText(QString((char*)cs));
			// Data type indicator, 01 =data (D), 10 =voice (V), 11 =V+D, 00 =reserved
			if ((buf[19] & 0x06U) == 0x04U)
			{
				rptr1 = "3200 F/R";
				mode = CODEC2_MODE_3200;
				sz = 16;
				nsam = 160;
			}
			else
			{
				rptr1 = "1600 V/D";
				mode = CODEC2_MODE_1600;
				sz = 8;
				nsam = 320;
			}
			streamid = (buf[4] << 8) | (buf[5] & 0xff);
			frameid = (buf[34] << 8) | (buf[35] & 0xff);

			// String ss = String("%1").arg(streamid, 4, 16, QChar('0'));
			// String n = String("%1").arg(fn, 4, 16, QChar('0'));
			// ui->rptr2->setText(n);
			// ui->streamid->setText(ss);
			if (frameid & 0x8000)
			{ // Frame Terminate
				RxTimeout = 0;
				pcmq.clean();
				audioq.clean();
				rxRef = true;
				return;
			}
			if (streamid == txstreamid && frameid == tx_cnt)
			{
				rxRef = true;
				return;
			}

			if (rxRef == false)
			{
				firstRX = true;
				pcmq.clean();
				audioq.clean();
				m17ConectTimeout = millis();
			}
			nbyte = sz;
			for (int i = 0; i < sz; ++i)
			{
				audioq.push(&buf[36 + i]);
			}
			rxRef = true;
		}
	}
}

void transmitM17()
{
	// FIELD	BYTES รูปแบบโปรโตคอล
	// prefix	4
	// SID		2
	// dst		10
	// src		10
	// type		2
	// nonce		14
	// fn		2
	// payload	16
	// crc_udp	2
	// uint16_t crc = 0;
	// char cs[10];
	// char d[20];
	char src[10];
	char dst[10];
	uint8_t txframe[55];
	if (tx && audioq.getCount() >= 16)
	{
		if (txstreamid == 0)
		{
			txstreamid = rand();
		}
		memset(dst, ' ', 9);
		memcpy(dst, config.reflector_name, strlen(config.reflector_name));
		// sprintf(&dst[0], "M17-THA  ");
		dst[8] = current_module;
		dst[9] = 0x00;
		M17encode_callsign((uint8_t *)dst);
		memset(src, ' ', 9);
		memcpy(src, config.mycall, strlen(config.mycall));
		// sprintf(&src[0], "HS5TQA   ");
		src[8] = config.mymodule;
		src[9] = 0x00;
		M17encode_callsign((uint8_t *)src);

		txframe[0] = 'M'; // MAGIC bytes 0x4d313720 ("M17 ")
		txframe[1] = '1';
		txframe[2] = '7';
		txframe[3] = ' ';
		// LICH (dst,src,streamtype,nonce)
		txframe[4] = txstreamid >> 8;
		txframe[5] = txstreamid & 0xff;
		memcpy(&txframe[6], dst, 6);
		memcpy(&txframe[12], src, 6);
		txframe[18] = 0;
		// Type field 00=none,01=no voice,10=3200bps,11=1600bps
		if (mode == CODEC2_MODE_1600)
		{
			txframe[19] = 0x06;
		}
		else
		{
			txframe[19] = 0x05; // Frame type voice only
		}
		memset(&txframe[20], 0x00, 14);
		// FN 16bit last frame at (FN & 0x8000)
		txframe[34] = tx_cnt >> 8;
		txframe[35] = tx_cnt & 0xff;
		// Payload 128bit
		int sz = 16;
		if (mode == CODEC2_MODE_1600)
			sz = 8;
		else
			sz = 16;
		for (int i = 0; i < sz; i++)
		{
			audioq.pop(&txframe[36 + i]);
		}

		// crc = CRC_M17(&hcrc, txframe, 52);//CRC_M17(CRC_LUT, out, 52);
		tx_cnt++;
		// if (++tx_cnt >= 0x8000)
		// 	tx_cnt = 0;

		udp.beginPacket(config.reflector_host, config.reflector_port);
		udp.write((uint8_t *)txframe, 54);
		udp.endPacket();
	}
}

void terminateM17()
{
	// FIELD	BYTES รูปแบบโปรโตคอล
	// prefix	4
	// SID		2
	// dst		10
	// src		10
	// type		2
	// nonce		14
	// fn		2
	// payload	16
	// crc_udp	2
	// uint16_t crc = 0;
	// char cs[10];
	// char d[20];
	char src[10];
	char dst[10];
	uint8_t txframe[55];
	memset(txframe, 0, 55);
	if (txstreamid == 0)
	{
		txstreamid = rand();
	}
	memset(dst, ' ', 9);
	memcpy(dst, config.reflector_name, strlen(config.reflector_name));
	// sprintf(&dst[0], "M17-THA  ");
	dst[8] = current_module;
	dst[9] = 0x00;
	M17encode_callsign((uint8_t *)dst);
	memset(src, ' ', 9);
	memcpy(src, config.mycall, strlen(config.mycall));
	src[8] = config.mymodule;
	src[9] = 0x00;
	M17encode_callsign((uint8_t *)src);

	txframe[0] = 'M'; // MAGIC bytes 0x4d313720 ("M17 ")
	txframe[1] = '1';
	txframe[2] = '7';
	txframe[3] = ' ';
	// LICH (dst,src,streamtype,nonce)
	txframe[4] = txstreamid >> 8;
	txframe[5] = txstreamid & 0xff;
	memcpy(&txframe[6], dst, 6);
	memcpy(&txframe[12], src, 6);
	txframe[18] = 0;
	// Type field 00=none,01=no voice,10=3200bps,11=1600bps
	if (mode == CODEC2_MODE_1600)
	{
		txframe[19] = 0x06;
	}
	else
	{
		txframe[19] = 0x05; // Frame type voice only
	}
	memset(&txframe[20], 0xFF, 14);
	// FN 16bit last frame at (FN & 0x8000)
	tx_cnt |= 0x8000;
	txframe[34] = tx_cnt >> 8;
	txframe[35] = tx_cnt & 0xff;
	// Payload 128bit
	int sz = 16;
	if (mode == CODEC2_MODE_1600)
		sz = 8;
	else
		sz = 16;
	for (int i = 0; i < sz; i++)
	{
		txframe[36 + i] = M17_3200_SILENCE[i];
		//audioq.pop(&txframe[36 + i]);
	}

	udp.beginPacket(config.reflector_host, config.reflector_port);
	udp.write((uint8_t *)txframe, 54);
	udp.endPacket();
}

//ยุติการเชื่อมต่อเซิร์ฟเวอร์รีแฟล็กเตอร์ M17
void disconnect_from_host()
{
	char cs[10];
	char d[100];
	int timeout = 0;
	while (!(tx_cnt & 0x8000))
	{
		delay(10);
		if (++timeout > 200)
			break;
	}
	memset(cs, ' ', 9);
	memcpy(&cs[0], config.mycall, strlen(config.mycall));
	// sprintf(&cs[0], "HS5TQA   ");
	cs[8] = config.mymodule;
	cs[9] = 0x00;
	M17encode_callsign((uint8_t *)cs);
	sprintf(&d[0], "DISC");
	memcpy(&d[4], cs, 6);
	// Send a packet
	udp.beginPacket(config.reflector_host, config.reflector_port);
	udp.write((uint8_t *)d, 10);
	udp.endPacket();
	//delay(500);
	//connect_status = DISCONNECTED;
	m17ConectTimeout = millis();
}

void M17encode_callsign(uint8_t *callsign)
{
	const std::string m17_alphabet(M17CHARACTERS);
	char cs[10];
	memset(cs, 0, sizeof(cs));
	memcpy(cs, callsign, strlen((char *)callsign));
	uint64_t encoded = 0;
	for (int i = strlen((char *)callsign) - 1; i >= 0; i--)
	{
		auto pos = m17_alphabet.find(cs[i]);
		if (pos == std::string::npos)
		{
			pos = 0;
		}
		encoded *= 40;
		encoded += pos;
	}
	for (int i = 0; i < 6; i++)
	{
		callsign[i] = (encoded >> (8 * (5 - i)) & 0xFFU);
	}
}

void M17decode_callsign(uint8_t *callsign)
{
	const std::string m17_alphabet(M17CHARACTERS);
	uint8_t code[6];
	uint64_t coded = callsign[0];
	for (int i = 1; i < 6; i++)
		coded = (coded << 8) | callsign[i];
	if (coded > 0xee6b27ffffffu)
	{
		// std::cerr << "Callsign code is too large, 0x" << std::hex << coded << std::endl;
		return;
	}
	memcpy(code, callsign, 6);
	memset(callsign, 0, 10);
	int i = 0;
	while (coded)
	{
		callsign[i++] = m17_alphabet[coded % 40];
		coded /= 40;
	}
}

//ตรวจสอบการเชื่อมต่อกับเซิร์ฟเวอร์รีแฟล็กเตอร์ M17 Ping<>Pong
void process_ping()
{
	char cs[10];
	char d[100];
	memset(cs, ' ', 9);
	memcpy(&cs[0], config.mycall, strlen(config.mycall));
	// sprintf(&cs[0], "HS5TQA   ");
	cs[8] = config.mymodule;
	cs[9] = 0x00;
	M17encode_callsign((uint8_t *)cs);
	sprintf(&d[0], "PONG");
	memcpy(&d[4], cs, 6);
	// Send a packet
	udp.beginPacket(config.reflector_host, config.reflector_port);
	udp.write((uint8_t *)d, 10);
	udp.endPacket();
}

//ฟังค์ชั่นขอเชื่อมต่อกับเซิร์ฟเวอร์รีแฟล็กเตอร์ M17
void process_connect()
{
	char cs[10];
	char d[20];
	memset(cs, ' ', 9);
	memcpy(&cs[0], config.mycall, strlen(config.mycall));
	cs[8] = config.mymodule;
	cs[9] = 0x00;
	M17encode_callsign((uint8_t *)cs);
	sprintf(&d[0], "CONN");
	memcpy(&d[4], cs, 6);
	d[10] = current_module;
	// Send a packet
	udp.beginPacket(config.reflector_host, config.reflector_port);
	udp.write((uint8_t *)d, 11);
	udp.endPacket();
	connect_status = CONNECTING;
	m17ConectTimeout = millis();
	linkToFlage = true;
}