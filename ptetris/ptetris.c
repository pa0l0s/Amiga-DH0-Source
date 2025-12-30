#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <clib/alib_protos.h>
#include <pragmas/graphics_pragmas.h> 
#include <pragmas/exec_pragmas.h> 
#include <intuition/screens.h>
#include <graphics/gfxbase.h> 
#include <stdio.h>
#include <string.h>
#include <intuition/intuition.h>
#include <exec/memory.h>
#include <devices/gameport.h>
#include <devices/inputevent.h>
#include <devices/audio.h>
/*#include <pragmas/graphics_pragmas.h>*/
#include <stdio.h>
/*#include <intuition/intuition.h>*/
#include <exec/memory.h>
#include <devices/gameport.h>
#include <devices/inputevent.h>
#include <devices/audio.h>
/*#include <pragmas/graphics_pragmas.h> */
#include <stdio.h>
/*#include <hardware/custom.h>*/
#include <hardware/dmabits.h>
/*#include <graphics/gfxmacros.h>*/
#include <graphics/sprite.h>
#include <graphics/gels.h>
#include <math.h>

#include <hardware/custom.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>


#include "ptetris.h"


/*Ustawienia ekranu*/
#define SCREEN_WHIDTH 640;
#define SCREEN_HEIGHT 200;
#define PAOLONR 640;
#define ORG -50

extern struct Custom custom;

struct IntuitionBase *intuitionBase;
struct GfxBase       *gfxBase;
struct Screen        *screen;
struct Window        *win;
struct ViewPort      *viewport;
struct IntuiMessage  *mess;
struct Message       *msg;
struct IntuiMessage  *intuiMsg;

struct TextAttr Font=
{ (STRPTR)"topaz.font",TOPAZ_EIGHTY,FS_NORMAL,FPF_ROMFONT };

struct NewScreen screen1=
{ 0,0,640,250,3,0,1,HIRES|SPRITES,CUSTOMSCREEN|SCREENQUIET,&Font,NULL,NULL,NULL };

struct NewWindow newindow=
{  0,0,640,250,0,1,
RAWKEY|GADGETUP, ACTIVATE|BORDERLESS|SIMPLE_REFRESH|RMBTRAP,
 NULL,NULL, NULL,NULL,NULL,
640,250,640,250,
CUSTOMSCREEN
};

int Score = 0;
int Level = 1;

double face1[5][2] = {
			    { 250, 25 },
			    { 350, 25 },
			    { 350, 125 },
			    { 250, 125 },
			    { 250, 25 }
		           };
 
double face2[5][2] = {
			    { 250+ORG, 25-ORG },
			    { 350+ORG, 25-ORG },
			    { 350+ORG, 125-ORG },
			    { 250+ORG, 125-ORG },
			    { 250+ORG, 25-ORG }
		           };
 
double angle = 5.0 * M_PI / 180;
double midx1, midy1, midx2, midy2;

void setcolor(int n){
	   SetAPen(win->RPort,n);
}

void line(int startx,int starty,int endx,int endy){
	Move(win->RPort, startx, starty);
	Draw(win->RPort, endx, endy);
}
 
BOOL lmbPressed(){
	
		return((*ciaa_pra & PRA_FIR0_BIT) == 0);	// quit when lmb pressed	
}

void waitmouse(void)
{
    volatile UBYTE *ciaa_pra = (volatile UBYTE *) 0xbfe001;
    while ((*ciaa_pra & PRA_FIR0_BIT) != 0) ;
}

/*Wypisuje graficznie numer na ekranie*/
/*n - numer */
/*b - maksymalna wartosc - okresla ilosc zer z przodu */
/*x - x coordinate in pixels */
/*y - y coordinate in pixels */
void Number(int n, int b, int x, int y)
{  int t, d;

   SetAPen(win->RPort,2);
   SetDrMd(win->RPort,0xC0);
   for(t=b;t>=1;t/=10)
   {  d=n/t; n-=(d*t);
         DrawImage(win->RPort,&Num[d],x,y);
      x+=16;
   }
   SetAPen(win->RPort,0);
}

void CloseUp()
{
   if(win)           CloseWindow(win);
   if(screen)        CloseScreen(screen);
}

void init()
{
	int t;

   if(!(screen=(struct Screen *)OpenScreen(&screen1)))
    CloseUp();
   newindow.Screen=screen;
   if(!(win=(struct Window *)OpenWindow(&newindow)))
     CloseUp();

/*ustawienie palety kolorow*/
   viewport=&(screen->ViewPort);
   for(t=0;t<=27;t++)
      SetRGB4(viewport,t,R[t],G[t],B[t]);
}

void clearScreen(){
	   /*ustawia na kolor tla*/
   SetAPen(win->RPort,0);
   /*wymazuje ekran tłem*/
   RectFill(win->RPort,0,0,640,250);
}

int kbhit(){
	int result;
	result = FALSE;
		while (intuiMsg = (struct IntuiMessage *)GetMsg(win->UserPort))
		{
		/*if (intuiMsg->Class == CLOSEWINDOW)
			{
			ReplyMsg((struct Message *)intuiMsg);
			return;
			}*/
		ReplyMsg((struct Message *)intuiMsg);
		result = TRUE;
		}
		intuiMsg = NULL;
		return(result);
	}


void cleardevice(){
	clearScreen();
}

void rotate (void)
{
    int i;
    for (i=0; i<5; i++)
    {
	double xnew, ynew;
 
	xnew = midx1 + (face1[i][0] - midx1) * cos (angle) -
	       (face1[i][1] - midy1) * sin (angle);
	ynew = midy1 + (face1[i][0] - midx1) * sin (angle) +
		(face1[i][1] - midy1) * cos (angle);
 
	face1[i][0] = xnew;
	face1[i][1] = ynew;
 
	xnew = midx2 + (face2[i][0] - midx2) * cos (angle) -
		(face2[i][1] - midy2) * sin (angle);
	ynew = midy2 + (face2[i][0] - midx2) * sin (angle) +
		(face2[i][1] - midy2) * cos (angle);
 
	face2[i][0] = xnew;
	face2[i][1] = ynew;
    }
 
    cleardevice();
 
    for (i=0; i<4; i++)
    {
	setcolor(2);
	line (face1[i][0], face1[i][1], face1[i+1][0], face1[i+1][1]);
	setcolor(3);
	line (face2[i][0], face2[i][1], face2[i+1][0], face2[i+1][1]);
	setcolor(4);
	line (face1[i][0], face1[i][1], face2[i][0], face2[i][1]);
    }
 
    Delay (1);
}


void newGame()
{
	  /*rysuje szary kwadrat z graphics.library*/
     RectFill(win->RPort,166,0,477,179);	
	 Number(Score,10000,56,1);
    Number(Level,10,56,11);
}

void czekajNaInput(){
	   /*czeka na klawisz*/
   while((mess=(struct IntuiMessage *)GetMsg(win->UserPort))==NULL);
   ReplyMsg((struct Message *)mess);
   RangeRand(10);
}

void Print()
{  int x,t;

   /* MoveSprite(NULL,&sprite1,350,200);
    MoveSprite(NULL,&sprite2,350,200);
    MoveSprite(NULL,&sprite3,350,200);
    MoveSprite(NULL,&sprite4,350,200);
    MoveSprite(NULL,&sprite5,350,200);*/
   
   /*ustawia kolor*/
   SetAPen(win->RPort,5);
   
   /*wypisuje tekst*/
   for(t=0;t<=21;t++)
   {  x=320-(strlen(Intro[t])*4);
      Move(win->RPort,x,t*9+10);
      Text(win->RPort,Intro[t],strlen(Intro[t]));
   }

	/*czekajNaInput();*/
	Delay(200);
   
clearScreen();

   /*AddGList(Win,&Quit,0,2,NULL);*/
   /*RefreshGList(&Quit,Win,NULL,2);*/
}

void cube(){
	    /*int gd = DETECT, gm;*/
 
    midx1 = (face1[0][0] + face1[1][0]) / 2.0;
    midy1 = (face1[1][1] + face1[2][1]) / 2.0;
    midx2 = (face2[0][0] + face2[1][0]) / 2.0;
    midy2 = (face2[1][1] + face2[2][1]) / 2.0;
 
   /* initgraph (&gd, &gm, "..\\bgi");*/
 
    while (!kbhit())
	rotate();
}

int main() {
	int a;

	
	init();
	Print();
	newGame();
	

	
	for(a=0;a<=16;a++)
	{
		Number(a, 10, 1+(a*40), 30+a);
	}

	/*opoznienie*/
    Delay(200); /*bo zlapie klawisz wcisniety przed chwila*/
	
	cube();
	
	CloseUp();
	 
	return(0);
} 