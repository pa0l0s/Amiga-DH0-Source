// ----------------------------------------------------------------------------------------------------------------------------

#include "fractal.h"

// ----------------------------------------------------------------------------------------------------------------------------

CString ErrorLog, ModuleDirectory;

// ----------------------------------------------------------------------------------------------------------------------------

complex::complex()
{
	r = 0.0;
	i = 0.0;
}

complex::~complex()
{
}

complex::complex(double r, double i)
{
	this->r = r;
	this->i = i;
}

double complex::abs()
{
	return sqrt(r * r + i * i);
}

complex operator + (const complex &a, const complex &b)
{
	return complex(a.r + b.r, a.i + b.i);
}

complex operator * (const complex &a, const complex &b)
{
	return complex(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
}

// ----------------------------------------------------------------------------------------------------------------------------

CSoftwareRenderer::CSoftwareRenderer()
{
	Width = 0;
	Height = 0;

	ColorBuffer = NULL;
	ColorBufferWidth = 0;
	ColorBufferHeight = 0;
}

CSoftwareRenderer::~CSoftwareRenderer()
{
	if(ColorBuffer != NULL)
	{
		delete [] ColorBuffer;
	}
}

int CSoftwareRenderer::GetPitch()
{
	return Pitch;
}

void CSoftwareRenderer::Clear()
{
	if(ColorBuffer != NULL)
	{
		memset(ColorBuffer, 0, ColorBufferWidth * ColorBufferHeight * 3);
	}
}

void CSoftwareRenderer::SetPixel(int i, BYTE r, BYTE g, BYTE b)
{
	ColorBuffer[i++] = b;
	ColorBuffer[i++] = g;
	ColorBuffer[i++] = r;
}

void CSoftwareRenderer::Viewport(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;

	if(ColorBuffer != NULL)
	{
		delete [] ColorBuffer;
		ColorBuffer = NULL;
	}

	if(Width > 0 && Height > 0)
	{
		ColorBufferWidth = Width;
		ColorBufferHeight = Height;

		int WidthMod4 = Width % 4;

		if(WidthMod4 > 0)
		{
			ColorBufferWidth += 4 - WidthMod4;
		}

		Pitch = (ColorBufferWidth - Width) * 3;

		ColorBuffer = new BYTE[ColorBufferWidth * ColorBufferHeight * 3];

		memset(&ColorBufferInfo, 0, sizeof(BITMAPINFO));
		ColorBufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		ColorBufferInfo.bmiHeader.biPlanes = 1;
		ColorBufferInfo.bmiHeader.biBitCount = 24;
		ColorBufferInfo.bmiHeader.biCompression = BI_RGB;
		ColorBufferInfo.bmiHeader.biWidth = ColorBufferWidth;
		ColorBufferInfo.bmiHeader.biHeight = ColorBufferHeight;
	}
}

void CSoftwareRenderer::SwapBuffers(HDC hDC)
{
	if(ColorBuffer != NULL)
	{
		StretchDIBits(hDC, 0, 0, Width, Height, 0, 0, Width, Height, ColorBuffer, &ColorBufferInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------

CFractalRenderer::CFractalRenderer()
{
	cx = -0.5;
	cy = 0.0;
	d = 1.5;

	maxmaxn = 8192;
	maxn = maxmaxn / 32;
	n = 0;

	zs = NULL;
	cs = NULL;
	ns = NULL;
	diverges = NULL;
}

CFractalRenderer::~CFractalRenderer()
{
}

int CFractalRenderer::GetN()
{
	return n;
}

int CFractalRenderer::GetMaxN()
{
	return maxn;
}

bool CFractalRenderer::Init()
{
	ColorsCount = 128;

	Colors = new vec3[ColorsCount];

	RandomizeColors();

	return true;
}

void CFractalRenderer::Render(float FrameTime)
{
	if(n < maxn)
	{
		if(n == 0)
		{
			Clear();

			int i = 0;

			for(int y = 0; y < Height; y++)
			{
				for(int x = 0; x < Width; x++)
				{
					// zs[i] = complex(0.0, 0.0);

					zs[i].r = 0.0;
					zs[i].i = 0.0;

					// cs[i] = complex(cx + ((double)x / (double)Width * 2.0 - 1.0) * d * aspect, cy + ((double)y / (double)Height * 2.0 - 1.0) * d);

					cs[i].r = cx + ((double)x / (double)Width * 2.0 - 1.0) * d * aspect;
					cs[i].i = cy + ((double)y / (double)Height * 2.0 - 1.0) * d;

					ns[i] = 0;

					diverges[i] = false;

					colored[i] = false;

					i++;
				}
			}
		}

		DWORD Time = GetTickCount();

		while(n < maxn && GetTickCount() - Time < 250)
		{
			for(int i = 0; i < WMH; i++)
			{
				if(!diverges[i])
				{
					// zs[i] = zs[i] * zs[i] + cs[i];

					double zr = zs[i].r, zi = zs[i].i, cr = cs[i].r, ci = cs[i].i;

					zs[i].r = zr * zr - zi * zi + cr;
					zs[i].i = 2.0 * zr * zi + ci;

					ns[i] = n;

					// diverges[i] = zs[i].abs() > 2.0;

					zr = zs[i].r;
					zi = zs[i].i;

					diverges[i] = zr * zr + zi * zi > 4.0;
				}
			}

			n++;
		}
	}

	int i = 0, ii = 0;

	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			if(diverges[i] && !colored[i])
			{
				float cf = (float)(ColorsCount - 2) * (float)ns[i] / (float)maxmaxn;

				int c1 = (int)cf, c2 = c1 + 1;

				cf -= (float)c1;

				vec3 Color = mix(Colors[c1], Colors[c2], cf);

				SetPixel(ii, (BYTE)Color.r, (BYTE)Color.g, (BYTE)Color.b);

				colored[i] = true;
			}

			i++;
			ii += 3;
		}

		ii += GetPitch();
	}
}

void CFractalRenderer::Resize(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;

	WMH = Width * Height;

	Viewport(Width, Height);

	aspect = (double)Width / (double)Height;

	if(zs != NULL)
	{
		delete [] zs;
		zs = NULL;
	}

	if(cs != NULL)
	{
		delete [] cs;
		cs = NULL;
	}

	if(ns != NULL)
	{
		delete [] ns;
		ns = NULL;
	}

	if(diverges != NULL)
	{
		delete [] diverges;
		diverges = NULL;
	}

	if(colored != NULL)
	{
		delete [] colored;
		colored = NULL;
	}

	if(Width > 0 && Height > 0)
	{
		zs = new complex[WMH];
		cs = new complex[WMH];
		ns = new int[WMH];
		diverges = new bool[WMH];
		colored = new bool[WMH];
	}

	n = 0;
}

void CFractalRenderer::Destroy()
{
	if(zs != NULL)
	{
		delete [] zs;
	}

	if(cs != NULL)
	{
		delete [] cs;
	}

	if(ns != NULL)
	{
		delete [] ns;
	}

	if(diverges != NULL)
	{
		delete [] diverges;
	}

	if(colored != NULL)
	{
		delete [] colored;
	}

	delete Colors;
}

void CFractalRenderer::RandomizeColors()
{
	for(int i = 0; i < ColorsCount; i++)
	{
		Colors[i].r = 255.0f * (float)rand() / (float)RAND_MAX;
		Colors[i].g = 255.0f * (float)rand() / (float)RAND_MAX;
		Colors[i].b = 255.0f * (float)rand() / (float)RAND_MAX;
	}

	Colors[0] *= 0.125f;

	for(int i = 0; i < WMH; i++)
	{
		colored[i] = false;
	}
}

void CFractalRenderer::OnKeyDown(UINT Key)
{
	switch(Key)
	{
		case VK_F1:
			RandomizeColors();
			break;

		case '1':
			n = 0;
			maxn = maxmaxn / 512;
			break;

		case '2':
			n = 0;
			maxn = maxmaxn / 256;
			break;

		case '3':
			n = 0;
			maxn = maxmaxn / 128;
			break;

		case '4':
			n = 0;
			maxn = maxmaxn / 64;
			break;

		case '5':
			n = 0;
			maxn = maxmaxn / 32;
			break;

		case '6':
			n = 0;
			maxn = maxmaxn / 16;
			break;

		case '7':
			n = 0;
			maxn = maxmaxn / 8;
			break;

		case '8':
			n = 0;
			maxn = maxmaxn / 4;
			break;

		case '9':
			n = 0;
			maxn = maxmaxn / 2;
			break;

		case '0':
			n = 0;
			maxn = maxmaxn;
			break;

		case 'R':
			cx = -0.5;
			cy = 0.0;
			d = 1.5;
			n = 0;
			break;
	}
}

void CFractalRenderer::OnLButtonDown(int X, int Y)
{
	LastClickedX = X;
	LastClickedY = Y;
}

void CFractalRenderer::OnLButtonUp(int X, int Y)
{
	if(X == LastClickedX && Y == LastClickedY)
	{
		cx += (X - Width / 2) * d * 2.0 / (double)Width * aspect;
		cy += (Height / 2 - Y) * d * 2.0 / (double)Height;

		d *= 0.5;

		n = 0;
	}
}

void CFractalRenderer::OnMouseMove(int X, int Y)
{
	if(GetKeyState(VK_LBUTTON) & 0x80)
	{
		cx += (LastX - X) * d * 2.0 / (double)Width * aspect;
		cy += (Y - LastY) * d * 2.0 / (double)Height;

		n = 0;
	}

	LastX = X;
	LastY = Y;
}

void CFractalRenderer::OnMouseWheel(short zDelta)
{
	if(zDelta > 0)
	{
		d *= 0.5;
		n = 0;
	}

	if(zDelta < 0)
	{
		d *= 2.0;
		n = 0;
	}
}

void CFractalRenderer::OnRButtonDown(int X, int Y)
{
	LastClickedX = X;
	LastClickedY = Y;
}

void CFractalRenderer::OnRButtonUp(int X, int Y)
{
	if(X == LastClickedX && Y == LastClickedY)
	{
		cx += (X - Width / 2) * d * 2.0 / (double)Width * aspect;
		cy += (Height / 2 - Y) * d * 2.0 / (double)Height;

		d *= 2.0;

		n = 0;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------

CFractalView::CFractalView()
{
    char *moduledirectory = new char[256];
    GetModuleFileName(GetModuleHandle(NULL), moduledirectory, 256);
    *(strrchr(moduledirectory, '\\') + 1) = 0;
    ModuleDirectory = moduledirectory;
    delete [] moduledirectory;
}

CFractalView::~CFractalView()
{
}

bool CFractalView::Create(HINSTANCE hInstance, char *Title, int Width, int Height)
{
	WNDCLASSEX WndClassEx;

	memset(&WndClassEx, 0, sizeof(WNDCLASSEX));

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = WndProc;
	WndClassEx.hInstance = hInstance;
	WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.lpszClassName = "Win32OpenGLWindow";

	if(RegisterClassEx(&WndClassEx) == 0)
	{
		ErrorLog.Set("RegisterClassEx failed!");
		return false;
	}

	this->Title = Title;

	this->Width = Width;
	this->Height = Height;

	DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if((hWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, Title, Style, 0, 0, Width, Height, NULL, NULL, hInstance, NULL)) == NULL)
	{
		ErrorLog.Set("CreateWindowEx failed!");
		return false;
	}

	if((hDC = GetDC(hWnd)) == NULL)
	{
		ErrorLog.Set("GetDC failed!");
		return false;
	}

	return FractalRenderer.Init();
}

void CFractalView::Show(bool Maximized)
{
	RECT dRect, wRect, cRect;

	GetWindowRect(GetDesktopWindow(), &dRect);
	GetWindowRect(hWnd, &wRect);
	GetClientRect(hWnd, &cRect);

	wRect.right += Width - cRect.right;
	wRect.bottom += Height - cRect.bottom;

	wRect.right -= wRect.left;
	wRect.bottom -= wRect.top;

	wRect.left = dRect.right / 2 - wRect.right / 2;
	wRect.top = dRect.bottom / 2 - wRect.bottom / 2;

	MoveWindow(hWnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);

	ShowWindow(hWnd, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}

void CFractalView::MsgLoop()
{
	MSG Msg;

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void CFractalView::Destroy()
{
	FractalRenderer.Destroy();

	DestroyWindow(hWnd);
}

void CFractalView::OnKeyDown(UINT Key)
{
	FractalRenderer.OnKeyDown(Key);
}

void CFractalView::OnLButtonDown(int X, int Y)
{
	FractalRenderer.OnLButtonDown(X, Y);
}

void CFractalView::OnLButtonUp(int X, int Y)
{
	FractalRenderer.OnLButtonUp(X, Y);
}

void CFractalView::OnMouseMove(int X, int Y)
{
	FractalRenderer.OnMouseMove(X, Y);
}

void CFractalView::OnMouseWheel(short zDelta)
{
	FractalRenderer.OnMouseWheel(zDelta);
}

void CFractalView::OnPaint()
{
	PAINTSTRUCT ps;

	BeginPaint(hWnd, &ps);

	static DWORD LastFPSTime = GetTickCount(), LastFrameTime = LastFPSTime;
	static int FPS = 0;

	DWORD Time = GetTickCount();

	float FrameTime = (Time - LastFrameTime) * 0.001f;

	LastFrameTime = Time;

	if(Time - LastFPSTime > 1000)
	{
		CString Text = Title;

		Text.Append(" - %dx%d", Width, Height);
		Text.Append(", FPS: %d", FPS);
		Text.Append(" - %d / %d", FractalRenderer.GetN(), FractalRenderer.GetMaxN());

		SetWindowText(hWnd, Text);

		LastFPSTime = Time;
		FPS = 0;
	}
	else
	{
		FPS++;
	}

	FractalRenderer.Render(FrameTime);

	FractalRenderer.SwapBuffers(hDC);

	EndPaint(hWnd, &ps);

	InvalidateRect(hWnd, NULL, FALSE);
}

void CFractalView::OnRButtonDown(int X, int Y)
{
	FractalRenderer.OnRButtonDown(X, Y);
}

void CFractalView::OnRButtonUp(int X, int Y)
{
	FractalRenderer.OnRButtonUp(X, Y);
}

void CFractalView::OnSize(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;

	FractalRenderer.Resize(Width, Height);
}

// ----------------------------------------------------------------------------------------------------------------------------

CFractalView FractalView;

// ----------------------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			FractalView.OnKeyDown((UINT)wParam);
			break;

		case WM_LBUTTONDOWN:
			FractalView.OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			FractalView.OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSEMOVE:
			FractalView.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSWHEEL:
			FractalView.OnMouseWheel(HIWORD(wParam));
			break;

		case WM_PAINT:
			FractalView.OnPaint();
			break;

		case WM_RBUTTONDOWN:
			FractalView.OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			FractalView.OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_SIZE:
			FractalView.OnSize(LOWORD(lParam), HIWORD(lParam));
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

// ----------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if(FractalView.Create(hInstance, "Fractal", 800, 600))
	{
		FractalView.Show();
		FractalView.MsgLoop();
	}
	else
	{
		MessageBox(NULL, ErrorLog, "Error", MB_OK | MB_ICONERROR);
	}

	FractalView.Destroy();

	return 0;
}

// ----------------------------------------------------------------------------------------------------------------------------
