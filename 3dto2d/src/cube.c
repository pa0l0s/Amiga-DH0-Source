#include "window.h"
#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

POINT3D center = {0,0,0};

int m_nFrameNr = 0;
int m_naAgleStep = 1;

// for convenience, an array of pointers to each of the six faces
POINT3D *Faces[] = {Face1,Face2,Face3,Face4,Face5,Face6};

void drawCube(void)
{
   register int i,j,k,x,y,tmp;
   double r;
   char text[80];
   static int Colors[] = {2,5,4,6,7,3};
   double  dtest = 10.5;
   double przesuniecieKonta=0;
   double dFrameNr;
   int itest;
   POINT3D FaceRotated[4];
   POINT3D teststart3d;
   POINT3D testend3d;
   POINT2D teststart2d;
   POINT2D testend2d;

   POINT3D testpolygon[] = {teststart3d,testend3d};

   //clear window
   WaitTOF();
   windowClear();

   //rotate wzdlurz osi y czyli x z obracamy
   k = 0;
	for(j = 0; j < 4; j++)
	{
	   r = lenght2d(Face1[j].x,Face1[j].z,center.x,center.z);

	   tmp = Face1[j].x;
	   snprintf(text, 80, "%d", tmp);
		//printText( text  ,8+j*60,180,2);

		tmp = Face1[j].z;
	   snprintf(text, 80, "%d", tmp);
		//printText( text  ,30+j*60,180,2);



	   tmp = r;

	   	 snprintf(text, 80, "%d", tmp);

		 //printText( text  ,200+j*20,160,2);

	   //przesuniecie k¹ta z wspolrzednych punktu
	   //FaceRotated[j].x =  r  * sin(m_naAgleStep*m_nFrameNr * M_PI / 180 + przesuniecieKonta);
	   //FaceRotated[j].z =  r  * cos(m_naAgleStep*m_nFrameNr * M_PI / 180 + przesuniecieKonta);

	      Face1[j].z +=0.1;

   }
   //calculateFrame(Faces);






   for (i = 0; i < 1; i++)
   {
      
	windowSetAPen(Colors[i]);
      
      //fg_3Dpolygonobject((double *)Faces[i],4);

	  	//printText("cube", getTextXCenter("cube"),80+10*i,Colors[i]);

		//3Dpolygonobject((double *)Faces[i],4);

	


		DrawPolygonObject((double *)Faces[i],4);


		//PrintCoordinates(FaceRotated,4,40,8+ 40*i);

		/*
		itest = Faces[i][1].x;
		
		snprintf(text, 80, "%d", itest);

		 printText( text  ,8,100+i*8,2);
		 itest = Faces[i][1].y;
		 		snprintf(text, 80, "%d",itest);

		 printText( text  ,100,100+i*8,2);



		 //testy
		 		 itest = dtest

		 		 	 		snprintf(text, 80, "%d", itest);

		 printText( text  ,200,150+i*8,2);

		 	 		snprintf(text, 80, "%d", dtest);

		 printText( text  ,220,150+i*8,2);
		 */



   }

   dFrameNr = m_nFrameNr;
   teststart3d.x=-10;
   testend3d.x = 10;
   teststart3d.y =10;
   testend3d.y=10;
   teststart3d.z = 3+dFrameNr/50;
   testend3d.z = 3+dFrameNr/50;
   windowSetAPen(1);
   DrawPolygonObject(testpolygon,2);


   rysujZegar(20, 30, 20);

	m_nFrameNr++;
	if (m_nFrameNr > 360/m_naAgleStep){
		m_nFrameNr = 0;

		for(j = 0; j < 4; j++)
		{
		Face1[j].z = -20
		}
	}

   printText("narysowane",220,8,2);
   snprintf(text, 80, "%d", m_nFrameNr);
   printText(text,8,8,2);

   WaitTOF();
}

void runCube(void)
{
	drawCube();
}

void DrawPolygonObject(POINT3D face[], int numberOfpoints)
{
	register int i;
	POINT2D point;
	int x,y;

	//obliczam ostatni punkt
	point = calculate2d(face[numberOfpoints-1]); //numeracja od zero dlatego numberOfpoints wskazuje na pierwszy (przekrêca sie 0, 1, 2, 3 gdy wskazuje na 4 wybiera 0 nie 3 - ostatni dlatego -1)
	x = point.x;
	y = point.y;
	//ustawiam punkt startowy na ostatni punkt
	windowMove(x,y);

   for (i = 0; i < numberOfpoints; i++)
   {
		point = calculate2d(face[i]);
		x = point.x;
		y = point.y;

		//rysuje od ustawionego punktu do podanych x y, przesuwaj¹c ustawiony punkt na koniec lini (troche jak w logo)
		windowDraw(x,y);
		
   }

}

void PrintCoordinates(POINT3D face[],int numberOfpoints,int x, int y)
{
	register int i;
	char text[80];
	POINT2D point;
	int n;

   for (i = 0; i < numberOfpoints; i++)
   {
		point = calculate2d(face[i]);

		n= point.x;
		snprintf(text, 80, "%d",n );

		 printText( text  ,x,y+i*8,2);
		 n= point.y;
		snprintf(text, 80, "%d", n);
		 printText(text,x+90,y+i*8,2);
   }
}

void calculateFrame(POINT3D *faces[])
{
	int i,j;
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 4; i++)
		{
			faces[i][j].x++;

		}
	}
}

double lenght2d(double startx,double starty,double endx,double endy)
{
	double result,x,y;
	x = endx - startx;
	y = endy - starty;

	result = sqrt( x*x + y*y);
	return result;
}

double lenght3d(POINT3D start, POINT3D end)
{
	double result,x,y,z;
	x = end.x - start.x;
	y = end.y - start.y;
	z = end.z - start.z;
	result = sqrt( x*x + y*y + z*z );
	return result;
}

void rysujZegar(int centerx, int centery, int r)
{
	int x,y;
	//rysowanie zegara test
	windowSetAPen(1);
	windowMove(centerx,centery); //srodek
	//przelicznik stopni na radiany rad = ang * pi / 180

	x = r * sin (m_nFrameNr * m_naAgleStep * M_PI / 180);
	y = r * cos (m_nFrameNr * m_naAgleStep * M_PI / 180);

	x+=centerx;
	y+=centery;
	windowDraw(x,y);
}
