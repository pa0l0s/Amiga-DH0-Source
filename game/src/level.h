#ifndef BOXO_LEVEL_H
#define BOXO_LEVEL_H
//============================================================================
#include "types.h"
#include "tile.h"

//--------------------------------------------------------


extern UBYTE* g_pLevel;
extern int g_nAmountOfLvl;
extern int g_nLvlNumber;

extern int g_nLvlTWidth;
extern int g_nLvlTHeight;

extern int InitLevels(void);
extern void KillLevels(void);

extern void CopyLvl(int nLvlNumber);
extern void LvlToWin(void);

extern void CleanLevel(void);
extern void RockLevel(void);
//============================================================================
#endif // BOXO_LEVEL_H
