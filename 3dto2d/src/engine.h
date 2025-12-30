#ifndef THREDTTWOOD_ENGINE_H
#define THREDTTWOOD_ENGINE_H

typedef struct point3d
{
   double x;
   double y;
   double z;
} POINT3D;

typedef struct point2d
{
   double x;
   double y;
} POINT2D;

extern int initEngine(void);
extern POINT2D calculate2d(POINT3D _point3d);
extern POINT2D calculate2dzle(POINT3D _point3d);

extern POINT3D g_g_camera
#endif // THREDTTWOOD_ENGINE_H
