#ifndef SCGEO_H
#define SCGEO_H

#include <exec/types.h>

void scGeoInit(float rb, float rx, float ry, float fovC, float xC, int animFrames);
void scGeoRelease();

BOOL initColorArrays(UWORD * gradient, UWORD nrColors);
void releaseColorArrays();

UWORD calcLinesAndColors(UWORD * lines, UWORD * colors, int index);

void LogParameters();

#endif