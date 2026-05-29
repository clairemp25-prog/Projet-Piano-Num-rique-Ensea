/*
* karplus_strong.h
*
* Created on: May 4, 2026
* Author: widad
*/



#ifndef INC_KARPLUS_STRONG_H_
#define INC_KARPLUS_STRONG_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

#define SAMPLE_RATE      16000      // Fréquence d'échantillonnage (Hz)
#define MAX_BUFFER_SIZE  512        // Taille max du buffer
#define NUM_NOTES        9          // 1 octave + 1 touche

typedef struct {
    float    buffer[MAX_BUFFER_SIZE];
    uint16_t size;      // N = SAMPLE_RATE / freq
    uint16_t index;     // index circulaire
    float    damping;   // facteur d'atténuation
    uint8_t  active;
} KarplusStrong_t;



void  KS_Init(KarplusStrong_t *ks, float frequency, float damping);
float KS_Step(KarplusStrong_t *ks);

#endif /* INC_KARPLUS_STRONG_H_ */

