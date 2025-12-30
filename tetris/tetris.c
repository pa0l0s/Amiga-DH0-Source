#include <intuition/intuition.h>
#include <exec/memory.h>
#include <devices/gameport.h>
#include <devices/inputevent.h>
#include <devices/audio.h>
#include <pragmas/graphics_pragmas.h> 
#include <stdio.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <graphics/gfxmacros.h>
#include <graphics/sprite.h>
#include <graphics/gfxbase.h> 
#include <mffp.h>
//#include <proto/all.h> 
//#include "tetris.h"
   
extern struct Custom far custom;

BOOL UnitOpened=FALSE,DeviceOpened=FALSE;

int X=155,Y=-36,LastX=0,LastY=0,JHorz=0,ButDown=0,Rock=0,NRock=4,
    Down=0,move=0,Angle=0,Score=0,Boxb[350],Play=-1,Gone=0,Lit=0,
    Clock=0,Level=1,LevDel=0,Keys=0,
    
    BBs   =30000,
    lands =3860,
    lines =3044,
    taps  =5160,
    rights=10690,
    boings=4180;
                 
struct IntuitionBase *IntuitionBase;
struct GfxBase       *GfxBase;
struct Screen        *screen;
struct Window        *Win;
struct SimpleSprite   sprite1,sprite2,sprite3,sprite4,sprite5;
struct ViewPort      *viewport;
struct InputEvent     GPEvent;
struct IOStdReq      *GIOMsg=NULL;
struct MsgPort       *GMsgP=NULL,
                     *port,*port1,*port2,*port3;
struct IntuiMessage  *mess;
struct IOAudio       *AIOptr,*AIOptr1,*AIOptr2,*AIOptr3;
struct Message       *msg;

ULONG device,device1,device2,device3;
UBYTE chan1[]={1,2,4,8};
   
char *Intro[]=
{  "TETRIS by BLUE-BLOOD SOFTWARE","",
   "Use joystick or arrow keys to control rock:",
   "UP-DOWN......for twisting the chunk around.",
   "LEFT-RIGHT...for moving the chunk sideways.",
   "Hold fire button for precise movements",
   "Complete five lines for a new level.","",
   "This version of Tetris replaces the old unplayable version.  How do",
   "know if you have an old version?  The new version opens and says",
   "Blue-Blood and the old version cheers All-Right.  If you have the old",
   "version then throw it away because you now have a better one. New",
   "features include keyboard controls, fast rock dropping ability, and",
   "game speed changes. :-) Space bar starts new game!!!","",
   "Send comments or contributions to:","",
   "Blue-Blood","574 County Road 69","Bovey, MN 55709","",
   "REMEMBER: THIS PROGRAM IS FREEWARE!!!"   };
     
   
PLANEPTR BMap[3],BoxB[350];

struct TextAttr Font=
{ (STRPTR)"topaz.font",TOPAZ_EIGHTY,FS_NORMAL,FPF_ROMFONT };

struct IntuiText 
QuitT ={ 5,0,JAM2,16,7,&Font,(UBYTE *)"QUIT",NULL },   
NextT ={ 5,0,JAM2,54,55,&Font,(UBYTE *)"NEXT",NULL },
ReseT ={ 5,0,JAM2,12,7,&Font,(UBYTE *)"RESET",NULL },
ScorT ={ 5,0,JAM2,0,2,&Font,(UBYTE *)"SCORE:",NULL },
LeveT ={ 5,0,JAM2,0,12,&Font,(UBYTE *)"LEVEL:",NULL };   
   
struct Gadget Reset=
{  NULL,520,80,64,20,
   GADGHBOX,RELVERIFY,BOOLGADGET,
   NULL,NULL,&ReseT,NULL,NULL,2,NULL },
               
Quit=
{  &Reset,520,50,64,20,
   GADGHBOX,RELVERIFY,BOOLGADGET,
   NULL,NULL,&QuitT,NULL,NULL,1,NULL };
   
struct NewScreen newscreen=
{
   0,0,640,200,3,0,1,
   HIRES|SPRITES,CUSTOMSCREEN|SCREENQUIET,&Font,
   NULL,NULL,NULL
};

struct NewWindow newindow=
{
   0,0,640,200,0,1,
   RAWKEY|GADGETUP, ACTIVATE|BORDERLESS|SIMPLE_REFRESH|RMBTRAP,
   NULL,NULL, NULL,NULL,NULL,
   640,200,640,200,
   CUSTOMSCREEN
};

int PosX[4][28]=
{  0,12,12,24, 0,12,12,24, 0,0,12,12, 0,0,0,0,    
   0,0,12,24,  0,0,12,24,  0,12,12,24,
   0,0,12,12,  0,0,12,12,  0,0,12,12, 0,12,24,36,
   0,0,0,12,   0,12,12,12, 0,0,0,12,
   0,12,12,24, 0,12,12,24, 0,0,12,12, 0,0,0,0,    
   0,12,24,24, 0,12,24,24, 0,12,12,24,
   0,0,12,12,  0,0,12,12,  0,0,12,12, 0,12,24,36, 
   0,12,12,12, 0,0,0,12,   0,12,12,12  },

PosY[4][28]=
{  12,12,24,24, 24,12,24,12, 24,12,24,12, 0,12,24,36,  12,24,24,24, 
   12,24,12,12, 24,12,24,24,
   12,24,12,0,  0,12,12,24,  24,12,24,12, 12,12,12,12, 0,12,24,0,
   0,0,12,24,   0,12,24,12,
   12,12,24,24, 24,12,24,12, 24,12,24,12, 0,12,24,36,  12,12,12,24,
   24,24,24,12, 12,12,24,12,
   12,24,12,0,  0,12,12,24,  24,12,24,12, 24,24,24,24, 24,0,12,24, 
   0,12,24,24,  12,0,12,24 },
      
R[]={ 0,0,0,0,4,9,5,3,9,9,9,9,9,9,9,9,9,9,7,6,9,0,0,0,0,0,0,0 },
G[]={ 0,9,5,7,4,0,5,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
B[]={ 0,0,0,0,4,0,5,3,0,0,0,0,0,0,0,0,0,9,7,6,0,9,5,7,0,9,5,7 },

Lev[11][20]=
{  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,14,12,14,6,14,6,13,6,12,0,0,0,0,0,0,0,0,0,0,
   0,12,0,13,12,12,12,13,0,0,0,0,0,0,0,0,0,0,0,0,
   5,13,6,13,7,13,6,12,6,14,0,0,0,0,0,0,0,0,0,0,
   0,12,1,12,12,12,11,12,0,0,0,0,0,0,0,0,0,0,0,0,
   0,14,12,14,3,12,4,12,8,12,9,12,4,11,8,11,0,0,0,0,
   1,12,1,13,1,14,3,14,5,14,7,14,9,14,11,12,11,13,11,14,
   1,12,1,13,2,13,3,11,11,12,11,13,10,13,9,11,0,0,0,0,
   0,10,3,12,5,14,6,14,7,14,6,13,9,12,12,10,0,0,0,0,
   0,11,1,11,2,11,3,10,9,10,10,11,11,11,12,11,0,0,0,0,
   1,13,2,12,3,11,6,14,6,12,6,10,9,11,10,12,11,13,0,0
};
   
struct GamePortTrigger Fire=
{ GPTF_UPKEYS+GPTF_DOWNKEYS,1,1,1 };

struct Star
{  int   X,Y,Dist;
   float Dir;       };

struct Star Stars[35];
   
struct Line
{
   int Rand,Row,Frame,Delay;
   struct Line *Next; 
};
              
struct Line Head,Tail;
              
USHORT chip CubeData[]=
{  
   0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7E00,0x7F00,
   0x7FFF,0x7F00,  0x7FFF,0x7F00,  0x7FFF,0x7F00,  0x7FFF,0x7F00,
   0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x0000,0x0100,
   
   0x8000,0x0000,  0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFDFF,0xFE00,
   0xFDFF,0xFE00,  0xFDFF,0xFE00,  0xFDFF,0xFE00,  0xFDFF,0xFE00,
   0xFD00,0x7E00,  0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00
},

BCubeData[]=
{  
   0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,
   0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,
   0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x7FFF,0xFF00,  0x0000,0x0100,
   
   0x8000,0x0000,  0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00,
   0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00,
   0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00,  0xFFFF,0xFE00
},

BlankData[]=
{
   0x8000,0x0000,  0x8000,0x0000,  0x8000,0x0000,  0x8000,0x0000,
   0x8000,0x0000,  0x8000,0x0000,  0x8000,0x0000,  0x8000,0x0000,
   0x8000,0x0000,  0x8000,0x0000,  0x8000,0x0000,  0xFFFF,0xFF00,
   
   0xFFFF,0xFF00,  0x8000,0x0100,  0x8000,0x0100,  0x8000,0x0100,
   0x8000,0x0100,  0x8000,0x0100,  0x8000,0x0100,  0x8000,0x0100,
   0x8000,0x0100,  0x8000,0x0100,  0x8000,0x0100,  0xFFFF,0xFF00,
   
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00
},

SkullData[]=
{
   0x8000,0x0000,  0x8000,0x0000,  0x883E,0x0800,  0x8200,0x8400,
   0x8008,0x2000,  0x8008,0x8000,  0x8001,0x0000,  0x8000,0x4000,
   0x8084,0x0400,  0x8804,0x0800,  0x8000,0x0000,  0xFFFF,0xFF00,
   
   0xFFFF,0xFF00,  0x8000,0x0100,  0x987E,0x1900,  0xA200,0xC500,
   0x843E,0x2100,  0x811C,0x8100,  0x8081,0x0100,  0x8200,0x4100,
   0xA0A5,0x0500,  0x981C,0x1900,  0x8000,0x0100,  0xFFFF,0xFF00,
   
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,
   0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00,  0xFFFF,0xFF00
},
  
zer[]={ 0x07E0,0x1830,0x3018,0x3018,0x3018,0x3018,0x1830,0x0FE0},
one[]={ 0x1C00,0x0E00,0x0600,0x0600,0x0600,0x0600,0x0700,0x0380},
two[]={ 0x1FE0,0x3030,0x1818,0x0018,0x0070,0x03C0,0x0E00,0x3FFC},
thr[]={ 0x0FF0,0x1818,0x0030,0x01E0,0x0070,0x0018,0x1818,0x0FF0},
fou[]={ 0x1818,0x0C30,0x1860,0x3060,0x3FF8,0x0060,0x0060,0x0060},
fiv[]={ 0x07F8,0x0C00,0x1800,0x3FE0,0x0030,0x0018,0x3018,0x1FF0},
six[]={ 0x00F0,0x0380,0x0E00,0x1800,0x1FF8,0x180C,0x180C,0x0FF8},
sev[]={ 0x3FF8,0x0018,0x0060,0x00C0,0x0180,0x0300,0x0600,0x0600},
eig[]={ 0x0FF0,0x1818,0x1C38,0x0FF0,0x1C38,0x1818,0x1C38,0x0FF0},
nin[]={ 0x07F0,0x1818,0x1818,0x0FF8,0x0018,0x0018,0x0018,0x0018},
ten[]={ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};    

UWORD chip
Blood[]=
{  0,0,0xE55C,0x0000,0x9550,0x0000,0x9550,0x0000,0xE55C,0x0000,
   0x9550,0x0000,0x9550,0x0000,0xE5DC,0x0000,0x0000,0x0000, 
   0xE4A6,0x0000,0x9555,0x0000,0x9555,0x0000,0xE555,0x0000,
   0x9555,0x0000,0x9555,0x0000,0xE4A6,0x0000,0,0  },
   
SprD2[]=
{  0,0,0x7FF0,0x8000,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x7070,0xFFE0,
   0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,
   0x7F70,0xE0E0,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x0010,0xFFE0,0,0  },

SprD3[]=
{  0,0,0x7FF0,0x8000,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x7070,0xFFE0,
   0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,
   0x7F70,0xE0E0,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x0010,0xFFE0,0,0  },

SprD4[]=
{  0,0,0x7FF0,0x8000,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x7070,0xFFE0,
   0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,
   0x7F70,0xE0E0,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x0010,0xFFE0,0,0  },

SprD5[]=
{  0,0,0x7FF0,0x8000,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x7070,0xFFE0,
   0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,0x7F70,0xEFE0,
   0x7F70,0xE0E0,0x7FF0,0xFFE0,0x7FF0,0xFFE0,0x0010,0xFFE0,0,0  };

struct Image 
Cube  ={ 0,0,24,12,2,&CubeData[0],3,0,NULL  },
BCube ={ 0,0,24,12,2,&BCubeData[0],3,0,NULL },
Skull ={ 0,0,24,12,3,&SkullData[0],7,0,NULL },
Blank ={ 0,0,24,12,3,&BlankData[0],7,0,NULL },
Num[] ={ 0,0,16,8,1,&zer[0],1,0,NULL,     
         0,0,16,8,1,&one[0],1,0,NULL,     
         0,0,16,8,1,&two[0],1,0,NULL,     
         0,0,16,8,1,&thr[0],1,0,NULL,     
         0,0,16,8,1,&fou[0],1,0,NULL,     
         0,0,16,8,1,&fiv[0],1,0,NULL,     
         0,0,16,8,1,&six[0],1,0,NULL,     
         0,0,16,8,1,&sev[0],1,0,NULL,     
         0,0,16,8,1,&eig[0],1,0,NULL,     
         0,0,16,8,1,&nin[0],1,0,NULL }; 

BOOL SetJoyType(type)
BYTE type;
{
   BOOL success=FALSE;
   BYTE CType=0;
   
   Forbid();
   GIOMsg->io_Command=GPD_ASKCTYPE;
   GIOMsg->io_Length=1;
   GIOMsg->io_Flags=IOF_QUICK;
   GIOMsg->io_Data=(APTR)&CType;
   DoIO(GIOMsg);
   if(CType==GPCT_NOCONTROLLER)
   {
      GIOMsg->io_Command=GPD_SETCTYPE;
      GIOMsg->io_Flags=IOF_QUICK;
      GIOMsg->io_Length=1;
      GIOMsg->io_Data=(APTR)&type;
      DoIO(GIOMsg);
      success=TRUE;
      UnitOpened=TRUE;
   }
   Permit();
   return(success);
}

void SetFire(struct GamePortTrigger *gpt)
{
   GIOMsg->io_Command=GPD_SETTRIGGER;
   GIOMsg->io_Length=(LONG)sizeof(struct GamePortTrigger);
   GIOMsg->io_Data=(APTR)gpt;
   DoIO(GIOMsg);
}

void Number(n,b,x,y)
register int n,x,y;
{
   register int t,d;
   
   SetAPen(Win->RPort,2);
   SetDrMd(Win->RPort,0xC0);  
   for(t=b;t>=1;t/=10)
   {
      d=n/t; n-=(d*t);
         DrawImage(Win->RPort,&Num[d],x,y);
      x+=16;
   }
   SetAPen(Win->RPort,0);
} 

void CloseUp()
{
   BYTE type=GPCT_NOCONTROLLER;
   OFF_SPRITE;
   FreeSprite(2);
   FreeSprite(3);
   FreeSprite(4);
   FreeSprite(5);
   FreeSprite(1);
   if(UnitOpened) 
   {
      GIOMsg->io_Command=GPD_SETCTYPE;
      GIOMsg->io_Length=1;
      GIOMsg->io_Flags=IOF_QUICK;
      GIOMsg->io_Data=(APTR)&type; 
      DoIO(GIOMsg);
   } 
   if(port)          DeletePort(port);
   if(device==0)     CloseDevice((struct IORequest *)AIOptr);
   if(AIOptr)        FreeMem(AIOptr,sizeof(struct IOAudio));
   if(port1)          DeletePort(port1);
   if(device1==0)     CloseDevice((struct IORequest *)AIOptr1);
   if(AIOptr1)        FreeMem(AIOptr1,sizeof(struct IOAudio));
   if(port2)          DeletePort(port2);
   if(device2==0)     CloseDevice((struct IORequest *)AIOptr2);
   if(AIOptr2)        FreeMem(AIOptr2,sizeof(struct IOAudio));
   if(port3)          DeletePort(port3);
   if(device3==0)     CloseDevice((struct IORequest *)AIOptr3);
   if(AIOptr3)        FreeMem(AIOptr3,sizeof(struct IOAudio));
   if(DeviceOpened)  CloseDevice(GIOMsg);
   if(GIOMsg)        DeleteStdIO(GIOMsg);
   if(GMsgP)         DeletePort(GMsgP);     
   if(Win)           CloseWindow(Win);
   if(screen)        CloseScreen(screen);
   if(IntuitionBase) CloseLibrary(IntuitionBase);
   if(GfxBase)       CloseLibrary((struct Library *)GfxBase);
   exit(1);
}

void ShowNext()
{
  register int t;
  
  RectFill(Win->RPort,22,70,118,118);
  for(t=0;t<=3;t++)
   DrawImage(Win->RPort,&Cube,22+(PosX[1][NRock+t]*2),70+PosY[1][NRock+t]);
}       

void SetBox(x,y,a)
register int x,y,a;
{
   int b;
   PLANEPTR B;
      
   B=BMap[0]+(x>>3)+(y*80);
   b=128>>(x%8);
   BoxB[a]=B;
   Boxb[a]=b;
}

void InitStars()
{ 
   register int t;
   for(t=0;t<=35;t++)
   {
      Stars[t].X=320;
      Stars[t].Y=100;
      Stars[t].Dist=0;
      Stars[t].Dir=RangeRand(360);
   }
}

void Hear1(data,leng,per)
int leng,per;
BYTE *data;
{
   AIOptr->ioa_Request.io_Message.mn_ReplyPort=port;
   AIOptr->ioa_Request.io_Command             =ADCMD_FINISH;
   BeginIO((struct IORequest *)AIOptr);
   if(1L<<port->mp_SigBit) msg=GetMsg(port);
   AIOptr->ioa_Request.io_Message.mn_ReplyPort=port;
   AIOptr->ioa_Request.io_Command             =CMD_WRITE;
   AIOptr->ioa_Request.io_Flags               =ADIOF_PERVOL;
   AIOptr->ioa_Data                           =data;
   AIOptr->ioa_Length                         =leng;
   AIOptr->ioa_Period                         =per;
   AIOptr->ioa_Volume                         =64;
   AIOptr->ioa_Cycles                         =1;
   BeginIO((struct IORequest *)AIOptr);
}

void Hear2(data,leng,per)
int leng,per;
BYTE *data;
{
   AIOptr1->ioa_Request.io_Message.mn_ReplyPort=port1;
   AIOptr1->ioa_Request.io_Command             =ADCMD_FINISH;
   BeginIO((struct IORequest *)AIOptr1);
   if(1L<<port->mp_SigBit) msg=GetMsg(port);
   AIOptr1->ioa_Request.io_Message.mn_ReplyPort=port1;
   AIOptr1->ioa_Request.io_Command             =CMD_WRITE;
   AIOptr1->ioa_Request.io_Flags               =ADIOF_PERVOL;
   AIOptr1->ioa_Data                           =data;
   AIOptr1->ioa_Length                         =leng;
   AIOptr1->ioa_Period                         =per;
   AIOptr1->ioa_Volume                         =64;
   AIOptr1->ioa_Cycles                         =1;
   BeginIO((struct IORequest *)AIOptr1);
}

void Hear3(data,leng,per)
int leng,per;
BYTE *data;
{
   AIOptr2->ioa_Request.io_Message.mn_ReplyPort=port2;
   AIOptr2->ioa_Request.io_Command             =ADCMD_FINISH;
   BeginIO((struct IORequest *)AIOptr2);
   if(1L<<port->mp_SigBit) msg=GetMsg(port);
   AIOptr2->ioa_Request.io_Message.mn_ReplyPort=port2;
   AIOptr2->ioa_Request.io_Command             =CMD_WRITE;
   AIOptr2->ioa_Request.io_Flags               =ADIOF_PERVOL;
   AIOptr2->ioa_Data                           =data;
   AIOptr2->ioa_Length                         =leng;
   AIOptr2->ioa_Period                         =per;
   AIOptr2->ioa_Volume                         =24;
   AIOptr2->ioa_Cycles                         =1;
   BeginIO((struct IORequest *)AIOptr2);
}

void Hear4(data,leng,per)
int leng,per;
BYTE *data;
{
   AIOptr3->ioa_Request.io_Message.mn_ReplyPort=port3;
   AIOptr3->ioa_Request.io_Command             =ADCMD_FINISH;
   BeginIO((struct IORequest *)AIOptr3);
   if(1L<<port->mp_SigBit) msg=GetMsg(port);
   AIOptr3->ioa_Request.io_Message.mn_ReplyPort=port3;
   AIOptr3->ioa_Request.io_Command             =CMD_WRITE;
   AIOptr3->ioa_Request.io_Flags               =ADIOF_PERVOL;
   AIOptr3->ioa_Data                           =data;
   AIOptr3->ioa_Length                         =leng;
   AIOptr3->ioa_Period                         =per;
   AIOptr3->ioa_Volume                         =64;
   AIOptr3->ioa_Cycles                         =1;
   BeginIO((struct IORequest *)AIOptr3);
}

void NewGame()
{
   struct Line *Temp=Head.Next;
   Down=0;
   MoveSprite(NULL,&sprite2,350,0);
   MoveSprite(NULL,&sprite3,350,0);
   MoveSprite(NULL,&sprite4,350,0);
   MoveSprite(NULL,&sprite5,350,0);
   RectFill(Win->RPort,166,0,477,179);
   while(Temp->Next)
   {
      Head.Next=Temp->Next;
      FreeMem(Temp,sizeof(struct Line));
      Temp=Head.Next;
   }
   Rock=RangeRand(7)<<2;
   NRock=RangeRand(7)<<2;
   ShowNext();
   Gone=0; Score=0; Level=1; Y=-35; X=155;
   InitStars();
   Number(Score,10000,56,1);
   Number(Level,10,56,11); 
}

void ClearStars()
{
   register int t,b;
   
   for(t=0;t<=35;t++)
   {
      b=128>>(Stars[t].X%8);
      *(BMap[0]+(Stars[t].X>>3)+(Stars[t].Y*80))^=b;
   }
}

void Init()
{
   int t,n=0;
   SHORT SprNum;
   
   InitStars();
     
   if(!(IntuitionBase=(struct IntuitionBase *)
    OpenLibrary("intuition.library",0L)))
     CloseUp();

   if(!(screen=(struct Screen *)OpenScreen(&newscreen)))
    CloseUp();
   newindow.Screen=screen; 

   if(!(Win=(struct Window *)OpenWindow(&newindow)))
    CloseUp(); 
   
   if(!(GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",0L)))
      CloseUp();
   
   if(!(GMsgP=CreatePort(0,0L)))
      CloseUp();
      
   if(!(GIOMsg=CreateStdIO(GMsgP)))
      CloseUp();
   
   if(OpenDevice("gameport.device",1,GIOMsg,0))
      CloseUp();
   else DeviceOpened=TRUE;
      
   if(!(SetJoyType(GPCT_RELJOYSTICK)))
      CloseUp();

   if(!(AIOptr=(struct IOAudio *)
      AllocMem(sizeof(struct IOAudio),MEMF_PUBLIC|MEMF_CLEAR)))
         CloseUp();     
   if(!(port=CreatePort(0,0)))
      CloseUp();                        

   if(!(AIOptr1=(struct IOAudio *)
      AllocMem(sizeof(struct IOAudio),MEMF_PUBLIC|MEMF_CLEAR)))
         CloseUp();     
   if(!(port1=CreatePort(0,0)))
      CloseUp();     

   if(!(AIOptr2=(struct IOAudio *)
      AllocMem(sizeof(struct IOAudio),MEMF_PUBLIC|MEMF_CLEAR)))
         CloseUp();     
   if(!(port2=CreatePort(0,0)))
      CloseUp();     

   if(!(AIOptr3=(struct IOAudio *)
      AllocMem(sizeof(struct IOAudio),MEMF_PUBLIC|MEMF_CLEAR)))
         CloseUp();     
   if(!(port3=CreatePort(0,0)))
      CloseUp();                 

   SetFire(&Fire);

   Head.Next=&Tail;
   Tail.Next=NULL;
   BMap[0]=screen->BitMap.Planes[0];
   BMap[1]=screen->BitMap.Planes[1];
   BMap[2]=screen->BitMap.Planes[2];
   viewport=&(screen->ViewPort);
   for(t=0;t<=27;t++)
      SetRGB4(viewport,t,R[t],G[t],B[t]);
   SprNum=GetSprite(&sprite1,1);
   SprNum=GetSprite(&sprite2,2);
   SprNum=GetSprite(&sprite3,3);
   SprNum=GetSprite(&sprite4,4);
   SprNum=GetSprite(&sprite5,5);
   sprite1.height=15;
   sprite2.height=12;
   sprite3.height=12;
   sprite4.height=12;
   sprite5.height=12;
   ChangeSprite(NULL,&sprite1,(APTR)Blood);  
   ChangeSprite(NULL,&sprite2,(APTR)SprD2); 
   ChangeSprite(NULL,&sprite3,(APTR)SprD3);
   ChangeSprite(NULL,&sprite4,(APTR)SprD4);
   ChangeSprite(NULL,&sprite5,(APTR)SprD5); 
   for(t=520;t<=584;t++)
      { SetBox(t,50,n);  SetBox(t,80,n+164);  n++; }
   for(t=51;t<=70;t++)
      { SetBox(584,t,n); SetBox(584,t+30,n+164); n++; }
   for(t=583;t>=520;t--)
      { SetBox(t,70,n);  SetBox(t,100,n+164);  n++; }   
   for(t=69;t>=51;t--)
      { SetBox(520,t,n); SetBox(520,t+30,n+164); n++; } 
   BoxB[n]=BoxB[0]; Boxb[n]=Boxb[0];  
}

void InitAud()
{
   AIOptr->ioa_Request.io_Message.mn_ReplyPort  =port;
   AIOptr->ioa_Request.io_Message.mn_Node.ln_Pri=0;
   AIOptr->ioa_Request.io_Command               =ADCMD_ALLOCATE;
   AIOptr->ioa_Request.io_Flags                 =ADIOF_NOWAIT;
   AIOptr->ioa_AllocKey                         =0;
   AIOptr->ioa_Data                             =chan1;
   AIOptr->ioa_Length                           =sizeof(chan1);
   if(device=OpenDevice
       ("audio.device",0L,(struct IORequest *)AIOptr,0L))
         CloseUp();

   AIOptr1->ioa_Request.io_Message.mn_ReplyPort  =port1;
   AIOptr1->ioa_Request.io_Message.mn_Node.ln_Pri=0;
   AIOptr1->ioa_Request.io_Command               =ADCMD_ALLOCATE;
   AIOptr1->ioa_Request.io_Flags                 =ADIOF_NOWAIT;
   AIOptr1->ioa_AllocKey                         =0;
   AIOptr1->ioa_Data                             =chan1;
   AIOptr1->ioa_Length                           =sizeof(chan1);
   if(device1=OpenDevice
       ("audio.device",0L,(struct IORequest *)AIOptr1,0L))
         CloseUp();

   AIOptr2->ioa_Request.io_Message.mn_ReplyPort  =port2;
   AIOptr2->ioa_Request.io_Message.mn_Node.ln_Pri=0;
   AIOptr2->ioa_Request.io_Command               =ADCMD_ALLOCATE;
   AIOptr2->ioa_Request.io_Flags                 =ADIOF_NOWAIT;
   AIOptr2->ioa_AllocKey                         =0;
   AIOptr2->ioa_Data                             =chan1;
   AIOptr2->ioa_Length                           =sizeof(chan1);
   if(device2=OpenDevice
       ("audio.device",0L,(struct IORequest *)AIOptr2,0L))
         CloseUp();

   AIOptr3->ioa_Request.io_Message.mn_ReplyPort  =port3;
   AIOptr3->ioa_Request.io_Message.mn_Node.ln_Pri=0;
   AIOptr3->ioa_Request.io_Command               =ADCMD_ALLOCATE;
   AIOptr3->ioa_Request.io_Flags                 =ADIOF_NOWAIT;
   AIOptr3->ioa_AllocKey                         =0;
   AIOptr3->ioa_Data                             =chan1;
   AIOptr3->ioa_Length                           =sizeof(chan1);
   if(device3=OpenDevice
       ("audio.device",0L,(struct IORequest *)AIOptr3,0L))
         CloseUp();
   Hear1(&BB,BBs,240);
   Hear2(&BB,BBs,240);
   Hear3(&BB,BBs,250);
   Hear4(&BB,BBs,240);
}
  
void Board()
{
   register int x;
   
   NewGame();
   for(x=0;x<=15;x++) 
   {
      DrawImage(Win->RPort,&Cube,142,x*12);
      DrawImage(Win->RPort,&Cube,478,x*12);
   }
   for(x=0;x<=14;x++)
      DrawImage(Win->RPort,&Cube,142+(x*24),180);
   PrintIText(Win->RPort,&NextT,0,0);
   PrintIText(Win->RPort,&ScorT,0,0);
   PrintIText(Win->RPort,&LeveT,0,0);
   ON_SPRITE;
}

void DrawRock()
{
   register int x;
   
   x=X-1;
   MoveSprite(NULL,&sprite2,x+PosX[Angle][Rock],Y+PosY[Angle][Rock]);
   MoveSprite(NULL,&sprite3,x+PosX[Angle][Rock+1],Y+PosY[Angle][Rock+1]);
   MoveSprite(NULL,&sprite4,x+PosX[Angle][Rock+2],Y+PosY[Angle][Rock+2]); 
   MoveSprite(NULL,&sprite5,x+PosX[Angle][Rock+3],Y+PosY[Angle][Rock+3]);
}

void Turn()
{
   register int a;
   
   a=Angle++; 
   if(Angle>3) Angle=0;
   if(Hit()) Angle=a;
   else Hear3(&tap,taps,500);
}
 
void JoyDir()
{
   if(!Keys)
   {
     GIOMsg->io_Command=GPD_READEVENT;
     GIOMsg->io_Length=sizeof(struct InputEvent);
     GIOMsg->io_Data=(APTR)&GPEvent;
     DoIO(GIOMsg);
     GetMsg(GMsgP);
     JHorz=GPEvent.ie_X;
     Down=GPEvent.ie_Y;
     switch(GPEvent.ie_Code)
     {
     case IECODE_LBUTTON: ButDown=1;
                          break;
     case (IECODE_LBUTTON|IECODE_UP_PREFIX):
                          ButDown=0;
                          break;
     }
   }
   if((Down<0)&&!LastY) Turn();
   if(ButDown&&!LastX) move=JHorz*6;
   if(!ButDown) move=JHorz*6;
   LastX=JHorz;
   LastY=Down;
}

int Hit()
{
   register int r=0,t,x,y,b;
   PLANEPTR B1=0;
   
   for(t=0;t<=3;t++)
   {
      y=Y+PosY[Angle][Rock+t]+12;
      if(y>0)
      {
         x=(X+PosX[Angle][Rock+t])<<1;
         b=128>>(x%8);
         x=(x>>3)+(y*80);
         *B1=*(BMap[0]+x)|*(BMap[1]+x)|*(BMap[2]+x);
         r|=(*B1&b);
      }
   }
   return(r);
}      

void Slide()
{
   register int x;
   x=X;
   if(Y<-12) move=0;
   if(move<0) X-=1; 
   if(move>0) X+=12;
   if(!Hit()) 
   {
      Hear3(&tap,taps,500);
      while(move)
      {   
         if(move<0)
           { x-=2; move++; }
         if(move>0)
           { x+=2; move--; }
         DrawRock();
      }
   }
   else move=0;
   X=x;
}

void CheckLine(int l)
{
   register int t,n=0,c;
   register struct Line *Temp;
   
   for(t=167;t<=455;t+=24)
   {
      c=ReadPixel(Win->RPort,t,l*12);
      if(c&&c<4) n++;
   }
   if(n>=13)
   {
      Temp=AllocMem(sizeof(struct Line),MEMF_PUBLIC);
      Temp->Next=Head.Next;
      Head.Next=Temp;
      Temp->Row=l;
      Temp->Frame=0;
      Temp->Delay=0;
      Temp->Rand=(RangeRand(15)+10);
   }
}      

void KillLine()
{
   register struct Line *Temp=Head.Next,*Last=&Head,*Renew;
   register int t,row;
   
   while(Temp->Next)
   {
      row=Temp->Row;
      if(Temp->Delay++>=Temp->Rand)
      {
       Temp->Delay=0;
       Temp->Frame++;
       if(Temp->Frame<30)
       {
        if(Temp->Frame&1)
         for(t=0;t<=12;t++)
          DrawImage(Win->RPort,&Blank,166+t*24,row*12);
        else 
         for(t=0;t<=12;t++)
          DrawImage(Win->RPort,&Skull,166+t*24,row*12); 
       }
       else
       {
        ScrollRaster(Win->RPort,0,-12,166,0,477,(row*12)+11);
        Last->Next=Temp->Next;
        FreeMem(Temp,sizeof(struct Line));
        Temp=Last->Next;
        Renew=Head.Next;
        Score+=(16-row)*100;
        Hear2(&lne,lines,400);
        Number(Score,10000,56,1);
        Gone++;
        while(Renew->Next)
        {
         if(Renew->Row<row) Renew->Row++;
         Renew=Renew->Next;
      }}}
      if(Temp->Next)
      {
       Last=Last->Next;  
       Temp=Temp->Next;
}  }  }

void Dead()
{
   int x,y;

   for(y=0;y<179;y+=12)
      for(x=166;x<=477;x+=24)
      {
         if(ReadPixel(Win->RPort,x,y))
            DrawImage(Win->RPort,&Skull,x,y);
      }
   X=350; Y=200; Play=-1; LastX=1; 
}
         
void Land()
{
   register int t,l;

   for(t=0;t<=3;t++)
      DrawImage(Win->RPort,&BCube,(X+PosX[Angle][Rock+t])*2,
                                  Y+PosY[Angle][Rock+t]);
   GIOMsg->io_Command=CMD_CLEAR;   
   DoIO(GIOMsg);
   Down=0; 
   Rock=NRock;
   NRock=RangeRand(7)*4;
   ShowNext();
   Hear1(&land,lands,650);
   if(Y>0)
   {
      for(t=0;t<=3;t++)
      {
         l=(Y/12)+t;
         if(l<=14) CheckLine(l);
      }
      Y=-35; X=155; Angle=1;
      Score+=Level;
      Number(Score,10000,56,1);
   }
   else Dead();
}

void Bounce()
 {
   static int BBX=50,BBY=50,BBXA=3,BBYA=0;
   
   BBX+=BBXA; BBYA+=1; BBY+=BBYA;
   if(BBX>=319||BBX<=1) { BBX-=BBXA; BBXA=-BBXA; }
   if(BBY>190) { BBYA=-BBYA; BBY+=BBYA; Hear3(&boing,boings,500); }
   MoveSprite(NULL,&sprite1,BBX,BBY);
}

void NewMsg()
{
   Keys=0;
   if(mess=(struct IntuiMessage *)GetMsg(Win->UserPort))
   {
      ReplyMsg((struct message *)mess);
      switch(mess->Class)
      {
         case GADGETUP: switch(((struct Gadget *)mess->IAddress)->GadgetID)
                        {
                           case 1: CloseUp();
                                   break;
                           case 2: ClearStars();
                                   Hear4(&tap,taps,400);
                                   NewGame();
                                   Play=-1;
                                   break;
                        }
                        break;
         case RAWKEY:   switch(mess->Code)
                        {
                           case 95: if(mess->Qualifier&IEQUALIFIER_LSHIFT)
                                    Gone=6;
                                    break;
                           case 64:          
                                    {
                                     MoveSprite(NULL,&sprite1,268,110); 
                                     ClearStars();
                                     NewGame();
                                     Hear4(&tap,taps,400);
                                     Play=1; 
                                    }
                                    break;
                           case 79: JHorz=-1;
                                    Keys=1;
                                    break;
                           case 78: JHorz= 1;
                                    Keys=1;
                                    break;
                           case 76: Down=-1;
                                    Keys=1;
                                    break;
                           case 77: Down= 1;
                                    Keys=1;
                                    break;
     
                        }
                        break;        
}  }  }
 
void Lights()
{
   register int l;
   
   for(l=Lit;l<=328;l+=8)
   {
      *BoxB[l]^=Boxb[l];       
      *BoxB[l+1]|=Boxb[l+1];
   }
   if(++Lit&8) Lit=0; 
}

void NewStar(register int T)
{
   Stars[T].X=320;
   Stars[T].Y=100;
   Stars[T].Dir=T^!Lit;
   Stars[T].Dist=0;
}

void MoveStars()
{
   register int t,b;
   
   for(t=0;t<=35;t++)
   {
      b=128>>(Stars[t].X%8);
      *(BMap[0]+(Stars[t].X>>3)+(Stars[t].Y*80))^=b;

      Stars[t].X+=(++Stars[t].Dist)*SPCos(Stars[t].Dir);
      Stars[t].Y+=Stars[t].Dist*SPSin(Stars[t].Dir);
      if(Stars[t].X>477||Stars[t].X<166||
         Stars[t].Y>179||Stars[t].Y<0) NewStar(t);
      else
      {  
         b=128>>(Stars[t].X%8);  
         *(BMap[0]+(Stars[t].X>>3)+(Stars[t].Y*80))^=b;
}  }  }

void NewLev()
{
   register int x,y;
   register struct Line *Temp=Head.Next;

   Down=0;
   switch(LevDel)
   {   
      case 5:  while(Temp->Next)
                {
                 Head.Next=Temp->Next;
                 FreeMem(Temp,sizeof(struct Line));
                 Temp=Head.Next;
                }
                MoveSprite(NULL,&sprite2,350,0);
                MoveSprite(NULL,&sprite3,350,0);
                MoveSprite(NULL,&sprite4,350,0);
                MoveSprite(NULL,&sprite5,350,0);
                break;
      case 30:  for(y=0;y<=179;y+=12)
                 for(x=166;x<=477;x+=24)
                  DrawImage(Win->RPort,&Cube,x,y);
                break;  
      case 60:  Hear1(&right,rights,440);
                RectFill(Win->RPort,166,0,477,179);       
                y=Level; if(y>10) y%=10;
                for(x=0;x<=9;x++)
                 if(Lev[y][x*2+1])
                  DrawImage(Win->RPort,&Cube,166+24*Lev[y][x*2],
                                                 12*Lev[y][x*2+1]);
                break;
      case 90:  Y=-36; X=155; Level++; Gone=0; LevDel=0;
                if(Level>99) Level=0;
                Number(Level,10,56,11);
   }
   LevDel++;                                 
}  

void Print()
{
   int x,t;

   MoveSprite(NULL,&sprite1,350,200);   
   MoveSprite(NULL,&sprite2,350,200);
   MoveSprite(NULL,&sprite3,350,200);
   MoveSprite(NULL,&sprite4,350,200);
   MoveSprite(NULL,&sprite5,350,200);
   SetAPen(Win->RPort,5);
   for(t=0;t<=21;t++)
   {
      x=320-(strlen(Intro[t])*4);
      Move(Win->RPort,x,t*9+10);
      Text(Win->RPort,Intro[t],strlen(Intro[t]));
   }
   InitAud();
   while((mess=(struct IntuiMessage *)GetMsg(Win->UserPort))==NULL);
   ReplyMsg((struct Message *)mess);
   RangeRand(10);
   SetAPen(Win->RPort,0);
   RectFill(Win->RPort,0,0,640,400);
   AddGList(Win,&Quit,0,2,NULL);
   RefreshGList(&Quit,Win,NULL,2);
}
  
void main()
{
   Init();
   Print();
   Board();
   FOREVER
   {
      if(Play>0)
      {
         if(Down<1) JoyDir();
         if(Gone>=5) NewLev();
         else
         {
            if(Hit()) Land();
            if(move) Slide();
            if(Level<12) Y=Y+Clock;
            else Y++;
            DrawRock();
            KillLine();
         }
      }
      else
      {
         WaitTOF();
         if(Clock) MoveStars();
         else Bounce();
      }
      if(!Clock) Lights();
      NewMsg();
      Clock^=1;
   }
}
