#ifndef COLORUTIL_H
#define COLORUTIL_H

#include <exec/types.h>

void Represent(UWORD col, char * dest);

float ComponentToFraction(UWORD comp);
UWORD FractionToComponent(float frac);

void Separate(UWORD col, UWORD *r, UWORD *g, UWORD *b);
void SeparateFractions(UWORD col, float *r, float *g, float *b);
UWORD Combine(UWORD r, UWORD g, UWORD b);
UWORD CombineFractions(float r, float g, float b);

UWORD Blend(UWORD col0, UWORD col1, float bias);

#endif