/* Graphics area example */
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>
#include <stdlib.h>
#include <exec/memory.h>
#include <stdio.h>

struct IntuitionBase *IntuitionBase = NULL;
struct GfxBase *GfxBase = NULL;
//struct GadToolsBase *GadToolsBase = NULL;


int main(void) {
     struct Window *myWindow;
     struct RastPort *rp;
     int closewin = FALSE ,i;
     struct IntuiMessage *msg;
     ULONG msgClass;
     struct AreaInfo ainfo = {0};
     WORD areabuf[200];
     struct TmpRas Tmp;
     APTR tmpbuf;
	 struct GadToolsBase *GadToolsBase = NULL;
	 
    GfxBase = (struct GfxBase *)OpenLibrary((UBYTE *)"graphics.library", 0);
    if (!GfxBase) {
        // Handle error
        return RETURN_FAIL;
    }
	IntuitionBase = (struct IntuitionBase *)OpenLibrary((UBYTE *)"intuition.library", 0);
    if (!IntuitionBase) {
        // Handle error
        return RETURN_FAIL;
    }
	GadToolsBase = (struct GadToolsBase *)OpenLibrary((UBYTE *)"gadtools.library", 0);
    if (!GadToolsBase) {
        // Handle error
        return RETURN_FAIL;
    }
	    
     /* Clear area buffer */ 
     for (i=0; i<200; i++)
        areabuf[i] = 0;
   
     myWindow = OpenWindowTags(NULL,
        WA_Left, 20, WA_Top, 20,
        WA_Width, 200, WA_Height, 150,
        WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW,
        WA_Flags, WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET | WFLG_ACTIVATE | WFLG_SMART_REFRESH,
        WA_Title, (ULONG)"My Window",
        WA_PubScreenName, (ULONG)"Workbench",
        TAG_DONE);
     /* Get Window's Rastport */
     rp = myWindow->RPort;
     /* Set AreaInfo pointer in RastPort */
     rp->AreaInfo = &ainfo;
     /* Allocate temp area for Raster to work in */ 
     if (!(tmpbuf = (APTR)AllocMem(200 * 150 * 8, MEMF_CHIP | MEMF_CLEAR)))
          return(5); /* return with warning if unable to alloc memory */
	 //printf("tmpbuf: %i\n",&tmpbuf);
     //InitTmpRas(&Tmp, tmpbuf, 200 * 150 * 8);
     rp->TmpRas = &Tmp;
   
     /* Set foreground colour */
     InitArea(&ainfo, areabuf, 200 * 2 / 5); 
     SetAPen(rp, 3);
     SetOutlinePen(rp, 3);

     /* Draw a corrider */
     AreaMove(rp, 90, 12);
     AreaDraw(rp, 90, 40);
     AreaDraw(rp, 30, 135);
     AreaDraw(rp, 30, 75);
     AreaEnd(rp);
   
     SetAPen(rp, 6);
     SetOutlinePen(rp, 6);
     AreaMove(rp, 4,  75);
     AreaDraw(rp, 30, 75);
     AreaDraw(rp, 30, 135);
     AreaDraw(rp, 4,  135);
     AreaEnd(rp);
   
     SetAPen(rp, 3);
     SetOutlinePen(rp, 3);
     AreaMove(rp, 110, 12);
     AreaDraw(rp, 170, 75);
     AreaDraw(rp, 170, 135);
     AreaDraw(rp, 110, 40);
     AreaEnd(rp);
   
     SetAPen(rp, 6);
     SetOutlinePen(rp, 6);
     AreaMove(rp, 170, 75);
     AreaDraw(rp, 195, 75);
     AreaDraw(rp, 195, 135);
     AreaDraw(rp, 170, 135);
     AreaEnd(rp);
   
     /* Finish off with some lines */
     SetAPen(rp, 1);
     Move(rp, 90, 40);
     Draw(rp, 110, 40);
     Move(rp, 30, 135);
     Draw(rp, 170, 135);
   
     while (closewin == FALSE) {
        Wait(1L << myWindow->UserPort->mp_SigBit);
        msg = GT_GetIMsg(myWindow->UserPort);
        msgClass = msg->Class;
        GT_ReplyIMsg(msg);
        if (msgClass == IDCMP_CLOSEWINDOW) {
           CloseWindow(myWindow);
           closewin = TRUE;
        }
     }
     /* Clean up */
     if (tmpbuf) FreeMem(tmpbuf, 200*150*8);
	 if (GfxBase) CloseLibrary((struct Library *)GfxBase);
	 if (IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
	 if (GadToolsBase) CloseLibrary((struct Library *)GadToolsBase);
	 
     return(0);
 }