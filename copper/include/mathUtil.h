#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <exec/types.h>

void SampleSineWORD(WORD * dst, float offset, float amplitude, UWORD nrSamples);

void SampleSineFloat(float * dst, float offset, float amplitude, int nrSamples);
void SampleCosineFloat(float * dst, float offset, float amplitude, int nrSamples);

#endif