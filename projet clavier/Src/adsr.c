/*
 * adsr.c
 *
 *  Created on: May 18, 2026
 *      Author: widad
 */


#include "adsr.h"

void ADSR_Init(ADSR_t *env, float a, float d, float s, float r)
{
    env->attack  = a;
    env->decay   = d;
    env->sustain = s;
    env->release = r;

    env->level = 0.0f;
    env->state = ADSR_OFF;
}

void ADSR_NoteOn(ADSR_t *env)
{
    env->state = ADSR_ATTACK;
}

void ADSR_NoteOff(ADSR_t *env)
{
    env->state = ADSR_RELEASE;
}

float ADSR_Step(ADSR_t *env)
{
    switch(env->state)
    {
        case ADSR_ATTACK:
            env->level += env->attack;
            if (env->level >= 1.0f) {
                env->level = 1.0f;
                env->state = ADSR_DECAY;
            }
            break;

        case ADSR_DECAY:
            env->level -= env->decay;
            if (env->level <= env->sustain) {
                env->level = env->sustain;
                env->state = ADSR_SUSTAIN;
            }
            break;

        case ADSR_SUSTAIN:
            // niveau constant
            break;

        case ADSR_RELEASE:
            env->level -= env->release;
            if (env->level <= 0.0f) {
                env->level = 0.0f;
                env->state = ADSR_OFF;
            }
            break;

        case ADSR_OFF:
        default:
            env->level = 0.0f;
            break;
    }

    return env->level;
}
