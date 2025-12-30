//#include "window.h"
#include "engine.h"
#include "types.h"
#include <math.h>


int WIDTH = 320/2;
int HEIGHT = 200/2;
int disToCanvas;

// g_camera Coordinates
POINT3D g_camera = {0,0,-50};
POINT3D g_cameraRotation ={0,90,0};


int initEngine(void)
{
	disToCanvas= tan(45) * WIDTH/2;
	return RT_OK;
}

// The function that changes 3D coordinates to 2D
POINT2D calculate2dzle(POINT3D _point3d)
{
	POINT2D _point2d = {1,1};

	_point2d.x = 1;
	_point2d.y = 1;

	      int X = (g_camera.x - _point3d.x);
          int Y = (g_camera.y - _point3d.y);
          int Z = (g_camera.z - _point3d.z);
          
          double Cx = cos(g_cameraRotation.x); // cos(θx)
          double Cy = cos(g_cameraRotation.y); // cos(θy)
          double Cz = cos(g_cameraRotation.z); // cos(θz)
          
          double Sx = sin(g_cameraRotation.x); // sin(θx)
          double Sy = sin(g_cameraRotation.y); // sin(θy)
          double Sz = sin(g_cameraRotation.z); // sin(θz)
          


		  double Dx = Cy * (Sz*Y + Cz*X) - Sy*Z;
			double Dy = Sx * (Cy*Z + Sy * (Sz*Y + Cz*X)) + Cx * (Cz*Y + Sz*X);
			double Dz = Cx * (Cy*Z + Sy * (Sz*Y + Cz*X)) - Sx * (Cz*Y + Sz*X);
          
          double Ex = _point3d.x / _point3d.z * disToCanvas; // This isn't 100% correct
          double Ey = _point3d.y / _point3d.z * disToCanvas; // This isn't 100% correct
          double Ez = disToCanvas;                   // This isn't 100% correct
          
          _point2d.x = Ez/Dz * Dx - Ex + WIDTH/2;  // Adding WIDTH/2 to center the g_camera
          _point2d.y = Ez/Dz * Dy - Ez + HEIGHT/2; // Adding HEIGHT/2 to center the g_camera

	return _point2d;
}

POINT2D calculate2d(POINT3D _point3d)
{
	double f = _point3d.z-g_camera.z;
	POINT2D _point2d;

	_point2d.x = ((_point3d.x-g_camera.x) * (f/_point3d.z))+g_camera.x + WIDTH;
	_point2d.y = ((_point3d.y-g_camera.y) * (f/_point3d.z))+g_camera.y + HEIGHT;
	return _point2d;
}


/*
var WIDTH = 320/2;
      var HEIGHT = 200/2;
      // Distance from center of Canvas (g_camera) with a Field of View of 90 digress, to the Canvas
      var disToCanvas = tan(45) * WIDTH/2;
      
      var canvas = document.createElement('canvas');
      canvas.width = WIDTH;
      canvas.height = HEIGHT;
      
      document.body.appendChild(canvas);
      
      var ctx = canvas.getContext('2d');
      
      var Player = function (){ // g_camera
      // g_camera Coordinates
      	this.x = 0;
        this.y = 0;
        this.z = 0;
      // g_camera Rotation (Angle)
        this.rx = 0;
        this.ry = 90;
        this.rz = 0;
      };
      var player = new Player();
      
      var Point = function (x, y ,z){
      // Point 3D Coordinates
      	this.x = x;
        this.y = y;
        this.z = z;
      // Point 2D Coordinates
        this.X2d = 0;
        this.Y2d = 0;
        
      // The function that changes 3D coordinates to 2D
        this.update = function (){
          var X = (player.x - this.x);
          var Y = (player.y - this.y);
          var Z = (player.z - this.z);
          
          var Cx = cos(player.rx); // cos(θx)
          var Cy = cos(player.ry); // cos(θy)
          var Cz = cos(player.rz); // cos(θz)
          
          var Sx = sin(player.rx); // sin(θx)
          var Sy = sin(player.ry); // sin(θy)
          var Sz = sin(player.rz); // sin(θz)
          
		  var Dx = Cy * (Sz*Y + Cz*X) - Sy*Z;
		  var Dy = Sx * (Cy*Z + Sy * (Sz*Y + Cz*X)) + Cx * (Cz*Y + Sz*X);
		  var Dz = Cx * (Cy*Z + Sy * (Sz*Y + Cz*X)) - Sx * (Cz*Y + Sz*X);
          
          var Ex = this.x / this.z * disToCanvas; // This isn't 100% correct
          var Ey = this.y / this.z * disToCanvas; // This isn't 100% correct
          var Ez = disToCanvas;                   // This isn't 100% correct
          
          this.X2d = Ez/Dz * Dx - Ex + WIDTH/2;  // Adding WIDTH/2 to center the g_camera
          this.Y2d = Ez/Dz * Dy - Ez + HEIGHT/2; // Adding HEIGHT/2 to center the g_camera
        }
      }
      // CREATING, UPDATING AND RENDERING A SQUARE
      var point = [];
      point[0] = new Point(10, 10, 10);
      point[1] = new Point(20, 10, 10);
      point[2] = new Point(20, 20, 10);
      point[3] = new Point(10, 20, 10);
      
      var run = setInterval(function (){
      	for (key in point){
        	point[key].update();
        }
        
        ctx.beginPath();
        ctx.moveTo(point[0].X2d, point[0].Y2d);
        ctx.lineTo(point[1].X2d, point[1].Y2d);
        ctx.lineTo(point[2].X2d, point[2].Y2d);
        ctx.lineTo(point[3].X2d, point[3].Y2d);
        ctx.lineTo(point[0].X2d, point[0].Y2d);
        
      }, 1000/30);

	  */