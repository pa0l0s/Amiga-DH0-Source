#ifndef BOXO_GAME_H
#define BOXO_GAME_H
//============================================================================
#include "types.h"
//--------------------------------------------------------
extern PVF g_pFnc;
//--------------------------------------------------------

extern void Title(void);
extern void NewGame(void);
extern void GameLoop(void);
extern void NextLevel(void);
extern void GameOver(void);
extern void Intro(void);
extern void Outro(void);
//============================================================================
#endif // BOXO_GAME_H
