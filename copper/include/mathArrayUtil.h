#ifndef MATHARRAYUTIL_H
#define MATHARRAYUTIL_H

# include "mathUtil.h"

#include <exec/types.h>

void FindMinMaxFloat(float * src, int len, float * min, float * max);
void FindMinMaxUWORD(UWORD * src, int len, UWORD * min, UWORD * max);

float CalcMeanFloat(float * src, int nrSrc);
int IndicesWithinRangeFloat(float * src, int nrSrc, int * dst, float min, float max);

BOOL DownSampleFloat(float * src, int nrSrc, float * dst, int nrDst);

#endif