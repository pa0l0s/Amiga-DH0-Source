#ifndef GFXUTIL_H
#define GFXUTIL_H

#include <exec/types.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <graphics/gfxbase.h>

extern struct GfxBase *GfxBase;
extern struct Custom custom;

BOOL initDisplay(void);
void resetDisplay(void);

#endif