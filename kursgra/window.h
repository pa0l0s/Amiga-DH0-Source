#ifndef BOXO_WINDOW_H
#define BOXO_WINDOW_H
//============================================================================
#include "types.h"

#define PENS_AMOUNT 2
//--------------------------------------------------------
extern struct RastPort* g_pRpMain;
extern ULONG g_nWindowSignal;
extern ULONG g_tabPens[PENS_AMOUNT];
//--------------------------------------------------------
extern BOOL signalsWindow(void);
extern int initWindow(void);
extern void killWindow(void);
//============================================================================
#endif // BOXO_WINDOW_H
