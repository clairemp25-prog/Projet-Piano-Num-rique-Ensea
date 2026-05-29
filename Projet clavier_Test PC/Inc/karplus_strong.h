/*
* karplus_strong.h
*
* Created on: May 4, 2026
* Author: widad
*/

///debut modifié pour enlever les hal et afficher sur pc

#pragma once
#include <stdint.h>

#define SAMPLE_RATE      16000
#define MAX_BUFFER_SIZE  512
#define NUM_NOTES        9

typedef struct {
    float    buffer[MAX_BUFFER_SIZE];
    uint16_t size;
    uint16_t index;
    float    damping;
    uint8_t  active;
} KarplusStrong_t;

void  KS_Init(KarplusStrong_t *ks, float frequency, float damping);
float KS_Step(KarplusStrong_t *ks);

