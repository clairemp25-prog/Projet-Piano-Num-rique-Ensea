/*
 * karplus_strong.c
 *
 *  Created on: May 2, 2026
 *      Author: widad
 */


#include "karplus_strong.h"
#include <stdlib.h>


void KS_Init(KarplusStrong_t *ks, float frequency, float damping)
{
    uint16_t N = (uint16_t)(SAMPLE_RATE / frequency);
    if (N > MAX_BUFFER_SIZE) N = MAX_BUFFER_SIZE;
    if (N < 2) N = 2; // sécurité

    ks->size    = N;
    ks->index   = 0;
    ks->damping = damping;
    ks->active  = 1;

    // Bruit blanc entre -1.0 et 1.0
    for (uint16_t i = 0; i < N; i++) {
        ks->buffer[i] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    }
}

float KS_Step(KarplusStrong_t *ks)
{
	if (!ks->active) return 0.0f;

    float current = ks->buffer[ks->index];
    uint16_t next_index = (ks->index + 1) % ks->size;

    float avg = ks->damping * 0.5f *
                (ks->buffer[ks->index] + ks->buffer[next_index]);

    ks->buffer[ks->index] = avg;
    ks->index = next_index;

    return current;
}

