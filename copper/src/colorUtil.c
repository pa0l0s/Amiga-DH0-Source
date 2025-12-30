# include "colorUtil.h"

# include <stdio.h>
# include <math.h>

float ComponentToFraction(UWORD comp)
{
	return ((float) comp) / 15.0f;
}

UWORD FractionToComponent(float frac)
{
	return (UWORD) round(frac * 15.0f);
}



void Separate(UWORD col, UWORD *r, UWORD *g, UWORD *b)
{
	*r = (col & 0xf00) >> 8;
	*g = (col & 0x0f0) >> 4;
	*b = col & 0x00f;
}

void SeparateFractions(UWORD col, float *r, float *g, float *b)
{
	*r = ComponentToFraction((col & 0xf00) >> 8);
	*g = ComponentToFraction((col & 0x0f0) >> 4);
	*b = ComponentToFraction(col & 0x00f);
}

UWORD Combine(UWORD r, UWORD g, UWORD b)
{
	return (r << 8) + (g << 4) + b;
}

UWORD CombineFractions(float r, float g, float b)
{
	return (FractionToComponent(r) << 8) + (FractionToComponent(g) << 4) + FractionToComponent(b);
}



void Represent(UWORD col, char * dest)
{
	UWORD r, g, b;
	Separate(col, &r, &g, &b);
	sprintf(dest, "RGB(%.2d, %.2d, %.2d)", r, g, b);
}

UWORD Blend(UWORD col0, UWORD col1, float bias)
{
	float r0, g0, b0;
	float r1, g1, b1;
	SeparateFractions(col0, &r0, &g0, &b0);
	SeparateFractions(col1, &r1, &g1, &b1);
	
	float bias1 = 1.0f - bias;
	float nr = (bias1 * r0) + (bias * r1);
	float ng = (bias1 * g0) + (bias * g1);
	float nb = (bias1 * b0) + (bias * b1);
	
	return CombineFractions(nr, ng, nb);
}
