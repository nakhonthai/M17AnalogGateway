/*
Heavily modified by El_Supremo (Arduino forums) to decode DTMF tones
It is also public domain and provided on an AS-IS basis. There's no warranty
or guarantee of ANY kind whatsoever.

The Goertzel algorithm is long standing so see
http://en.wikipedia.org/wiki/Goertzel_algorithm for a full description.
It is often used in DTMF tone detection as an alternative to the Fast
Fourier Transform because it is quick with low overheard because it
is only searching for a single frequency rather than showing the
occurrence of all frequencies.
This work is entirely based on the Kevin Banks code found at
http://www.eetimes.com/design/embedded/4024443/The-Goertzel-Algorithm
so full credit to him for his generic implementation and breakdown. I've
simply massaged it into an Arduino library. I recommend reading his article
for a full description of whats going on behind the scenes.

Created by Jacob Rosenthal, June 20, 2012.
Released into the public domain.
*/

// ensure this library description is only included once
#ifndef DTMF_h
#define DTMF_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class DTMF
{
  // user-accessible "public" interface
  public:
    DTMF(float,float);
    DTMF(float);
void sample(int);
void detect(float dtmf_mag[],float adc[]);
char button(float mags[],float magnitude);
  // library-accessible "private" interface
  private:

//El_Supremo change to int
void ProcessSample(float);
void ResetDTMF(void);

};

#endif

