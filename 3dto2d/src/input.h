#ifndef THREDTTWOOD_INPUT_H
#define THREDTTWOOD_INPUT_H
//============================================================================
#include "types.h"

//--------------------------------------------------------
extern BOOL g_bRight;
extern BOOL g_bLeft;
extern BOOL g_bUp;
extern BOOL g_bDown;
extern BOOL g_bFire;
extern ULONG g_nGamePortSignal;

extern BOOL SignalsInput(void);
extern int InitInput(void);
extern void KillInput(void);
//============================================================================
#endif // THREDTTWOOD_INPUT_H
