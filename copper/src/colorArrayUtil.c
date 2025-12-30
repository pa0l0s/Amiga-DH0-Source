# include "colorArrayUtil.h"
# include "colorUtil.h"
# include "mathArrayUtil.h"

# include <stdio.h>
# include <math.h>

BOOL InterpolateColors(UWORD col0, UWORD col1, UWORD nrSamples, UWORD * samples)
{
	if(nrSamples < 1) return FALSE;
	
	if(nrSamples < 2)
	{
		samples[0] = Blend(col0, col1, 0.5f);
		return TRUE;
	}
	
	if(nrSamples < 3)
	{
		samples[0] = col0;
		samples[1] = col1;
		return TRUE;
	}
	
	// common case: 3 or more samples
	samples[0] = col0;
	samples[nrSamples - 1] = col1;
	
	float deltaBias = 1.0f / ((float) (nrSamples - 1));
	float currBias;
	for(int iblend = 1; iblend <= (nrSamples - 2); iblend++)
	{
		currBias = deltaBias * ((float) iblend);
		samples[iblend] = Blend(col0, col1, currBias);
	}
	
	return TRUE;
}

void PrintColors(UWORD * colors, UWORD nrColors)
{
	char strCol[] = "RGB(15, 15, 15)";
	for(UWORD iColor = 0; iColor < nrColors; iColor++)
	{
		Represent(colors[iColor], strCol);
		printf("-- %.3d: %s\n", iColor, strCol);
	}
}

void MirrorColors(UWORD * src, UWORD * dst, UWORD nrColors)
{
	UWORD nrColors1 = nrColors - 1;
	for(UWORD iSrc = 0; iSrc < nrColors; iSrc++)
	{
		dst[nrColors1 - iSrc] = src[iSrc];
	}
}

// dst has src appended to it -- so it should be big enough
void AppendColors(UWORD * dst, UWORD * src, UWORD nrDst, UWORD nrSrc, BOOL skip1)
{
	UWORD srcStart = 1;
	if(!skip1) srcStart = 0;
	UWORD iDst = nrDst;
	for(UWORD iSrc = srcStart; iSrc < nrSrc; iSrc++)
	{
		dst[iDst] = src[iSrc];
		iDst++;
	}
}



void SeparateFractionsColors(UWORD * colors, float *r, float *g, float *b, UWORD nrColors)
{
	for(UWORD iColor = 0; iColor < nrColors; iColor++)
	{
		SeparateFractions(colors[iColor], &r[iColor], &g[iColor], &b[iColor]);
	}
}

void CombineFractionsColors(float * r, float * g, float * b, UWORD * colors, UWORD nrColors)
{
	for(UWORD iColor = 0; iColor < nrColors; iColor++)
	{
		colors[iColor] = CombineFractions(r[iColor], g[iColor], b[iColor]);
	}
}



BOOL DownSampleColors(UWORD * src, UWORD nrSrc, UWORD * dst, UWORD nrDst)
{
	// ?? need to do this, or I get 'jump into scope of variable-length array'
	BOOL rv = TRUE;
	BOOL doIt = TRUE;
	
	// illegal/special cases
	if(nrDst >= nrSrc)
	{	
		rv = FALSE;
		doIt = FALSE;
	}
	
	if(nrDst < 1)
	{	
		rv = FALSE;
		doIt = FALSE;
	}
	
	if(doIt)
	{
		float srcR[nrSrc], srcG[nrSrc], srcB[nrSrc];
		SeparateFractionsColors(src, srcR, srcG, srcB, nrSrc);
		
		float dstR[nrDst], dstG[nrDst], dstB[nrDst];
		if(!DownSampleFloat(srcR, nrSrc, dstR, nrDst)) rv = FALSE;
		if(!DownSampleFloat(srcG, nrSrc, dstG, nrDst)) rv = FALSE;
		if(!DownSampleFloat(srcB, nrSrc, dstB, nrDst)) rv = FALSE;
		
		CombineFractionsColors(dstR, dstG, dstB, dst, nrDst);
	}
	
	return rv;
}