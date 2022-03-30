#ifndef I2S_H
#define I2S_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/dac.h"
#include "esp_system.h"

#define SAMPLE_RATE 16000
#define SAMPLE_RATE_CODEC2 8000
#define PIN_I2S_BCLK 26
#define PIN_I2S_LRC 5
#define PIN_I2S_DIN 36
#define PIN_I2S_DOUT 25

typedef enum { LEFTCHANNEL = 0, RIGHTCHANNEL = 1 } SampleIndex;

// This I2S specification : 
//  -   LRC high is channel 2 (right).
//  -   LRC signal transitions once each word.
//  -   DATA is valid on the CLOCK rising edge.
//  -   Data bits are MSB first.
//  -   DATA bits are left-aligned with respect to LRC edge.
//  -   DATA bits are right-shifted by one with respect to LRC edges.
//        It's valid for ADMP441 (microphone) and MAX98357A (speaker). 
//        It's not valid for SPH0645LM4H(microphone) and WM8960(microphon/speaker).
//
//  -   44100Hz
//  -   stereo

/// @parameter MODE : I2S_MODE_RX or I2S_MODE_TX
/// @parameter BPS : I2S_BITS_PER_SAMPLE_16BIT or I2S_BITS_PER_SAMPLE_32BIT
void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS);

/// I2S_Read() for I2S_MODE_RX
/// @parameter data: pointer to buffer
/// @parameter numData: buffer size
/// @return Number of bytes read
int I2S_Read(char* data, int numData);

/// I2S_Write() for I2S_MODE_TX
/// @param data: pointer to buffer
/// @param numData: buffer size
void I2S_Write(char* data, int numData);

bool ConsumeSample(int16_t sample[2], bool mono);
int16_t Amplify(int16_t s, float gainF2P6);
void MakeSampleStereo16(int16_t sample[2], char channels, char bps);

#endif
