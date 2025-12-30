#define MAIN_WIN_HEIGHT 550
#define MAIN_WIN_WIDTH 550

#define WIN_TITLE "PentagoSDL"

#define BOARD_LEFT_MARGIN ((MAIN_WIN_WIDTH - BOARD_WIDTH)/2)
#define BOARD_TOP_MARGIN ((MAIN_WIN_HEIGHT - BOARD_HEIGHT)/2)

#define LINE_THICKNESS 3
#define FIELD_WIDTH 50
#define FIELD_HEIGHT 50

#define BOARD_WIDTH ((BOARD_COLS*FIELD_WIDTH)+(BOARD_COLS+4)*LINE_THICKNESS)
#define BOARD_HEIGHT ((BOARD_ROWS*FIELD_HEIGHT)+(BOARD_ROWS+4)*LINE_THICKNESS)

#define MARBLE_RAD (FIELD_WIDTH/2) - 3 /* marble radius */

extern SDL_Surface *main_screen;
extern SDL_Surface *background;
extern SDL_Surface *marbles;
extern SDL_Surface *rotations;
extern SDL_Surface *cw_img;
extern SDL_Surface *ccw_img;
extern SDL_Surface *player_info;
extern SDL_Surface *icon;

extern SDL_Color font_color;

SDL_Rect rect(int x, int y, int w, int h);
SDL_Surface *load_img(char *filename);
void load_images();
void draw_marble(SDL_Surface *screen, int row, int col, int color);
void draw_gameboard(SDL_Surface *screen);
