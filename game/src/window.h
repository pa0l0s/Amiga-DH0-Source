#ifndef BOXO_WINDOW_H
#define BOXO_WINDOW_H
//============================================================================
#include "types.h"
//--------------------------------------------------------
extern int g_nWinWidth;
extern int g_nWinHeight;

extern struct RastPort* g_pRpMain;
extern ULONG g_nWindowSignal;
extern struct ViewPort* g_pViewPort;
//--------------------------------------------------------
extern BOOL signalsWindow(void);
extern int initWindow(void);
extern void killWindow(void);
//============================================================================
#endif // BOXO_WINDOW_H
