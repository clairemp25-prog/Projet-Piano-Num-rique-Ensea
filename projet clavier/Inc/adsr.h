/*
 * adsr.h
 *
 *  Created on: May 18, 2026
 *      Author: widad
 */


#ifndef INC_ADSR_H_
#define INC_ADSR_H_

#include "stm32l4xx_hal.h"

typedef enum {
    ADSR_ATTACK = 0,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
    ADSR_OFF
} ADSR_State_t;

typedef struct {
    float attack;     // vitesse montée
    float decay;      // vitesse descente
    float sustain;    // niveau sustain
    float release;    // vitesse release

    float level;      // niveau courant
    ADSR_State_t state;
} ADSR_t;

void ADSR_Init(ADSR_t *env, float a, float d, float s, float r);
void ADSR_NoteOn(ADSR_t *env);
void ADSR_NoteOff(ADSR_t *env);
float ADSR_Step(ADSR_t *env);


#endif /* INC_ADSR_H_ */
