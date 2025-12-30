#ifndef COLORARRAYUTIL_H
#define COLORARRAYUTIL_H

#include <exec/types.h>

BOOL InterpolateColors(UWORD col0, UWORD col1, UWORD nrSamples, UWORD * samples);
void PrintColors(UWORD * colors, UWORD nrColors);
void MirrorColors(UWORD * src, UWORD * dst, UWORD nrColors);
// dst has src appended to it -- so it should be big enough
void AppendColors(UWORD * dst, UWORD * src, UWORD nrDst, UWORD nrSrc, BOOL skip1);

void SeparateFractionsColors(UWORD * colors, float *r, float *g, float *b, UWORD nrColors);
void CombineFractionsColors(float * r, float * g, float * b, UWORD * colors, UWORD nrColors);

BOOL DownSampleColors(UWORD * src, UWORD nrSrc, UWORD * dst, UWORD nrDst);

#endif