#include <SDL/SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "common.h"
#include "display.h"
#include "game.h"

SDL_Surface *main_screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *marbles = NULL;
SDL_Surface *rotations = NULL;
SDL_Surface *cw_img = NULL;
SDL_Surface *ccw_img = NULL;
SDL_Surface *move_info = NULL;
SDL_Surface *game_result_msg = NULL;
SDL_Surface *play_again_msg = NULL;
SDL_Surface *icon = NULL;

TTF_Font *font = NULL;

SDL_Color font_color = {0, 0, 0};
int font_size = 24;

void load_images()
{
    if (icon == NULL)
        icon = load_img("graphics/icon.png");
    if (cw_img == NULL)
        cw_img = load_img("graphics/cw.png");
    if (ccw_img == NULL)
        ccw_img = load_img("graphics/ccw.png");
    if (background == NULL)
        background = load_img("graphics/background.png");
}

void create_surfaces()
{
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (marbles == NULL)
        marbles = SDL_CreateRGBSurface(SDL_SWSURFACE, BOARD_WIDTH, BOARD_HEIGHT, 32, rmask, gmask, bmask, amask);
    if (rotations == NULL)
        rotations = SDL_CreateRGBSurface(SDL_SWSURFACE, BOARD_WIDTH, BOARD_HEIGHT, 32, rmask, gmask, bmask, amask);
}

SDL_Rect rect(int x, int y, int w, int h)
{
    SDL_Rect temp = {0, 0, 0, 0};
    temp.x = x;
    temp.y = y;
    temp.w = w;
    temp.h = h;
    return temp;
}

void clear_surface(SDL_Surface *screen)
{
    SDL_FillRect(screen, NULL, 0xffffff);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void initialize_SDL()
{
    SDL_Surface *txt;
    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");

    main_screen = SDL_SetVideoMode(MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT, 32, SDL_SWSURFACE);

    SDL_WM_SetIcon(icon, NULL);
    SDL_WM_SetCaption(WIN_TITLE, WIN_TITLE);

    printf("Initializing SDL_ttf...\n");
    if (TTF_Init() == -1) {
        printf( "Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        exit(-1);
    }
    printf("SDL_ttf initialized.\n");
    font  = TTF_OpenFont("aklatan.ttf", font_size);
}

void prepare_graphics()
{
    SDL_Rect dest = {0, 0, 0, 0};

    load_images();
    create_surfaces();
    /* we draw the gameboard on the background surface, it makes it easier
     * later to reset the display */
    draw_gameboard(background);


    /* below we prepare the rotations surface */
    /* first the semi-transparent white box: */
    boxRGBA(rotations, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, 255, 255, 255, 200);
    /* and now all four counterclockwise arrows: */
    dest = rect(2*LINE_THICKNESS + FIELD_WIDTH/2, 2*LINE_THICKNESS + FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(ccw_img, NULL, rotations, &dest);
    dest = rect(LINE_THICKNESS + BOARD_WIDTH/2 + FIELD_WIDTH/2, 2*LINE_THICKNESS + FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(ccw_img, NULL, rotations, &dest);
    dest = rect(2*LINE_THICKNESS + FIELD_WIDTH/2, BOARD_HEIGHT/2 + FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(ccw_img, NULL, rotations, &dest);
    dest = rect(LINE_THICKNESS + BOARD_WIDTH/2 + FIELD_WIDTH/2, BOARD_HEIGHT/2 + FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(ccw_img, NULL, rotations, &dest);
    /* and the clockwise ones: */
    dest = rect(3*LINE_THICKNESS + 3*FIELD_WIDTH/2, 3*LINE_THICKNESS + 3*FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(cw_img, NULL, rotations, &dest);
    dest = rect(2*LINE_THICKNESS + BOARD_WIDTH/2 + 3*FIELD_WIDTH/2, 3*LINE_THICKNESS + 3*FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(cw_img, NULL, rotations, &dest);
    dest = rect(3*LINE_THICKNESS + 3*FIELD_WIDTH/2, 2*LINE_THICKNESS + BOARD_HEIGHT/2 + 3*FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(cw_img, NULL, rotations, &dest);
    dest = rect(2*LINE_THICKNESS + BOARD_WIDTH/2 + 3*FIELD_WIDTH/2, 2*LINE_THICKNESS + BOARD_HEIGHT/2 + 3*FIELD_HEIGHT/2, 56, 56);
    SDL_BlitSurface(cw_img, NULL, rotations, &dest);


    dest = rect(0, 0, MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT);
    SDL_BlitSurface(background, NULL, main_screen, &dest);
}

SDL_Surface *load_img(char *filename)
{
    SDL_Surface *image = NULL;
    image = IMG_Load(filename);
    return image;
}

void free_surfaces()
{
    SDL_FreeSurface(icon);
    icon = NULL;
    SDL_FreeSurface(play_again_msg);
    play_again_msg = NULL;
    SDL_FreeSurface(game_result_msg);
    game_result_msg = NULL;
    SDL_FreeSurface(move_info);
    move_info = NULL;
    SDL_FreeSurface(ccw_img);
    ccw_img = NULL;
    SDL_FreeSurface(cw_img);
    cw_img = NULL;
    SDL_FreeSurface(rotations);
    rotations = NULL;
    SDL_FreeSurface(marbles);
    marbles = NULL;
    SDL_FreeSurface(background);
    background = NULL;
    SDL_FreeSurface(main_screen);
    main_screen = NULL;
}

void draw_gameboard(SDL_Surface *screen)
{
    int i, j;

    /* draw board vertical lines */
    for (i = 0; i < BOARD_WIDTH; i+=FIELD_WIDTH) {
        if (i % 3) {
            for (j = 0; j < LINE_THICKNESS; j++, i++)
                vlineRGBA(screen, BOARD_LEFT_MARGIN+i, BOARD_TOP_MARGIN, BOARD_TOP_MARGIN+BOARD_HEIGHT-1, 0, 0, 0, 255); /* why the hell -1? */
        }
        else {
            for (j = 0; j < 2*LINE_THICKNESS; j++, i++)
                vlineRGBA(screen, BOARD_LEFT_MARGIN+i, BOARD_TOP_MARGIN, BOARD_TOP_MARGIN+BOARD_HEIGHT-1, 0, 0, 0, 255); /* why the hell -1? */
        }
    }
    /* draw board horizontal lines */
    for (i = 0; i < BOARD_HEIGHT; i+=FIELD_HEIGHT) {
        if (i % 3) {
            for (j = 0; j < LINE_THICKNESS; j++, i++)
                hlineRGBA(screen, BOARD_LEFT_MARGIN, BOARD_LEFT_MARGIN+BOARD_WIDTH-1, BOARD_TOP_MARGIN+i, 0, 0, 0, 255);
        }
        else {
            for (j = 0; j < 2*LINE_THICKNESS; j++, i++)
                hlineRGBA(screen, BOARD_LEFT_MARGIN, BOARD_LEFT_MARGIN+BOARD_WIDTH-1, BOARD_TOP_MARGIN+i, 0, 0, 0, 255);
        }
    }
}

void draw_marble(SDL_Surface *screen, int row, int col, int color)
{
    int x, y;
    x = (col+1)*LINE_THICKNESS + (col/3 + 1)*LINE_THICKNESS + col*FIELD_WIDTH + FIELD_WIDTH/2;
    y = (row+1)*LINE_THICKNESS + (row/3 + 1)*LINE_THICKNESS + row*FIELD_HEIGHT + FIELD_HEIGHT/2;
    if (color == WHITE) {
        filledCircleRGBA(screen, x, y, MARBLE_RAD, 255, 255, 255, 255);
        circleRGBA(screen, x, y, MARBLE_RAD, 0, 0, 0, 255);
    }
    else if (color == BLACK)
        filledCircleRGBA(screen, x, y, MARBLE_RAD, 0, 0, 0, 255);
}

void update_gameboard()
{
    int i, j;
    SDL_Rect dest = {0, 0, 0, 0};

    clear_surface(marbles);

    for (i = 0; i < BOARD_ROWS; i++) {
        for (j = 0; j < BOARD_COLS; j++) {
            if (gameboard[i][j] == EMPTY)
                continue;
            else if (gameboard[i][j] == WHITE)
                draw_marble(marbles, i, j, WHITE);
            else if (gameboard[i][j] == BLACK)
                draw_marble(marbles, i, j, BLACK);
        }
    }
    dest = rect(BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
    SDL_BlitSurface(marbles, NULL, main_screen, &dest);
}

/* refreshes the background and marbles placement */
void refresh_window()
{
    SDL_Rect dest = {0, 0, 0, 0};

    SDL_BlitSurface(background, NULL, main_screen, NULL);
    dest = rect(BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
    SDL_BlitSurface(marbles, NULL, main_screen, &dest);
    SDL_Flip(main_screen);
}

void show_move_info(int player, int move_type)
{
    SDL_Rect dest = {0, 0, 0, 0};

    if (player == WHITE) {
        if (move_type == PUT_MARBLE)
            move_info = TTF_RenderText_Blended(font, "Player White: place a marble", font_color);
        else if (move_type == ROTATE)
            move_info = TTF_RenderText_Blended(font, "Player White: rotate", font_color);
    }
    if (player == BLACK) {
        if (move_type == PUT_MARBLE)
            move_info = TTF_RenderText_Blended(font, "Player Black: place a marble", font_color);
        else if (move_type == ROTATE)
            move_info = TTF_RenderText_Blended(font, "Player Black: rotate", font_color);
    }
    dest = rect(MAIN_WIN_WIDTH/2 - move_info->w/2, MAIN_WIN_HEIGHT - 2*move_info->h, move_info->w, move_info->h);
    SDL_BlitSurface(move_info, NULL, main_screen, &dest);
    SDL_UpdateRect(main_screen, MAIN_WIN_WIDTH/2 - move_info->w/2, MAIN_WIN_HEIGHT - 2*move_info->h, move_info->w, move_info->h);
}

void show_game_result(int winner)
{
    SDL_Rect dest = {0, 0, 0, 0};

    if (winner == WHITE)
        game_result_msg = TTF_RenderText_Blended(font, "Player White wins!", font_color);
    else if (winner == BLACK)
        game_result_msg = TTF_RenderText_Blended(font, "Player Black wins!", font_color);
    else if (winner == NOBODY)
        game_result_msg = TTF_RenderText_Blended(font, "We have a draw.", font_color);
    dest = rect(MAIN_WIN_WIDTH/2 - game_result_msg->w/2, BOARD_TOP_MARGIN/2 - game_result_msg->h, game_result_msg->w, game_result_msg->h);
    SDL_BlitSurface(game_result_msg, NULL, main_screen, &dest);
    SDL_UpdateRect(main_screen, MAIN_WIN_WIDTH/2 - game_result_msg->w/2, BOARD_TOP_MARGIN/2 - game_result_msg->h, game_result_msg->w, game_result_msg->h);

    play_again_msg = TTF_RenderText_Blended(font, "Play again? Press (Y)es or (N)o", font_color);
    dest = rect(MAIN_WIN_WIDTH/2 - play_again_msg->w/2, MAIN_WIN_HEIGHT - 2*play_again_msg->h, play_again_msg->w, play_again_msg->h);
    SDL_BlitSurface(play_again_msg, NULL, main_screen, &dest);
    SDL_UpdateRect(main_screen, MAIN_WIN_WIDTH/2 - play_again_msg->w/2, MAIN_WIN_HEIGHT - 2*play_again_msg->h, play_again_msg->w, play_again_msg->h);
}

void stop_SDL()
{
    TTF_CloseFont(font);
    font = NULL;

    SDL_Quit();
}
