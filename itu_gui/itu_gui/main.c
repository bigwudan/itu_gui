//// ConsoleApplication3.cpp : Defines the entry point for the console application.



#include <windows.h>

#include <stdint.h>
#include <assert.h>
#include "ite\itu.h"
//
ITUScene            theScene;
static ITUSurface   *screenSurf;

static HWND hWnd;

static LRESULT CALLBACK
WndProc(
HWND hwnd,
UINT msg,
WPARAM wparam,
LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			return 0;

		case VK_UP:
		case VK_DOWN:
		case VK_RETURN:
		default:
			break;
		}
		break;

	case WM_CREATE:
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


//初始化头结点
static void _test_init()
{
	// init itu
	ituLcdInit();
	ituSWInit();
	ITUBackground* bg = calloc(1, sizeof(ITUBackground));
	if (!bg) return;
	screenSurf = ituGetDisplaySurface();

	theScene.root = bg;



	bg->orgHeight = 100;
	bg->orgWidth = 50;
	ituBackgroundInit(bg);
	((ITUWidget *)bg)->visible = 1;

	ITUWidget* widget = (ITUWidget*)bg;

	widget->rect.height = 100;
	widget->rect.width = 50;


	widget->color.alpha = 255;
	widget->color.red = 255;
	widget->alpha = 255;

	printf("wudan\n");
	return;
}


int main(void)
{
	uint8_t *pArray;
	WNDCLASS wc;
	MSG msg;
	HDC     hdc, bitmapDc;
	BITMAPINFOHEADER *bmiHeader;
	DWORD            *colors;
	int uiTotalBytes = 1000;
	int count = 0;
	bmiHeader = (BITMAPINFOHEADER *)calloc(1, sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 3);

	assert(bmiHeader);

	bmiHeader->biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader->biWidth = 20;
	bmiHeader->biHeight = -5;
	bmiHeader->biPlanes = 1;
	bmiHeader->biBitCount = 16;
	bmiHeader->biCompression = BI_BITFIELDS;
	bmiHeader->biSizeImage = 0;
	bmiHeader->biXPelsPerMeter = 0;
	bmiHeader->biYPelsPerMeter = 0;
	bmiHeader->biClrUsed = 3;
	bmiHeader->biClrImportant = 0;

	colors = (DWORD *)(bmiHeader + 1);

	colors[0] = 0xF800;
	colors[1] = 0x07E0;
	colors[2] = 0x001F;


	// create window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "ITE";

	if (RegisterClass(&wc) == FALSE)
		return -1;

	hWnd = CreateWindow("ITE",
		"ITE APPLICATION", // Window name
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		T_WIDTH,
		T_HEIGHT,
		NULL,
		NULL,
		wc.hInstance,
		NULL);
	if (hWnd == NULL)
		return -1;

	hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateDIBSection(hdc, (BITMAPINFO *)bmiHeader, DIB_RGB_COLORS, (void **)&pArray, NULL, 0);//创建DIB

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);



	_test_init();

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		ituSceneDraw(&theScene, screenSurf);

		//count++;
		//Sleep(1000);
		//for (int i = 0; i < uiTotalBytes; i++){
		//	if (count % 2 == 1){
		//		pArray[i] = 0xcc;
		//	}
		//	else{
		//		pArray[i] = 0xff;
		//	}

		//}
		//SelectObject(hdcMem, hbmp);
		//BitBlt(hdc, 0, 0, 1030, 629, hdcMem, 0, 0, SRCCOPY);

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
