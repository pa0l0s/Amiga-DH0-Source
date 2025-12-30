#include <SDL.h>
SDL_Surface * ekran = NULL;
SDL_Surface * przycisk = NULL;
SDL_Rect przyciskDane;
SDL_Rect przyciskCel;
SDL_Event zdarzenie;
int myszkaX, myszkaY;
bool wyjscie = false;

int aa = 0;

bool dupa = false;
SDL_Rect rdupa;

	

int pokarz_dupe()
{
	if (dupa) {
			SDL_FillRect(ekran , &rdupa , SDL_MapRGB(ekran->format, 255, 0, 0));
			dupa = false;
			//SDL_Flip( ekran );
	} 
	else {
			SDL_FillRect(ekran , &rdupa , SDL_MapRGB(ekran->format, 0, 255, 0));
			dupa = true;
			//SDL_Flip( ekran );
	}
	
	return 0;	
}

int main( int argc, char * args[] )
{
	rdupa.x = 100;
	rdupa.y = 100;
	rdupa.w = 100;
	rdupa.h = 100;

	
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
    ekran = SDL_SetVideoMode( 640, 512, 8, SDL_FULLSCREEN );
	if (ekran == NULL) {
			fprintf(stderr,
			"Error SDL_SetVideoMode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
    przycisk = SDL_LoadBMP( "przycisk.bmp" );
	if (przycisk == NULL) {
		fprintf(stderr,	"Error SDL_LoadBMP(przycisk.bmp) : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
    przyciskDane.w = 200;
    przyciskDane.h = 50;
    przyciskCel.x = 300;
    przyciskCel.y = 275;
	
	//fprintf(stderr,	"przed while: %s\n", SDL_GetError());
	//SDL_Quit();
	//exit(EXIT_FAILURE);
	
	
	aa=0;
	
    while( !wyjscie && aa<100)
    {

        SDL_FillRect( ekran, NULL, 0 );
        SDL_BlitSurface( przycisk, & przyciskDane, ekran, & przyciskCel );
		
		pokarz_dupe();
        SDL_Flip( ekran );
        while( SDL_PollEvent( & zdarzenie ) )
        {
			printf("Zdarzenie");
			
            if( zdarzenie.type == SDL_MOUSEMOTION )
            {
                myszkaX = zdarzenie.motion.x;
                myszkaY = zdarzenie.motion.y;
				
				printf("Mouse moved to (%d,%d)\n",
                myszkaX, myszkaY);
            }
            if( zdarzenie.type == SDL_MOUSEBUTTONDOWN )
            {
				printf("myszka guzik");
				
                if( zdarzenie.button.button == SDL_BUTTON_LEFT &&
                ( myszkaX >= przyciskCel.x && myszkaX <=
                przyciskCel.x + przyciskDane.w ) &&( myszkaY >=
                przyciskCel.y && myszkaY <= przyciskCel.y +
                przyciskDane.h ) )
                {
                    wyjscie = true;
                }
            }
            if( zdarzenie.key.keysym.sym == SDLK_ESCAPE ) wyjscie =
                 true;
           
        }
		
		aa++;
		
		
    }
	
	printf(" aa (%d)\n",aa);
	
    SDL_FreeSurface( przycisk );
	SDL_Quit();
    return 0;
}