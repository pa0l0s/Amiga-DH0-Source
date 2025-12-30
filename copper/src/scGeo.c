#include "scGeo.h"
#include "mathUtil.h"
#include "mathArrayUtil.h"
#include "colorArrayUtil.h"

#include <stdio.h>
#include <math.h>
#include <clib/exec_protos.h>

float rBar;				// radius of the bar
float rxPath;			// radius x path ellipse
float ryPath;			// radius y path ellipse
	
float fovCam;			// (half) field of view camera
float xCam;				// x position of/distance to camera

// private/screen variables
UWORD lineTop;			// first visible/available line
UWORD lineBottom;		// last visible/available line
UWORD nrLines;			// # visible/available lines
float deltaLine;		// (nrLines) / (2 * fov)
float ratioLineAng;		// (lineBottom - lineTop) / (-2 * fov)

// tables
int lenTables;			// length of all tables; equals animation frame length
float * sinTable;		// table of pure sin (offset 0, apmlitude 1)
float * cosTable;		// table of pure cos (offset 0, apmlitude 1)
float * xTable;			// x component of path positions
float * yTable;			// y component of path positions

// derived tables
float * dstTable;		// distance from path to cam
UWORD * cTable;			// center line of bar
UWORD * hTable;			// height of bar

// derived parameters
float minDistance;
float maxDistance;
UWORD minCenter;
UWORD maxCenter;
UWORD minHeight;
UWORD maxHeight;
UWORD nrHeights;

// color
UWORD ** colorArrays;	// array[nrHeights] of color arrays[minHeight .. maxHeight]



UWORD AngleToLine(float angle)
{
	return (UWORD) round((ratioLineAng * (angle - fovCam)) + ((float) lineTop));
}

UWORD AngularRangeToLineHeight(float angRange)
{
	return (UWORD) round(deltaLine * angRange);
}



void initTables()
{
	sinTable = (float *) AllocMem(lenTables * sizeof(float), 0L);
	SampleSineFloat(sinTable, 0.0f, 1.0f, lenTables);
	cosTable = (float *) AllocMem(lenTables * sizeof(float), 0L);
	SampleCosineFloat(cosTable, 0.0f, 1.0f, lenTables);
	
	xTable = (float *) AllocMem(lenTables * sizeof(float), 0L);
	yTable = (float *) AllocMem(lenTables * sizeof(float), 0L);
	for(int iSample = 0; iSample < lenTables; iSample++)
	{
		xTable[iSample] = rxPath * cosTable[iSample];
		yTable[iSample] = ryPath * sinTable[iSample];
	}
}

void initDerivedTables()
{
	dstTable = (float *) AllocMem(lenTables * sizeof(float), 0L);
	float cx, cy, cdst;
	
	cTable = (UWORD *) AllocMem(lenTables * sizeof(UWORD), 0L);
	float cAngle;
	
	hTable = (UWORD *) AllocMem(lenTables * sizeof(UWORD), 0L);
	float angRange;
	
	for(int iSample = 0; iSample < lenTables; iSample++)
	{
		cx = xTable[iSample];
		cy = yTable[iSample];
		
		cdst = sqrt(((cx - xCam) * (cx - xCam)) + (cy * cy));
		dstTable[iSample] = cdst;
		
		cAngle = asin(cy / cdst);
		cTable[iSample] = AngleToLine(cAngle);
		
		angRange = 2.0f * asin(rBar / cdst);
		hTable[iSample] = AngularRangeToLineHeight(angRange);
	}
}

void initDerivedParameters()
{
	minDistance = 1000.0f;
	maxDistance = 0.0f;
	FindMinMaxFloat(dstTable, lenTables, &minDistance, &maxDistance);

	minCenter = 1000;
	maxCenter = 0;
	FindMinMaxUWORD(cTable, lenTables, &minCenter, &maxCenter);

	minHeight = 1000;
	maxHeight = 0;
	FindMinMaxUWORD(hTable, lenTables, &minHeight, &maxHeight);
	nrHeights = maxHeight - minHeight + 1;
}



void initColorArraysOLD(UWORD * gradient, UWORD nrColors)
{
	// TODO: take arguments into account
	// for now: 1 color
	UWORD color = 0xf00;
	
	colorArrays = (UWORD **) AllocMem(nrHeights * sizeof(UWORD *), 0L);
	UWORD currHeight;
	UWORD * currColorArray;
	for(UWORD iHeight = 0; iHeight < nrHeights; iHeight++)
	{
		currHeight = minHeight + iHeight;
		currColorArray = AllocMem(currHeight * sizeof(UWORD), 0L);
		for(UWORD iColor = 0; iColor < currHeight; iColor++)
		{
			currColorArray[iColor] = color;
		}
		
		colorArrays[iHeight] = currColorArray;
	}
}

BOOL initColorArrays(UWORD * gradient, UWORD nrColors)
{
	BOOL rv = TRUE;
	
	colorArrays = (UWORD **) AllocMem(nrHeights * sizeof(UWORD *), 0L);
	UWORD currHeight;
	UWORD * currColorArray;
	for(UWORD iHeight = 0; iHeight < nrHeights; iHeight++)
	{
		currHeight = minHeight + iHeight;
		currColorArray = AllocMem(currHeight * sizeof(UWORD), 0L);
		
		if(!DownSampleColors(gradient, nrColors, currColorArray, currHeight))
		{
			rv = FALSE;
			break;
		}
		
		colorArrays[iHeight] = currColorArray;
	}
	
	return rv;
}

void releaseColorArrays()
{
	UWORD currHeight;
	UWORD * currColorArray;
	for(UWORD iHeight = 0; iHeight < nrHeights; iHeight++)
	{
		currHeight = minHeight + iHeight;
		currColorArray = colorArrays[iHeight];
		FreeMem(currColorArray, currHeight * sizeof(UWORD));
	}
	
	FreeMem(colorArrays, nrHeights * sizeof(UWORD *));
}



UWORD calcLinesAndColors(UWORD * lines, UWORD * colors, int index)
{
	UWORD height = hTable[index];
	UWORD center = cTable[index];
	UWORD startLine = center - (height / 2);
	
	for(int idx = 0; idx < height; idx++)
	{
		lines[idx] = startLine + idx;
		colors[idx] = colorArrays[height - minHeight][idx];
	}
	
	return height;
}


// fovC in °
void scGeoInit(float rb, float rx, float ry, float fovC, float xC, int animFrames)
{	
	rBar = rb;
	rxPath = rx;
	ryPath = ry;
	
	fovCam = (M_PI * fovC) / 180.0f;
	xCam = xC;
	
	lenTables = animFrames;
	
	lineTop = 0x2d;
	lineBottom = 255;
	nrLines = lineBottom - lineTop + 1;
	deltaLine = (nrLines) / (2.0f * fovCam);
	ratioLineAng = ((float) (lineBottom - lineTop )) / (-2.0f * fovCam);
	
	initTables();
	initDerivedTables();
	initDerivedParameters();
}

void scGeoRelease()
{
	FreeMem(hTable, lenTables * sizeof(UWORD));
	FreeMem(cTable, lenTables * sizeof(UWORD));
	FreeMem(dstTable, lenTables * sizeof(float));

	FreeMem(xTable, lenTables * sizeof(float));
	FreeMem(yTable, lenTables * sizeof(float));

	FreeMem(sinTable, lenTables * sizeof(float));
	FreeMem(cosTable, lenTables * sizeof(float));
}

void LogParameters()
{
	printf("%s\n", "scGeo.LogParameters():");
	
	printf("-- %s\n", "screen:");
	printf("-- -- lineTop:    %d\n", lineTop);
	printf("-- -- lineBottom: %d\n", lineBottom);
	printf("-- -- nrLines:    %d\n", nrLines);
	
	printf("-- %s\n", "dstTable:");
	printf("-- -- minDistance:    %f\n", minDistance);
	printf("-- -- maxDistance:    %f\n", maxDistance);
	
	printf("-- %s\n", "cTable:");
	printf("-- -- minCenter:    %d\n", minCenter);
	printf("-- -- maxCenter:    %d\n", maxCenter);
	
	printf("-- %s\n", "hTable:");
	printf("-- -- minHeight:    %d\n", minHeight);
	printf("-- -- maxHeight:    %d\n", maxHeight);
}
