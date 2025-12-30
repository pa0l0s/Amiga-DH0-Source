extern "C" {
    #include <exec/types.h>
    #include <exec/memory.h>
    #include <intuition/intuition.h>
    #include <graphics/gfx.h>
    #include <proto/exec.h>
    #include <proto/intuition.h>
    #include <proto/graphics.h>
	#include <dos/dos.h>
	#include <proto/dos.h>
	#include <math.h>
}

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

struct CubeApp {
    struct Window* window;
    struct RastPort* rp;
};

struct DosLibrary *DOSBase;

// We'll define 8 cube vertices in 3D space, centered around (0,0,0):
struct Vec3 {
    float x, y, z;
};

struct Edge {
    int from, to;
};

Vec3 cubeVertices[8] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, // back face
    {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}  // front face
};

Edge cubeEdges[12] = {
    {0,1}, {1,2}, {2,3}, {3,0}, // back
    {4,5}, {5,6}, {6,7}, {7,4}, // front
    {0,4}, {1,5}, {2,6}, {3,7}  // connectors
};

BOOL InitCubeApp(struct CubeApp* app) {
    IntuitionBase = (struct IntuitionBase*)OpenLibrary((UBYTE *)"intuition.library", 0);
    GfxBase = (struct GfxBase*)OpenLibrary((UBYTE *)"graphics.library", 0);
	DOSBase = (struct DosLibrary *)OpenLibrary((UBYTE *)"dos.library", 0);
	
    if (!IntuitionBase || !GfxBase || !DOSBase) return FALSE;

    app->window = OpenWindowTags(NULL,
        WA_Title,        (ULONG)"Rotating Cube",
        WA_Left,         100,
        WA_Top,          50,
        WA_Width,        320,
        WA_Height,       240,
        WA_Flags,        WFLG_CLOSEGADGET | WFLG_DEPTHGADGET | WFLG_DRAGBAR | WFLG_ACTIVATE,
		//WA_BackFill, 	 FALSE,
        WA_IDCMP,        IDCMP_CLOSEWINDOW,
        TAG_DONE);

    if (!app->window) return FALSE;

    app->rp = app->window->RPort;
    return TRUE;
}

//Not used
void DrawCubePlaceholder(struct CubeApp* app) {
    if (!app->window) return;

    SetAPen(app->rp, 1); // Change this to 2 or 3 if 1 is not visible

    Move(app->rp, 60, 60);
    Draw(app->rp, 160, 60);
    Draw(app->rp, 160, 160);
    Draw(app->rp, 60, 160);
    Draw(app->rp, 60, 60);
}

void ProjectAndDraw(struct CubeApp* app, float angle) {
    int centerX = 160;
    int centerY = 120;
    float scale = 60.0f;
    float distance = 3.0f;

    Vec3 projected[8];

    for (int i = 0; i < 8; i++) {
        Vec3 v = cubeVertices[i];

        // Rotate around Y and X
        float x = v.x * cos(angle) - v.z * sin(angle);
        float z = v.z * cos(angle) + v.x * sin(angle);
        float y = v.y * cos(angle * 0.5f) - z * sin(angle * 0.5f);
        z = z * cos(angle * 0.5f) + v.y * sin(angle * 0.5f);

        // Perspective projection
        float factor = scale / (z + distance);
        projected[i].x = centerX + x * factor;
        projected[i].y = centerY + y * factor;
    }

    // Clear previous frame (fill black)
    //SetAPen(app->rp, 0);
    //RectFill(app->rp, 0, 0, 319, 239);
	SHORT left   = app->window->BorderLeft;
	SHORT top    = app->window->BorderTop;
	SHORT right  = app->window->Width  - app->window->BorderRight  - 1;
	SHORT bottom = app->window->Height - app->window->BorderBottom - 1;

	SetAPen(app->rp, 0);
	RectFill(app->rp, left, top, right, bottom);

    // Draw cube edges
    SetAPen(app->rp, 1);
    for (int j = 0; j < 12; ++j) {
        int a = cubeEdges[j].from;
        int b = cubeEdges[j].to;

        Move(app->rp, (int)projected[a].x, (int)projected[a].y);
        Draw(app->rp, (int)projected[b].x, (int)projected[b].y);
    }
}

void RunCubeApp(struct CubeApp* app) {
    BOOL running = TRUE;

	float angle = 0.0f;

	while (running) {
		struct IntuiMessage* msg;

		ProjectAndDraw(app, angle);
		angle += 0.05f;

		for (int i = 0; i < 2; ++i) WaitTOF();

		while ((msg = (struct IntuiMessage*)GetMsg(app->window->UserPort))) {
			if (msg->Class == IDCMP_CLOSEWINDOW) running = FALSE;
			ReplyMsg((struct Message*)msg);
		}
	}
}


void CleanupCubeApp(struct CubeApp* app) {
    if (app->window) CloseWindow(app->window);
    if (GfxBase) CloseLibrary((struct Library*)GfxBase);
    if (IntuitionBase) CloseLibrary((struct Library*)IntuitionBase);
	if (DOSBase) CloseLibrary((struct Library *)DOSBase);
}

int main() {
    struct CubeApp app;
    if (InitCubeApp(&app)) {
        RunCubeApp(&app);
    }
    CleanupCubeApp(&app);
    return 0;
}
