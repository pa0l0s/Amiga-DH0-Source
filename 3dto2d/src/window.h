#ifndef THREDTTWOOD_WINDOW_H
#define THREDTTWOOD_WINDOW_H
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
extern initDefaultWindow(void);
extern int initWindow(int winWidth, int winHeight);
extern void killWindow(void);
extern void printText(char text[], int x, int y, int pen);
extern int getTextXCenter(char text[]);
extern void windowSetAPen(int pen);
extern void windowMove(int x,int y);
extern void windowDraw(int x,int y);
extern void windowClear(void);
//============================================================================
#endif // THREDTTWOOD_WINDOW_H
