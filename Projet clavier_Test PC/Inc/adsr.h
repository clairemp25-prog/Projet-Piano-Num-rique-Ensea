/*
 * adsr.h
 *
 *  Created on: May 18, 2026
 *      Author: widad
 */

///debut modifié pour enlever les hal et afficher sur pc

#pragma once

typedef enum {
    ADSR_ATTACK = 0,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
    ADSR_OFF
} ADSR_State_t;

typedef struct {
    float attack;
    float decay;
    float sustain;
    float release;

    float level;
    ADSR_State_t state;
} ADSR_t;

void ADSR_Init(ADSR_t *env, float a, float d, float s, float r);
void ADSR_NoteOn(ADSR_t *env);
void ADSR_NoteOff(ADSR_t *env);
float ADSR_Step(ADSR_t *env);
