extern "C" {
    #include <exec/types.h>
    #include <exec/memory.h>
    #include <intuition/intuition.h>
    #include <graphics/gfx.h>
    #include <proto/exec.h>
    #include <proto/intuition.h>
    #include <proto/graphics.h>
}

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

struct CubeApp {
    struct Window* window;
    struct RastPort* rp;
};

BOOL InitCubeApp(struct CubeApp* app) {
    IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 0);
    GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0);
    if (!IntuitionBase || !GfxBase) return FALSE;

    app->window = OpenWindowTags(NULL,
        WA_Title,        (ULONG)"Rotating Cube",
        WA_Left,         100,
        WA_Top,          50,
        WA_Width,        320,
        WA_Height,       240,
        WA_Flags,        WFLG_CLOSEGADGET | WFLG_DEPTHGADGET | WFLG_DRAGBAR | WFLG_ACTIVATE,
        WA_IDCMP,        IDCMP_CLOSEWINDOW,
        TAG_DONE);

    if (!app->window) return FALSE;

    app->rp = app->window->RPort;
    return TRUE;
}

void DrawCubePlaceholder(struct CubeApp* app) {
    // Replace this with 3D cube logic later
    SetAPen(app->rp, 1);
    Move(app->rp, 60, 60);
    Draw(app->rp, 100, 60);
    Draw(app->rp, 100, 100);
    Draw(app->rp, 60, 100);
    Draw(app->rp, 60, 60); // square for now
}

void RunCubeApp(struct CubeApp* app) {
    BOOL running = TRUE;

    while (running) {
        struct IntuiMessage* msg;
        WaitPort(app->window->UserPort);
        while ((msg = (struct IntuiMessage*)GetMsg(app->window->UserPort))) {
            if (msg->Class == IDCMP_CLOSEWINDOW) {
                running = FALSE;
            }
            ReplyMsg((struct Message*)msg);
        }

        DrawCubePlaceholder(app);  // call your cube render here
    }
}

void CleanupCubeApp(struct CubeApp* app) {
    if (app->window) CloseWindow(app->window);
    if (GfxBase) CloseLibrary((struct Library*)GfxBase);
    if (IntuitionBase) CloseLibrary((struct Library*)IntuitionBase);
}

int main() {
    struct CubeApp app;
    if (InitCubeApp(&app)) {
        RunCubeApp(&app);
    }
    CleanupCubeApp(&app);
    return 0;
}
