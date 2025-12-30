#ifndef THREDTTWOOD_TIMER_H
#define THREDTTWOOD_TIMER_H
//============================================================================
#include "types.h"
//--------------------------------------------------------
extern ULONG g_nTimerSignal;
//--------------------------------------------------------
extern int initTimer(void);
extern void killTimer(void);
extern void signalsTimer(void);
//============================================================================
#endif // THREDTTWOOD_TIMER_H
