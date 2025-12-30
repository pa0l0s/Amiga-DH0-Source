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

#define WIDTH 320
#define HEIGHT 240

APTR tmpbuf;
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

// Face definition (quad)
struct Face {
    int vertices[4]; // Indices into cubeVertices
    int color;       // Base color for this face
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

// Define the 6 faces of the cube (each face is a quad)
Face cubeFaces[6] = {
    {{0, 1, 2, 3}, 1}, // back (red)
    {{4, 5, 6, 7}, 2}, // front (blue)
    {{0, 1, 5, 4}, 3}, // bottom (green)
    {{2, 3, 7, 6}, 4}, // top (yellow)
    {{0, 3, 7, 4}, 5}, // left (cyan)
    {{1, 2, 6, 5}, 6}  // right (magenta)
};

BOOL InitCubeApp(struct CubeApp* app) {
    IntuitionBase = (struct IntuitionBase*)OpenLibrary((UBYTE *)"intuition.library", 0);
    GfxBase = (struct GfxBase*)OpenLibrary((UBYTE *)"graphics.library", 0);
	DOSBase = (struct DosLibrary *)OpenLibrary((UBYTE *)"dos.library", 0);
	
    if (!IntuitionBase || !GfxBase || !DOSBase) return FALSE;

	struct AreaInfo ainfo = {0};
	WORD areabuf[200];
    struct TmpRas Tmp;

	
	/* Clear area buffer */ 
    for (int i=0; i<200; i++)
        areabuf[i] = 0;
	
    app->window = OpenWindowTags(NULL,
        WA_Title,        (ULONG)"Rotating Cube",
        WA_Left,         100,
        WA_Top,          50,
        WA_Width,        WIDTH,
        WA_Height,       HEIGHT,
        WA_Flags,        WFLG_CLOSEGADGET | WFLG_DEPTHGADGET | WFLG_DRAGBAR | WFLG_ACTIVATE,
		//WA_BackFill, 	 FALSE,
        WA_IDCMP,        IDCMP_CLOSEWINDOW,
        TAG_DONE);

    if (!app->window) return FALSE;
	
	/* Get Window's Rastport */
    app->rp = app->window->RPort;
	

    /* Set AreaInfo pointer in RastPort */
    app->rp->AreaInfo = &ainfo;
    /* Allocate temp area for Raster to work in */ 
    if (!(tmpbuf = (APTR)AllocMem(WIDTH * HEIGHT * 8, MEMF_CHIP | MEMF_CLEAR)))
        return(5); /* return with warning if unable to alloc memory */
	
	ULONG tmpRasSize = ((WIDTH + 15) / 16) * 2 * HEIGHT;
	
    InitTmpRas(&Tmp, (PLANEPTR)tmpbuf, tmpRasSize);
    app->rp->TmpRas = &Tmp;
	InitArea(&ainfo, areabuf, 200 * 2 / 5);

    return TRUE;
}

Vec3 CalculateFaceNormal(Vec3* vertices, Face* face) {
    Vec3 v0 = vertices[face->vertices[0]];
    Vec3 v1 = vertices[face->vertices[1]];
    Vec3 v2 = vertices[face->vertices[2]];
    
    Vec3 a = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
    Vec3 b = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};
    
    // Cross product
    Vec3 normal = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    
    // Normalize (approximate)
    float length = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
    if (length > 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }
    
    return normal;
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

void ClearWindow(struct CubeApp* app) {
	if (!app->window) return;
	
	// Clear previous frame (fill black)
    //SetAPen(app->rp, 0);
    //RectFill(app->rp, 0, 0, 319, 239);
	SHORT left   = app->window->BorderLeft;
	SHORT top    = app->window->BorderTop;
	SHORT right  = app->window->Width  - app->window->BorderRight  - 1;
	SHORT bottom = app->window->Height - app->window->BorderBottom - 1;

	SetAPen(app->rp, 0);
	RectFill(app->rp, left, top, right, bottom);
}

void ProjectAndDraw(struct CubeApp* app, float angle) {
    int centerX = WIDTH/2;
    int centerY = HEIGHT/2;
    float scale = 60.0f;
    float distance = 3.0f;

	Vec3 transformed[8];
    Vec3 projected[8];

    for (int i = 0; i < 8; i++) {
        Vec3 v = cubeVertices[i];

        // Rotate around Y and X
        float x = v.x * cos(angle) - v.z * sin(angle);
        float z = v.z * cos(angle) + v.x * sin(angle);
        float y = v.y * cos(angle * 0.5f) - z * sin(angle * 0.5f);
        z = z * cos(angle * 0.5f) + v.y * sin(angle * 0.5f);
		
		transformed[i].x=x;
		transformed[i].y=y;
		transformed[i].z=z;

        // Perspective projection
        float factor = scale / (z + distance);
        projected[i].x = centerX + x * factor;
        projected[i].y = centerY + y * factor;
    }

	ClearWindow(app);
	
    // Draw cube edges
    SetAPen(app->rp, 1);
    for (int j = 0; j < 12; ++j) {
        int a = cubeEdges[j].from;
        int b = cubeEdges[j].to;

        Move(app->rp, (int)projected[a].x, (int)projected[a].y);
        Draw(app->rp, (int)projected[b].x, (int)projected[b].y);
    }
	
	//RectFill(app->rp, 20, 20, 120, 120);
	
	SetAPen(app->rp, 3);
	AreaMove(app->rp, 20, 20);
	AreaDraw(app->rp, 120, 20);
	AreaDraw(app->rp, 120, 120);
	AreaDraw(app->rp, 20, 120);
	AreaDraw(app->rp, 20, 20);
	AreaEnd(app->rp);
	
	    // Draw each face with backface culling
    for (int f = 0; f < 6; f++) {
        Vec3 normal = CalculateFaceNormal(transformed, &cubeFaces[f]);
        
        // Backface culling - only draw if facing towards the viewer
        if (normal.z > 0) {
            // Calculate brightness based on angle (simple lighting)
            float brightness = (normal.z + 1.0f) * 0.5f; // 0.0 to 1.0
            
            // Set color based on face and brightness
            int color = cubeFaces[f].color;
            SetAPen(app->rp, f);
            
            // Draw the filled polygon
            WORD points[10]; // 4 points * 2 coordinates + 2 for terminator
            for (int k = 0; k < 4; k++) {
                points[k*2]   = (WORD)projected[cubeFaces[f].vertices[k]].x;
                points[k*2+1] = (WORD)projected[cubeFaces[f].vertices[k]].y;
            }
            points[8] = points[0]; // Close the polygon
            points[9] = points[1];
			
			// AreaMove(app->rp, 20, 20);
			// AreaDraw(app->rp, 120, 20);
			// AreaDraw(app->rp, 120, 120);
			// AreaDraw(app->rp, 20, 120);
			// AreaDraw(app->rp, 20, 20);
			// AreaEnd(app->rp);
            
            // Draw the filled polygon
            //AreaMove(app->rp, points[0], points[1]);
            //AreaDraw(app->rp, points[2], points[3]);
            //AreaDraw(app->rp, points[4], points[5]);
            //AreaDraw(app->rp, points[6], points[7]);
            //AreaEnd(app->rp);
            
            // Draw the outline
            //SetAPen(app->rp, 0); // Black outline
            //Move(app->rp, points[0], points[1]);
            //Draw(app->rp, points[2], points[3]);
            //Draw(app->rp, points[4], points[5]);
            //Draw(app->rp, points[6], points[7]);
            //Draw(app->rp, points[0], points[1]);
        }
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
	
	if (tmpbuf) FreeMem(tmpbuf, WIDTH * HEIGHT * 8);
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
