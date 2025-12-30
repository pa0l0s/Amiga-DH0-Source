#include <stdio.h>
#include <SDL.h>

//Inicjalizacja zmiennych
SDL_Surface * ekran = NULL;
SDL_Surface * obraz = NULL;

SDL_Rect sprst[ 3 ];
SDL_Rect dprst[ 3 ];

SDL_Rect rtofill;

//glowna funkcja programu
int main( int argc, char * args[] )
{
	//inicjalizacja biblioteki SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//Tworzenie ekranu
    ekran = SDL_SetVideoMode( 640, 512, 8, SDL_FULLSCREEN );
	
	if (ekran == NULL) {
			fprintf(stderr,
			"Error SDL_SetVideoMode: %s\n", SDL_GetError());
		exit(1);
	}
	
	
	obraz = SDL_LoadBMP( "obraz.bmp" );
    //SDL_BlitSurface( obraz, NULL, ekran, NULL );
	
	
	sprst[ 0 ].x = 400;
    sprst[ 0 ].y = 0;
    sprst[ 0 ].w = 200;
    sprst[ 0 ].h = 200;
    sprst[ 1 ].x = 200;
    sprst[ 1 ].y = 0;
    sprst[ 1 ].w = 200;
    sprst[ 1 ].h = 200;
    sprst[ 2 ].x = 0;
    sprst[ 2 ].y = 0;
    sprst[ 2 ].w = 200;
    sprst[ 2 ].h = 200;
    dprst[ 0 ].x = 0;
    dprst[ 0 ].y = 0;
    dprst[ 1 ].x = 200;
    dprst[ 1 ].y = 0;
    dprst[ 2 ].x = 400;
    dprst[ 2 ].y = 0;
    SDL_BlitSurface( obraz, & sprst[ 0 ], ekran, & dprst[ 0 ] );
    SDL_BlitSurface( obraz, & sprst[ 1 ], ekran, & dprst[ 1 ] );
    SDL_BlitSurface( obraz, & sprst[ 2 ], ekran, & dprst[ 2 ] );
	
		
	//uaktualniamy ekran
    SDL_Flip( ekran );
	
	//ta funkcja zatrzymuje program na pewien czas. Gdybyś usunął tą funkcję program by się włączył i od razu wyłączył, przez co nie zobaczyłbyś okna. W argumencie funkcji podajemy czas w milisekundach. Program zatrzyma się na 2000 milisekund, czyli 2 sekundy. Oczywiście możesz wpisać tam co chcesz.
    SDL_Delay( 1000 );

    for (int i = 1;i <= 200; ++i)
    {
            ++dprst[ 0 ].x;
            ++dprst[ 0 ].x;
            ++dprst[ 1 ].y;
            --dprst[ 2 ].x;
            --dprst[ 2 ].x;

        SDL_BlitSurface( obraz, & sprst[ 0 ], ekran, & dprst[ 0 ] );
        SDL_BlitSurface( obraz, & sprst[ 1 ], ekran, & dprst[ 1 ] );
        SDL_BlitSurface( obraz, & sprst[ 2 ], ekran, & dprst[ 2 ] );

        SDL_Flip( ekran );
        //SDL_Delay( 1 );
    }
	
	rtofill.x = 100;
	rtofill.y = 300;
	rtofill.w = 100;
	rtofill.h = 100;
	
	SDL_FillRect(ekran , &rtofill , SDL_MapRGB(ekran->format, 255, 0, 0));
	
	SDL_Flip( ekran );
	SDL_Delay( 5000 );
	
	//- tą funkcją kończymy pracę biblioteki.
	//Po zakończeniu programu zwracamy wartość 0. Zero stosowane jest, aby poinformować system, że program dobrze działał i nie było błędów. No i to wszystko. Następna lekcja będzie jeszcze przyjemniejsza. Nauczysz się wyświetlać obrazki.
    SDL_Quit();
    return 0;
}