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



static ITUBackground* _create_widget(ITURectangle *rect, ITUColor *color, char *name)
{
	ITUBackground* bg = calloc(1, sizeof(ITUBackground));
	ituBackgroundInit(bg);
	((ITUWidget *)bg)->visible = 1;
	ITUWidget* widget = (ITUWidget*)bg;
	memmove(&widget->rect, rect, sizeof(ITURectangle));
	memmove(&widget->color, color, sizeof(ITUColor));
	
	strcpy(widget->name, name);


	widget->alpha = 255;
	return bg;
}

//初始化头结点
static void _test_init()
{
	// init itu
	ituLcdInit();
	ituSWInit();
	ITUBackground* bg1 = NULL;
	ITUBackground* bg2 = NULL;
	screenSurf = ituGetDisplaySurface();
	ITURectangle rect;
	memset(&rect, 0, sizeof(ITURectangle));
	ITUColor color; 
	memset(&color, 0, sizeof(ITUColor));
	rect.height = 200;
	rect.width = 400;
	rect.x = 20;
	rect.y = 30;
	color.red = 255;
	color.alpha = 255;
	bg1 = _create_widget(&rect, &color, "wudan1");
	theScene.root = bg1;

	memset(&rect, 0, sizeof(ITURectangle));
	memset(&color, 0, sizeof(ITUColor));
	rect.height = 60;
	rect.width = 80;
	rect.x = 10;
	rect.y = 20;
	color.green = 255;
	color.alpha = 255;
	bg2 = _create_widget(&rect, &color, "wudan2");


	itcTreePushFront(bg1, bg2);
	return;

	
}


int main(void)
{

	WNDCLASS wc;
	MSG msg;



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



	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);



	_test_init();

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		ituSceneDraw(&theScene, screenSurf);
		ituFlip(screenSurf);


		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
