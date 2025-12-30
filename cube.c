/****************************************************************************\
*                                                                            *
*  Cube.c                                                                    *
*                                                                            *
*  This program draws a cube in 3D world space and allows the user to move   *
*  and rotate the cube through keyboard controls. Each of the six cube faces *
*  is a different color.                                                     *
*                                                                            *
\****************************************************************************/

//#include 
#define vbWidth  640
#define vbHeight 480
typedef struct point3d
{
   double x;
   double y;
   double z;
} POINT3D;
// six faces of a 40x40x40 cube, defined in object coordinates
POINT3D Face1[] = {
   { 20.0,-20.0,-20.0},
   {-20.0,-20.0,-20.0},
   {-20.0, 20.0,-20.0},
   { 20.0, 20.0,-20.0}
};
POINT3D Face2[] = {
   {-20.0,-20.0,-20.0},
   {-20.0,-20.0, 20.0},
   {-20.0, 20.0, 20.0},
   {-20.0, 20.0,-20.0}
};
POINT3D Face3[] = {
   { 20.0, 20.0, 20.0},
   {-20.0, 20.0, 20.0},
   {-20.0,-20.0, 20.0},
   { 20.0,-20.0, 20.0}
};
POINT3D Face4[] = {
   { 20.0,-20.0, 20.0},
   { 20.0,-20.0,-20.0},
   { 20.0, 20.0,-20.0},
   { 20.0, 20.0, 20.0}
};
POINT3D Face5[] = {
   { 20.0,-20.0, 20.0},
   {-20.0,-20.0, 20.0},
   {-20.0,-20.0,-20.0},
   { 20.0,-20.0,-20.0}
};
POINT3D Face6[] = {
   { 20.0, 20.0,-20.0},
   {-20.0, 20.0,-20.0},
   {-20.0, 20.0, 20.0},
   { 20.0, 20.0, 20.0}
};
// for convenience, an array of pointers to each of the six faces
POINT3D *Faces[] = {Face1,Face2,Face3,Face4,Face5,Face6};
LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
void CheckForMovement(void);
void DrawCube(void);
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdParam, int iCmdShow)
{
   static char szAppName[] = "FGcube";
   HWND        hWnd;
   MSG         msg;
   WNDCLASSEX  wndclass;
   wndclass.cbSize        = sizeof(wndclass);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   wndclass.lpfnWndProc   = WindowProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = hInstance;
   wndclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
   wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
   wndclass.hbrBackground = NULL;
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = szAppName;
   wndclass.hIconSm       = LoadIcon(NULL,IDI_APPLICATION);
   RegisterClassEx(&wndclass);
   hWnd = CreateWindow(szAppName, // window class name
      "Single 3D Cube",        // window caption
      WS_OVERLAPPEDWINDOW,     // window style
      CW_USEDEFAULT,           // initial x position
      CW_USEDEFAULT,           // initial y position
      CW_USEDEFAULT,           // initial x size
      CW_USEDEFAULT,           // initial y size
      NULL,                    // parent window handle
      NULL,                    // window menu handle
      hInstance,               // program instance handle
      NULL);                   // creation parameters
   ShowWindow(hWnd,iCmdShow);
   UpdateWindow(hWnd);
   // The message loop processes entries placed in the message queue.
   // When no message is ready, call CheckForMovement() to check if
   // we want to perform movement.
   while (TRUE)
   {
      if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
      {
         if (msg.message == WM_QUIT)
            break;
         else
         {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }
      else
         CheckForMovement();
   }
   return msg.wParam;
}
/****************************************************************************\
*                                                                            *
*  WindowProc()                                                              *
*                                                                            *
\****************************************************************************/
HDC      hDC;
HPALETTE hPal;
int      hVB;
UINT     cxClient, cyClient;
int      vbDepth;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   switch (iMsg)
   {
      case WM_CREATE:
         hDC = GetDC(hWnd);
         fg_setdc(hDC);
         hPal = fg_defpal();
         fg_realize(hPal);
         fg_vbinit();
         vbDepth = fg_colors();
         fg_vbdepth(vbDepth);
         hVB = fg_vballoc(vbWidth,vbHeight);
         fg_vbopen(hVB);
         fg_vbcolors();
         fg_setcolor(-1);
         fg_fillpage();
         fg_3Dviewport(0,vbWidth-1,0,vbHeight-1,0.5);
         fg_3Drenderstate(FG_ZCLIP);
         return 0;
      case WM_PAINT:
         BeginPaint(hWnd,&ps);
         fg_vbscale(0,vbWidth-1,0,vbHeight-1,0,cxClient-1,0,cyClient-1);
         EndPaint(hWnd,&ps);
         return 0;
      case WM_SETFOCUS:
         fg_realize(hPal);
         InvalidateRect(hWnd,NULL,TRUE);
         return 0;
      case WM_SIZE:
         cxClient = LOWORD(lParam);
         cyClient = HIWORD(lParam);
         return 0;
      case WM_DESTROY:
         fg_vbclose();
         fg_vbfree(hVB);
         fg_vbfin();
         DeleteObject(hPal);
         ReleaseDC(hWnd,hDC);
         PostQuitMessage(0);
         return 0;
   }
   return DefWindowProc(hWnd,iMsg,wParam,lParam);
}
/****************************************************************************\
*                                                                            *
*  CheckForMovement()                                                        *
*                                                                            *
*  The CheckForMovement() function checks for key presses that control the   *
*  cube's movement, and if required redraws the cube at its new position and *
*  orientation. It is called from the WinMain() message loop when there are  *
*  no messages waiting.                                                      *
*                                                                            *
\****************************************************************************/
void CheckForMovement()
{
   static double xWorld = 0.0, yWorld = 0.0, zWorld = 100.0;
   static int xAngle = 0, yAngle = 0, zAngle = 0;
   static BOOL Redraw = TRUE;
   BOOL ShiftKey;
   // check if either shift key is pressed
   ShiftKey = fg_kbtest(42) | fg_kbtest(54);
   // + and - move cube along the z axis (+ is toward viewer, - is
   // away from viewer)
   if (fg_kbtest(74))
   {
      zWorld += 3.0;
      Redraw = TRUE;
   }
   else if (fg_kbtest(78))
   {
      zWorld -= 3.0;
      Redraw = TRUE;
   }
   // left and right arrow keys move cube along x axis
   else if (fg_kbtest(75))
   {
      xWorld -= 3.0;
      Redraw = TRUE;
   }
   else if (fg_kbtest(77))
   {
      xWorld += 3.0;
      Redraw = TRUE;
   }
   // up and down arrow keys move cube along y axis
   else if (fg_kbtest(72))
   {
      yWorld += 3.0;
      Redraw = TRUE;
   }
   else if (fg_kbtest(80))
   {
      yWorld -= 3.0;
      Redraw = TRUE;
   }
   // x rotates counterclockwise around x axis, X rotates clockwise
   else if (fg_kbtest(45))
   {
      if (ShiftKey)
      {
         xAngle += 6;
         if (xAngle >= 360) xAngle -= 360;
      }
      else
      {
         xAngle -= 6;
         if (xAngle < 0) xAngle += 360;
      }
      Redraw = TRUE;
   }
   // y rotates counterclockwise around y axis, Y rotates clockwise
   else if (fg_kbtest(21))
   {
      if (ShiftKey)
      {
         yAngle += 6;
         if (yAngle >= 360) yAngle -= 360;
      }
      else
      {
         yAngle -= 6;
         if (yAngle < 0) yAngle += 360;
      }
      Redraw = TRUE;
   }
   // z rotates counterclockwise around z axis, Z rotates clockwise
   else if (fg_kbtest(44))
   {
      if (ShiftKey)
      {
         zAngle += 6;
         if (zAngle >= 360) zAngle -= 360;
      }
      else
      {
         zAngle -= 6;
         if (zAngle < 0) zAngle += 360;
      }
      Redraw = TRUE;
   }
   // if the cube's position or rotation changed, redraw the cube
   if (Redraw)
   {
      // erase the previous frame from the virtual buffer
      fg_setcolor(-1);
      fg_fillpage();
      // define the cube's new position and rotation in 3D world space
      fg_3Dsetobject(xWorld,yWorld,zWorld,xAngle*10,yAngle*10,zAngle*10);
      // draw the cube
      DrawCube();
      // display what we just drew
      fg_vbscale(0,vbWidth-1,0,vbHeight-1,0,cxClient-1,0,cyClient-1);
      Redraw = FALSE;
   }
}
/****************************************************************************\
*                                                                            *
*  DrawCube()                                                                *
*                                                                            *
*  Draws each of the six cube faces in 3D world space.                       *
*                                                                            *
\****************************************************************************/
void DrawCube()
{
   register int i;
   int r, g, b;
   static int Colors[] = {84,88,92,96,100,104};
   for (i = 0; i < 6; i++)
   {
      if (vbDepth > 8)
      {
         fg_getrgb(Colors[i],&r,&g,&b);
         fg_setcolorrgb(r,g,b);
      }
      else
      {
         fg_setcolor(Colors[i]);
      }
      fg_3Dpolygonobject((double *)Faces[i],4);
   }
}
