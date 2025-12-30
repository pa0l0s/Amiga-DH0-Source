# include "mathUtil.h"

# include <math.h>

void SampleSineWORD(WORD * dst, float offset, float amplitude, UWORD nrSamples)
{
	float deltaAngle = 2.0 * M_PI / ((float) nrSamples), currAngle;
	float currSine;

	UWORD iSample;
	WORD currSample;
 
	for (iSample = 0; iSample < nrSamples; iSample++)
	{
		currAngle = deltaAngle * ((float) iSample);
		currSine = sin(currAngle);
		currSample = (WORD) round(offset + (amplitude * currSine));
		dst[iSample] = currSample;
	}
}

void SampleSineFloat(float * dst, float offset, float amplitude, int nrSamples)
{
	float deltaAngle = 2.0 * M_PI / ((float) nrSamples), currAngle;
	float currSine;

	int iSample;
	float currSample;
 
	for (iSample = 0; iSample < nrSamples; iSample++)
	{
		currAngle = deltaAngle * ((float) iSample);
		currSine = sin(currAngle);
		currSample = offset + (amplitude * currSine);
		dst[iSample] = currSample;
	}
}

void SampleCosineFloat(float * dst, float offset, float amplitude, int nrSamples)
{
	float deltaAngle = 2.0 * M_PI / ((float) nrSamples), currAngle;
	float currSine;

	int iSample;
	float currSample;
 
	for (iSample = 0; iSample < nrSamples; iSample++)
	{
		currAngle = deltaAngle * ((float) iSample);
		currSine = cos(currAngle);
		currSample = offset + (amplitude * currSine);
		dst[iSample] = currSample;
	}
}
