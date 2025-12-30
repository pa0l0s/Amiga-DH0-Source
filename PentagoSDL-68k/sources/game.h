#include "common.h"

/* rotate directions */
#define CW 0
#define CCW 1
#define DIR_NOT_SET 2

/* move types */
#define ROTATE 0
#define PUT_MARBLE 1

extern int player;
extern int move_type;
extern int gameboard[BOARD_ROWS][BOARD_COLS];
