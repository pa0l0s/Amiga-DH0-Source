#ifndef BOXO_INTRO_H
#define BOXO_INTRO_H
//============================================================================
#include "types.h"
//--------------------------------------------------------

extern struct BitMap* g_pIntroBMap;

extern int g_nIntroWidth;
extern int g_nIntroHeight;
extern int g_nIntroFrameNr;
extern int g_nIntroFramesPerRow;


extern int initIntro(void);
extern void killIntro(void);
extern int runIntro(void);

//============================================================================
#endif // BOXO_INTRO_H
