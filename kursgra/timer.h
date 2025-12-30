#ifndef BOXO_TIMER_H
#define BOXO_TIMER_H
//============================================================================
#include "types.h"
//--------------------------------------------------------
extern ULONG g_nTimerSignal;
//--------------------------------------------------------
extern int initTimer(void);
extern void killTimer(void);
extern void signalsTimer(void);
//============================================================================
#endif // BOXO_TIMER_H
