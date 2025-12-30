#include <stdio.h>
#include <SDL/SDL.h>
#include "common.h"
#include "display.h"
#include "game.h"

enum game_states {
    PLAY = 0,
    WIN = 1,
    DRAW = 2,
    QUIT = 3
};

int game;

int game_end(int player)
{
    if (!has_empty_field()) {
        game = DRAW;
        show_game_result(NOBODY);
        return 1;
    }
    if (check_for_lines(player, MARBLE_WIN_NUM)) {
        game = WIN;
        show_game_result(player);
        return 1;
    }
    return 0;
}

int
main(void)
{
    int i, j;
    int row, col, sub_x, sub_y, dir;
    SDL_Event event;
    SDL_Rect dest = {0, 0, 0, 0};

    initialize_SDL();
    prepare_graphics();

    clear_gameboard();

    SDL_Flip(main_screen);

    player = WHITE;
    move_type = PUT_MARBLE;
    game = PLAY;

    show_move_info(player, move_type);

    while (game != QUIT) {

        SDL_WaitEvent(&event);
        switch (event.type) {

            case SDL_QUIT:
                game = QUIT;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q)
                    game = QUIT;
                /* if a game is won or we have a draw, the program will surely
                 * ask whether we want to play again or not, so we read
                 * (Y)es/(N)o */
                if (game == WIN || game == DRAW) {
                    if (event.key.keysym.sym == SDLK_y) {
                        clear_gameboard();
                        player = WHITE;
                        move_type = PUT_MARBLE;
                        update_gameboard();
                        refresh_window();
                        SDL_UpdateRect(main_screen, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                        show_move_info(player, move_type);
                        game = PLAY;
                    }
                    if (event.key.keysym.sym == SDLK_n)
                        game = QUIT;
                }       
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (game == PLAY) {
                
                    if (move_type == PUT_MARBLE) {
                        if (event.motion.x > (BOARD_LEFT_MARGIN + 2*LINE_THICKNESS) &&
                                event.motion.x < (BOARD_LEFT_MARGIN + BOARD_WIDTH - 2*LINE_THICKNESS) &&
                                event.motion.y > (BOARD_TOP_MARGIN + LINE_THICKNESS) &&
                                event.motion.y < (BOARD_TOP_MARGIN + BOARD_HEIGHT - 2*LINE_THICKNESS)) {
                            col = (event.motion.x - BOARD_LEFT_MARGIN - 2*LINE_THICKNESS) / ( (4/3)*LINE_THICKNESS + FIELD_WIDTH);
                            row = (event.motion.y - BOARD_TOP_MARGIN - 2*LINE_THICKNESS) / ( (4/3)*LINE_THICKNESS + FIELD_HEIGHT);

                            /* if a marble is placed, we:
                             * - refresh the window to get rid of the text,
                             * - update the marbles surface (marbles placement)
                             *   according to the gameboard[][] and blit to 
                             *   main_screen,
                             * - blit the rotations surface (preparation to the
                             *   rotation move,
                             * - refresh the gameboard view,
                             * - check whether we have a winner (and a weenie)
                             *   or a draw,
                             * - finally we change the move type to rotation.
                             */
                            if (player == WHITE) {
                                if (place_marble(row, col, player)) {
                                    refresh_window();
                                    update_gameboard();
                                    dest = rect(BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                                    SDL_BlitSurface(rotations, NULL, main_screen, &dest);
                                    SDL_UpdateRect(main_screen, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                                    if (game_end(player))
                                        continue;
                                    move_type = ROTATE;
                                }
                            }
                            else if (player == BLACK) {
                                if (place_marble(row, col, player)) {
                                    refresh_window();
                                    update_gameboard();
                                    dest = rect(BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                                    SDL_BlitSurface(rotations, NULL, main_screen, &dest);
                                    SDL_UpdateRect(main_screen, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                                    if (game_end(player))
                                        continue;
                                    move_type = ROTATE;
                                }
                            }
                        }
                    }
                    else if (move_type == ROTATE) {
                        if (event.motion.x > BOARD_LEFT_MARGIN &&
                                event.motion.x < BOARD_LEFT_MARGIN + BOARD_WIDTH &&
                                event.motion.y > BOARD_TOP_MARGIN &&
                                event.motion.y < BOARD_TOP_MARGIN + BOARD_HEIGHT) {

                            if (event.motion.x < BOARD_LEFT_MARGIN + BOARD_WIDTH/2) {
                                sub_x = 0;
                            }
                            else if (event.motion.x > BOARD_LEFT_MARGIN + BOARD_WIDTH/2) {
                                sub_x = 1;
                            }

                            if (event.motion.y < BOARD_TOP_MARGIN + BOARD_HEIGHT/2) {
                                sub_y = 0;
                            }
                            else if (event.motion.y > BOARD_TOP_MARGIN + BOARD_HEIGHT/2) {
                                sub_y = 1;
                            }

                            if (event.motion.x < BOARD_LEFT_MARGIN + BOARD_WIDTH/4 ||
                                    (event.motion.x > BOARD_LEFT_MARGIN + BOARD_WIDTH/2 &&
                                     event.motion.x < BOARD_LEFT_MARGIN + 3*BOARD_WIDTH/4)) {
                                if (event.motion.y < BOARD_TOP_MARGIN + BOARD_HEIGHT/4 ||
                                        (event.motion.y > BOARD_TOP_MARGIN + BOARD_HEIGHT/2 &&
                                         event.motion.y < BOARD_TOP_MARGIN + 3*BOARD_HEIGHT/4)) {
                                    dir = CCW;
                                }
                            }
                            else if ((event.motion.x > BOARD_LEFT_MARGIN + BOARD_WIDTH/4 &&
                                        event.motion.x < BOARD_LEFT_MARGIN + BOARD_WIDTH/2) ||
                                    event.motion.x > BOARD_LEFT_MARGIN + 3*BOARD_WIDTH/4) {
                                if ((event.motion.y > BOARD_TOP_MARGIN + BOARD_HEIGHT/4 &&
                                            event.motion.y < BOARD_TOP_MARGIN + BOARD_HEIGHT/2) ||
                                        event.motion.y > BOARD_TOP_MARGIN + 3*BOARD_HEIGHT/4) {
                                    dir = CW;
                                }
                            }

                            if (dir == CW || dir == CCW) {
                                /* if a subboard was rotated successfully, we:
                                 * - update the marbles placement according to
                                 *   gameboard[][],
                                 * - refresh the window to get rid of any text
                                 *   and the rotations surface,
                                 * - refresh the gameboard view,
                                 * - check whether the game has ended,
                                 * - change move type to marble placement
                                 */
                                if (rotate_subboard(sub_x, sub_y, dir)) {
                                    update_gameboard();
                                    refresh_window();
                                    SDL_UpdateRect(main_screen, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, BOARD_WIDTH, BOARD_HEIGHT);
                                    if (game_end(player))
                                        continue;
                                    move_type = PUT_MARBLE;
                                }

                                if (player == WHITE)
                                    player = BLACK;
                                else if (player == BLACK)
                                    player = WHITE;

                                /* we check for a winner again, because after
                                 * the rotation the opponent player could have
                                 * won */
                                if (game_end(player))
                                    continue;
                            }
                            dir = DIR_NOT_SET;
                        }
                    }
                    show_move_info(player, move_type);
                }
                break;
        }
    }

    free_surfaces();
    stop_SDL();
    printf("Quitting... Bye.\n");
    
    return 0;
}
