#include "main.h"
#include "Voice.h"

extern unsigned long RxTimeout;

void createVoice(char *text)
{
    unsigned int start = 0;
    unsigned int length = 0;
    char symbol;
    size_t textSize = strlen(text);

    for (int j = 0U; j < 20; j++)
    {
        while (audioq.getCount() > (CODEC2_BUFF-16))
            delay(10);
        for (int k = 0; k < M17_PAYLOAD_LENGTH_BYTES; k++)
        {
            unsigned char au = M17_3200_SILENCE[k];
            audioq.push(&au);
        }
    }

    for (int t = 0; t < textSize; t++)
    {
        symbol = text[t];
        //Find text symbol
        for (int i = 0; i < sizeof(m17voice); i++)
        {
            if ((symbol == 0x20) || (symbol == '-') || (symbol == '.'))
            {
                for (int j = 0U; j < 10; j++)
                {
                    while (audioq.getCount() > (CODEC2_BUFF-16))
                        delay(10);
                    //Serial.printf("[%c,%d]", symbol, 16);
                    for (int k = 0; k < M17_PAYLOAD_LENGTH_BYTES; k++)
                    {
                        unsigned char au = M17_3200_SILENCE[k];
                        audioq.push(&au);
                        //Serial.print(au, HEX);
                    }
                    //Serial.println();
                }
                break;
            }
            else if (symbol == m17voice[i].symbol)
            {
                length = (m17voice[i].length + 1U) / 2U;
                start = m17voice[i].start * M17_3200_LENGTH_BYTES;
                unsigned char *audio = (unsigned char *)&en_Us[start];
                for (int j = 0U; j < length; j++)
                {
                    while (audioq.getCount() > (CODEC2_BUFF-16))
                        delay(10);
                    //Serial.printf("[%c,%d]", symbol, length);
                    for (int k = 0; k < M17_PAYLOAD_LENGTH_BYTES; k++)
                    {
                        unsigned char au = audio[k];
                        audioq.push(&au);
                        //Serial.print(au, HEX);
                    }
                    audio += M17_PAYLOAD_LENGTH_BYTES;
                    //Serial.println();
                }
                break;
            }
        }
    }
}

// void linkedTo(char *reflector)
// {
//     char text[50];
//     RxTimeout = millis() + 30000;

//     //sprintf(text, "a  b  c  d  l  n  g  y");
//     sprintf(text, "g %s l %c", config.reflector_name, config.reflector_module);
//     Serial.println(text);
//     createVoice(text);
//     Serial.println("Create Voice End.");
//     while (!pcmq.isEmpty())
//     {
//         if (millis() > RxTimeout)
//             break;
//         delay(50);
//     }
//     Serial.println("Create Terminate.");
//     RxTimeout = 0;
// }