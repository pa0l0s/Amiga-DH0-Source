// ----------------------------------------------------------------------------------------------------------------------------

#include <windows.h>

#ifndef WM_MOUSWHEEL
	#define WM_MOUSWHEEL 0x020A
#endif

#include "string.h"
#include "glmath.h"

#pragma comment(lib, "winmm.lib")

#pragma warning(disable : 4996)

// ----------------------------------------------------------------------------------------------------------------------------

class complex
{
public:
	double r, i;

public:
	complex();
	~complex();

public:
	complex(double r, double i);

public:
	double abs();

public:
	friend complex operator + (const complex &a, const complex &b);
	friend complex operator * (const complex &a, const complex &b);
};

// ----------------------------------------------------------------------------------------------------------------------------

class CSoftwareRenderer
{
private:
	int Width, Height;

private:
	BYTE *ColorBuffer;
	int ColorBufferWidth, ColorBufferHeight;
	BITMAPINFO ColorBufferInfo;

private:
	int Pitch;

public:
	CSoftwareRenderer();
	~CSoftwareRenderer();

public:
	int GetPitch();

public:
	void Clear();
	void SetPixel(int i, BYTE r, BYTE g, BYTE b);

public:
	void Viewport(int Width, int Height);
	void SwapBuffers(HDC hDC);
};

// ----------------------------------------------------------------------------------------------------------------------------

class CFractalRenderer : public CSoftwareRenderer
{
private:
	int LastX, LastY, LastClickedX, LastClickedY;

private:
	int Width, Height, WMH;

private:
	double cx, cy, d, aspect;

private:
	int maxmaxn, maxn, n;

private:
	complex *zs, *cs;
	int *ns;
	bool *diverges, *colored;

private:
	vec3 *Colors;
	int ColorsCount;

public:
	CFractalRenderer();
	~CFractalRenderer();

public:
	int GetN();
	int GetMaxN();

public:
	bool Init();
	void Render(float FrameTime);
	void Resize(int Width, int Height);
	void Destroy();

public:
	void RandomizeColors();

public:
	void OnKeyDown(UINT Key);
	void OnLButtonDown(int X, int Y);
	void OnLButtonUp(int X, int Y);
	void OnMouseMove(int X, int Y);
	void OnMouseWheel(short zDelta);
	void OnRButtonDown(int X, int Y);
	void OnRButtonUp(int X, int Y);
};

// ----------------------------------------------------------------------------------------------------------------------------

class CFractalView
{
private:
	char *Title;
	int Width, Height;
	HWND hWnd;
	HDC hDC;

private:
	CFractalRenderer FractalRenderer;

public:
	CFractalView();
	~CFractalView();

public:
	bool Create(HINSTANCE hInstance, char *Title, int Width, int Height);
	void Show(bool Maximized = false);
	void MsgLoop();
	void Destroy();

public:
	void OnKeyDown(UINT Key);
	void OnLButtonDown(int X, int Y);
	void OnLButtonUp(int X, int Y);
	void OnMouseMove(int X, int Y);
	void OnMouseWheel(short zDelta);
	void OnPaint();
	void OnRButtonDown(int X, int Y);
	void OnRButtonUp(int X, int Y);
	void OnSize(int Width, int Height);
};

// ----------------------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow);

// ----------------------------------------------------------------------------------------------------------------------------
