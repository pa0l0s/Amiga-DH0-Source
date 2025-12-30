#define GEL_SIZE 4 /* number of lines in the bob */
/* Bob data - two sets that are alternated between. Note that this */
/* data is at the resolution of the screen. */
/* data is 2 planes by 2 words by GEL_SIZE lines */
WORD __chip bob_data1[2 * 2 * GEL_SIZE] =
{
      /* plane 1 */
      0xffff, 0x0003, 0xfff0, 0x0003, 0xfff0, 0x0003, 0xffff, 0x0003,
      /* plane 2 */
      0x3fff, 0xfffc, 0x3ff0, 0x0ffc, 0x3ff0, 0x0ffc, 0x3fff, 0xfffc
};
/* data is 2 planes by 2 words by GEL_SIZE lines */
WORD __chip bob_data2[2 * 2 * GEL_SIZE] =
{
      /* plane 1 */
      0xc000, 0xffff, 0xc000, 0x0fff, 0xc000, 0x0fff, 0xc000, 0xffff,
      /* plane 2 */
      0x3fff, 0xfffc, 0x3ff0, 0x0ffc, 0x3ff0, 0x0ffc, 0x3fff, 0xfffc
};


/*tekst na poczatku*/
char *Intro[]=
{  "P TETRIS by PA0L0","",
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



/*Paleta kolorow*/
int
R[]={ 0,0,0,0,4,9,5,3,9,9,9,9,9,9,9,9,9,9,7,6,9,0,0,0,0,0,0,0 },
G[]={ 0,9,5,7,4,0,5,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
B[]={ 0,0,0,0,4,0,5,3,0,0,0,0,0,0,0,0,0,9,7,6,0,9,5,7,0,9,5,7 };

USHORT __chip zer[]={ 0x07E0,0x1830,0x3018,0x3018,0x3018,0x3018,0x1830,0x0FE0},
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


struct Image
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
		 

		 
		 
