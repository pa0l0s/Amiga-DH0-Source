#include <intuition/intuition.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <graphics/gfx.h>
#include <libraries/gadtools.h>
#include <proto/exec.h>
#include <proto/intuition.h>

struct IntuitionBase *IntuitionBase;

int main() {
    struct Window *win;

    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    if (!IntuitionBase) {
        return 1;
    }

    win = OpenWindowTags(NULL,
        WA_Title,        (ULONG)"Hello from StormC",
        WA_Left,         100,
        WA_Top,          50,
        WA_Width,        300,
        WA_Height,       100,
        WA_Flags,        WFLG_CLOSEGADGET | WFLG_DEPTHGADGET | WFLG_DRAGBAR | WFLG_ACTIVATE,
        WA_IDCMP,        IDCMP_CLOSEWINDOW,
        TAG_DONE);

    if (win) {
        BOOL running = TRUE;
        while (running) {
            struct IntuiMessage *msg;
            WaitPort(win->UserPort);
            msg = (struct IntuiMessage *)GetMsg(win->UserPort);
            if (msg) {
                if (msg->Class == IDCMP_CLOSEWINDOW) {
                    running = FALSE;
                }
                ReplyMsg((struct Message *)msg);
            }
        }
        CloseWindow(win);
    }

    CloseLibrary((struct Library *)IntuitionBase);
    return 0;
}
