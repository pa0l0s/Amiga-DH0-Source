#include <stdio.h>
#include <SDL.h>
#include "game.h"

//Inicjalizacja zmiennych
SDL_Surface * ekran = NULL;
SDL_Surface * ship_image_srf = NULL;

SDL_Rect sprst[ 3 ];
SDL_Rect dprst[ 3 ];

SDL_Rect ship_sprt[2];
SDL_Rect ship_dest;

SDL_Rect rtofill;

char * ship_image_path = "i_are_spaceship.bmp";

//glowna funkcja programu
int game( int argc, char * args[] )
{
	//inicjalizacja biblioteki SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//Tworzenie ekranu
    ekran = SDL_SetVideoMode( 320, 256, 8, SDL_FULLSCREEN );
	
	if (ekran == NULL) {
			fprintf(stderr,
			"Error SDL_SetVideoMode: %s\n", SDL_GetError());
		exit(1);
	}
	
	
	ship_image_srf = SDL_LoadBMP( ship_image_path );
    //SDL_BlitSurface( ship_image_srf, NULL, ekran, NULL );
	
    ship_sprt[ 0 ].x = 0;
    ship_sprt[ 0 ].y = 0;
    ship_sprt[ 0 ].w = 16;
    ship_sprt[ 0 ].h = 24;
    ship_sprt[ 1 ].x = 16;
    ship_sprt[ 1 ].y = 0;
    ship_sprt[ 1 ].w = 16;
    ship_sprt[ 1 ].h = 24;

    ship_dest.x = 150;
    ship_dest.y = 150;
	
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
    //SDL_BlitSurface( ship_image_srf, & sprst[ 0 ], ekran, & dprst[ 0 ] );
    //SDL_BlitSurface( ship_image_srf, & sprst[ 1 ], ekran, & dprst[ 1 ] );
    //SDL_BlitSurface( ship_image_srf, & sprst[ 2 ], ekran, & dprst[ 2 ] );
	
		
	//uaktualniamy ekran
    //SDL_Flip( ekran );
	
	//ta funkcja zatrzymuje program na pewien czas. Gdybyś usunął tą funkcję program by się włączył i od razu wyłączył, przez co nie zobaczyłbyś okna. W argumencie funkcji podajemy czas w milisekundach. Program zatrzyma się na 2000 milisekund, czyli 2 sekundy. Oczywiście możesz wpisać tam co chcesz.
    //SDL_Delay( 1000 );

    int ship_anim = 0;

    for (int i = 1;i <= 200; ++i)
    {
        //     ++dprst[ 0 ].x;
        //     ++dprst[ 0 ].x;
        //     ++dprst[ 1 ].y;
        //     --dprst[ 2 ].x;
        //     --dprst[ 2 ].x;

        // SDL_BlitSurface( ship_image_srf, & sprst[ 0 ], ekran, & dprst[ 0 ] );
        // SDL_BlitSurface( ship_image_srf, & sprst[ 1 ], ekran, & dprst[ 1 ] );
        // SDL_BlitSurface( ship_image_srf, & sprst[ 2 ], ekran, & dprst[ 2 ] );

        SDL_BlitSurface( ship_image_srf, & ship_sprt[ ship_anim ], ekran, & ship_dest );

        ++ship_anim;
        
        if(ship_anim>1)
        {
           ship_anim=0; 
        }

        SDL_Flip( ekran );
        SDL_Delay( 100 );
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