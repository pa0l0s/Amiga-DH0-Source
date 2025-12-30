#include <SDL.h>
SDL_Surface * ekran = NULL;
int main( int argc, char * args[] )
{
    SDL_Init( SDL_INIT_EVERYTHING );
    ekran = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
    SDL_Flip( ekran );
    SDL_Delay( 2000 );
    SDL_Quit();
    return 0;
}