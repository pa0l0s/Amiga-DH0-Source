# include "mathArrayUtil.h"

# include <math.h>

void FindMinMaxFloat(float * src, int len, float * min, float * max)
{
	float cvalue;
	for(int idx = 0; idx < len; idx++)
	{
		cvalue = src[idx];
		
		if(*max < cvalue) *max = cvalue;
		if(*min > cvalue) *min = cvalue;
	}
}

void FindMinMaxUWORD(UWORD * src, int len, UWORD * min, UWORD * max)
{
	UWORD cvalue;
	for(int idx = 0; idx < len; idx++)
	{
		cvalue = src[idx];
		
		if(*max < cvalue) *max = cvalue;
		if(*min > cvalue) *min = cvalue;
	}
}

float CalcMeanFloat(float * src, int nrSrc)
{
	float rv = 0.0f;
	
	for(int iSrc = 0; iSrc < nrSrc; iSrc++) rv += src[iSrc];
	rv /= (float) nrSrc;
	
	return rv;
}

int IndicesWithinRangeFloat(float * src, int nrSrc, int * dst, float min, float max)
{
	int iDst = 0;
	float cSrc;
	for(int iSrc = 0; iSrc < nrSrc; iSrc++)
	{
		cSrc = src[iSrc];
		if(cSrc > min)
		{
			if(cSrc < max) dst[iDst++] = iSrc;
		}
	}
	
	return iDst;
}

BOOL DownSampleFloat(float * src, int nrSrc, float * dst, int nrDst)
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
	
	if(nrDst == 1)
	{
		dst[0] = CalcMeanFloat(src, nrSrc);
		doIt = FALSE;
	}
	
	// common case: nrDst >= 2 -- so nrSrc >= 3
	if(doIt)
	{
		// setup calc params
		float dSrc = 1.0f / ((float) nrSrc);
		float dSrc2 = 0.5f * dSrc;
		float posSrc[nrSrc];
		for(int iSrc = 0; iSrc < nrSrc; iSrc++)
		{
			posSrc[iSrc] = dSrc2 + (dSrc * ((float) iSrc));
		}

		float dDst = 1.0f / ((float) nrDst);
		float dDst2 = 0.5f * dDst;
		float posDst[nrDst];
		for(int iDst = 0; iDst < nrDst; iDst++)
		{
			posDst[iDst] = dDst2 + (dDst * ((float) iDst));
		}
		float dDstX = dDst + dSrc;
		float dDstX2 = 0.5f * dDstX;
		
		// actual calc
		float cposSrc, cposDst;
		float min, max;
		int srcIndices[nrSrc];
		int nrSrcIndices, cSrcIndex;
		float csrcMin, csrcMax;
		float cWeight, totWeight, cDst;
		for(int iDst = 0; iDst < nrDst; iDst++)
		{
			cposDst = posDst[iDst];
			
			min = cposDst - dDstX2;
			if(min < 0.0f) min = 0.0f;
			max = cposDst + dDstX2;
			if(max > 1.0f) max = 1.0f;
			nrSrcIndices = IndicesWithinRangeFloat(posSrc, nrSrc, srcIndices, min, max);
			
			min = cposDst - dDst2;
			max = cposDst + dDst2;
			totWeight = 0.0f;
			cDst = 0.0f;
			for(int iSrcIndices = 0; iSrcIndices < nrSrcIndices; iSrcIndices++)
			{
				cSrcIndex = srcIndices[iSrcIndices];
				cposSrc = posSrc[cSrcIndex];
				
				csrcMin = cposSrc - dSrc2;
				if(csrcMin < min) csrcMin = min;
				csrcMax = cposSrc + dSrc2;
				if(csrcMax > max) csrcMax = max;
				
				cWeight = csrcMax - csrcMin;
				cDst += cWeight * src[cSrcIndex];
				totWeight += cWeight;
			}
			cDst /= totWeight;
			dst[iDst] = cDst;
		}
	}
	
	return rv;
}































