#define BOARD_ROWS 6
#define BOARD_COLS 6

#define SUB_ROWS 3
#define SUB_COLS 3

#define EMPTY 0
/* BLACK and WHITE are used both to define player and color (marble), sometimes
 * even both cases in one place, which may not be very precise or logic, but
 * who cares? normal people just play games, not read their source code (no
 * offence to you, mate) */
#define BLACK 1
#define WHITE 2
/* in case of a draw, we use player NOBODY to tell who won, another intelligent idea */
#define NOBODY -1

#define MARBLE_WIN_NUM 5

