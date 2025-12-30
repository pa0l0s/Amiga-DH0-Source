/* Lines */
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>
#include <stdlib.h>

int WindowWidth = 400;
int WindowHeight = 200;

/* Random no between 0 and n */
int randomNum(int n) {
     return (rand() % n);
   }

int main() {
    struct Window *myWindow;
    struct RastPort *rp;
    int closewin = FALSE;
    struct IntuiMessage *msg;
    ULONG msgClass;

    myWindow = OpenWindowTags(NULL,
      WA_Left, 20, WA_Top, 20,
      WA_Width, WindowWidth, WA_Height, WindowHeight,
      WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW,
      WA_Flags, WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET    | WFLG_CLOSEGADGET | WFLG_ACTIVATE |
      WFLG_SMART_REFRESH,
      WA_Title, "Lines C",
      WA_PubScreenName, "Workbench",
      TAG_DONE);
    /* Get Window's Rastport */
    rp = myWindow->RPort;


    while (closewin == FALSE) {

				/*local varibles*/
		long startx,starty,endx,endy;

		/*set pen colour*/
		SetAPen(rp, (ULONG)randomNum(32)+1);

		/*randomize coordinates*/
		startx = 10 + randomNum(WindowWidth-20);
		starty = 10 + randomNum(WindowHeight-20);
		endx = 10 + randomNum(WindowWidth-20);
		endy = 10 + randomNum(WindowHeight-20);

		/*set start point*/
		Move(rp, startx, starty);

		/*draw line from start point to given coordinates*/
		Draw(rp, endx, endy);

       /*This waits for action from window. not needed
       /Wait(1L << myWindow->UserPort->mp_SigBit);*/

       msg = GT_GetIMsg(myWindow->UserPort);
       msgClass = msg->Class;
       GT_ReplyIMsg(msg);
       if (msgClass == IDCMP_CLOSEWINDOW) {
          CloseWindow(myWindow);
          closewin = TRUE;
       }
       if (msgClass == IDCMP_REFRESHWINDOW)
          RefreshWindowFrame(myWindow);
    }
  return(0);
}


