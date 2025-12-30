#include "game.h"
#include "common.h"

int player;
int move_type;
int gameboard[BOARD_ROWS][BOARD_COLS];

void clear_gameboard()
{
    int i, j;
    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++)
            gameboard[i][j] = EMPTY;
    }
}

int place_marble(int row, int col, int color)
{
    if (gameboard[row][col] == EMPTY)
        gameboard[row][col] = color;
    else
        return 0;  /* field already occupied */
    return 1;
}

int rotate_subboard(int x, int y, int dir)
{
    int i, j;
    int temp[SUB_ROWS][SUB_COLS];
    for (i = 0; i < SUB_ROWS; i++) {
        for (j = 0; j < SUB_COLS; j++)
            temp[i][j] = gameboard[SUB_ROWS*y + i][SUB_COLS*x + j];
    }
    if (dir == CW) {
        for (i = 0; i < SUB_ROWS; i++) {
            for (j = 0; j < SUB_COLS; j++)
                gameboard[SUB_ROWS*y + i][SUB_COLS*x + j] = temp[SUB_ROWS-1-j][i];
        }
    }
    else if (dir == CCW) {
        for (i = 0; i < SUB_ROWS; i++) {
            for (j = 0; j < SUB_COLS; j++)
                gameboard[SUB_ROWS*y + i][SUB_COLS*x + j] = temp[j][SUB_COLS-1-i];
        }
    }
    return 1;
}

int has_empty_field()
{
    int i, j;
    for (i = 0; i < BOARD_ROWS; i++ ) {
        for (j = 0; j < BOARD_COLS; j++) {
            if (gameboard[i][j] == EMPTY)
                return 1;
        }
    }
    return 0;
}

/* check for lines of _marble_num_ _color_ marbles;
 * at the moment the function can search only for continuous ones
 * (non-continuous will be usefull while writing AI player), it searches in all
 * rows and columns and in six diagonal lines, where a winning 5-marble line
 * can occur */
int check_for_lines(int color, int marble_num)
{
    int i, j, counter;

    counter = 0;
    /* check for lines in rows */
    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++) {
            if (j == 0 && gameboard[i][j] == EMPTY)
                continue;
            if (gameboard[i][j] == color)
                counter++;
            else if (gameboard[i][j] != color &&
                    j > 0)
                break;

            if (counter >= marble_num) {
                printf("We have a horizontal line in row %i.\n", i+1);
                return 1;
            }
        }
        counter = 0;
    }

    counter = 0;
    /* check for lines in cols */
    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++) {
            if (j == 0 && gameboard[j][i] == EMPTY)
                continue;
            if (gameboard[j][i] == color)
                counter++;
            else if (gameboard[j][i] != color &&
                    j > 0)
                break;
            if (counter >= marble_num) {
                printf("We have a vertical line in column %i.\n", i+1);
                return 1;
            }
        }
        counter = 0;
    }

    /* check for diagonal lines, left to right */
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (i == 0 && gameboard[i][i] == EMPTY)
            continue;
        if (gameboard[i][i] == color)
            counter++;
        else if (gameboard[i][i] != color &&
                i > 0)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (gameboard[i+1][i] == color)
            counter++;
        else if (gameboard[i+1][i] != color)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (gameboard[i][i+1] == color)
            counter++;
        else if (gameboard[i][i+1] != color)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }

    /* check for diagonal lines, right to left */
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (i == 0 && gameboard[i][BOARD_COLS-1-i] == EMPTY)
            continue;
        if (gameboard[i][BOARD_COLS-1-i] == color)
            counter++;
        else if (gameboard[i][BOARD_COLS-1-i] != color &&
                i > 0)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (gameboard[i][BOARD_COLS-2-i] == color)
            counter++;
        else if (gameboard[i][BOARD_COLS-2-i] != color &&
                i > 0)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }
    counter = 0;
    for (i = 0; i < BOARD_ROWS; i++) {
        if (gameboard[i+1][BOARD_COLS-1-i] == color)
            counter++;
        else if (gameboard[i+1][BOARD_COLS-1-i] != color &&
                i > 0)
            break;
        if (counter >= marble_num) {
            printf("We have a diagonal line.\n");
            return 1;
        }
    }

    return 0;
}

