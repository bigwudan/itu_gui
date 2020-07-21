//// ConsoleApplication3.cpp : Defines the entry point for the console application.



#include <windows.h>
#include <windowsx.h>
#include <stdint.h>
#include <assert.h>
#include "ite\SDL_events.h"
#include "ite\itu.h"
//
ITUScene            theScene;
static ITUSurface   *screenSurf;

static HWND hWnd;


typedef struct
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} bmpfileinfo;

typedef struct {
	uint32_t      biSize;
	uint32_t      biWidth;
	uint32_t      biHeight;
	uint16_t      biPlanes;
	uint16_t      biBitCount;
	uint32_t      biCompression;
	uint32_t      biSizeImage;
	uint32_t      biXPelsPerMeter;
	uint32_t      biYPelsPerMeter;
	uint32_t      biClrUsed;
	uint32_t      biClrImportant;
}bmpinfo;

static LRESULT CALLBACK
WndProc(
HWND hwnd,
UINT msg,
WPARAM wparam,
LPARAM lparam)
{
	SDL_Event event_ev;
	int xPos;
	int yPos;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		xPos = GET_X_LPARAM(lparam);
		yPos = GET_Y_LPARAM(lparam);
		event_ev.type = SDL_MOUSEBUTTONDOWN;
		event_ev.button.x = xPos;
		event_ev.button.y = yPos;
		SDL_PushEvent(&event_ev);
		break;

	case WM_LBUTTONUP:
		xPos = GET_X_LPARAM(lparam);
		yPos = GET_Y_LPARAM(lparam);
		event_ev.type = SDL_MOUSEBUTTONUP;
		event_ev.button.x = xPos;
		event_ev.button.y = yPos;
		SDL_PushEvent(&event_ev);
		break;

	case WM_MOUSEMOVE:
		switch (wparam)
		{
		case MK_LBUTTON:
			xPos = GET_X_LPARAM(lparam);
			yPos = GET_Y_LPARAM(lparam);
			event_ev.type = SDL_MOUSEMOTION;
			event_ev.button.x = xPos;
			event_ev.button.y = yPos;
			SDL_PushEvent(&event_ev);
			break;
		case MK_MBUTTON:
			printf("ok\n");
			break;
		default:
			break;
		}
		break;
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



static ITUBackground* _create_background(ITURectangle *rect, ITUColor *color, char *name)
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

static ITULayer* _create_layer(ITURectangle *rect, ITUColor *color, char *name)
{
	ITULayer* layer = NULL;
	layer = calloc(1, sizeof(ITULayer));
	ituLayerInit(layer);
	((ITUWidget *)layer)->visible = 1;
	ITUWidget* widget = (ITUWidget*)layer;
	memmove(&widget->rect, rect, sizeof(ITURectangle));
	memmove(&widget->color, color, sizeof(ITUColor));
	strcpy(widget->name, name);
	widget->alpha = 255;
	return layer;
}
//建立button
static ITUButton* _create_button(ITURectangle *rect, ITUColor *color, char *name)
{
	ITUButton* btn = calloc(1, sizeof(ITUButton));
	ituButtonInit(btn);
	((ITUWidget *)btn)->visible = 1;
	ITUWidget* widget = (ITUWidget*)btn;
	memmove(&widget->rect, rect, sizeof(ITURectangle));
	memmove(&widget->color, color, sizeof(ITUColor));
	strcpy(widget->name, name);
	widget->alpha = 255;

	btn->actions[0].action = ITU_ACTION_HIDE;
	btn->actions[0].ev = ITU_EVENT_PRESS;
	strcpy(btn->actions[0].target, "bk_wudan1");
	//strcpy(btn->actions[0].param, "tst");
	

	


	//ITURectangle rect;
	//memset(&rect, 0, sizeof(ITURectangle));
	//ITUColor color;
	//memset(&color, 0, sizeof(ITUColor));
	//rect.width = 80;
	//rect.height = 80;
	//rect.x = 250;
	//rect.y = 200;
	//color.red = 255;
	//color.alpha = 255;
	//((ITUWidget *)btn)->visible = 1;
	//ITUWidget* widget = (ITUWidget*)btn;
	//memmove(&widget->rect, &rect, sizeof(ITURectangle));
	//memmove(&widget->color, &color, sizeof(ITUColor));
	//strcpy(widget->name, name);
	//widget->alpha = 255;

	return btn;
}




uint8_t *map_buf = NULL;
uint32_t len_t = 0;
void test_readfile()
{
	bmpfileinfo fileheader;
	bmpinfo infoheader;
	memset(&fileheader, 0, sizeof(bmpfileinfo));
	memset(&infoheader, 0, sizeof(bmpinfo));

	FILE *p_file;
	uint8_t buf[100];
	uint32_t len = 0;
	uint32_t idx = 0;
	uint32_t color_idx[3] = { 0 };
	memset(buf, 0, sizeof(buf));


	p_file = fopen("33.bmp", "rb");

	if (p_file){

		printf("ok\n");
	}
	else{
		return -1;
		printf("no\n");
	}

	//读取fileinfo
	len = fread(buf, 1, 14 + 40, p_file);

	if (len < 14){
		printf("err! read info\n");
		return -1;
	}

	memmove(&fileheader.bfType, buf, 2);
	memmove(&fileheader.bfSize, buf + 2, 4);
	memmove(&fileheader.bfReserved1, buf + 2 + 4, 2);
	memmove(&fileheader.bfReserved2, buf + 2 + 2 + 4, 2);
	memmove(&fileheader.bfOffBits, buf + 2 + 2 + 2 + 4, 4);
	idx = 2 + 2 + 2 + 4 + 4;

	memmove(&infoheader.biSize, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biWidth, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biHeight, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biPlanes, buf + idx, 2);
	idx += 2;

	memmove(&infoheader.biBitCount, buf + idx, 2);
	idx += 2;

	memmove(&infoheader.biCompression, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biSizeImage, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biXPelsPerMeter, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biYPelsPerMeter, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biClrUsed, buf + idx, 4);
	idx += 4;

	memmove(&infoheader.biClrImportant, buf + idx, 4);
	idx += 4;



	//读取掩码
	//len = fread(buf, 1, 12, p_file);
	len = fread(&color_idx[0], 1, 4, p_file);
	len = fread(&color_idx[1], 1, 4, p_file);
	len = fread(&color_idx[2], 1, 4, p_file);



	int iLineByteCnt = (((infoheader.biWidth * infoheader.biBitCount) + 31) >> 5) << 2;


	int m_iImageDataSize = iLineByteCnt * infoheader.biHeight;

	int skip = 4 - ((infoheader.biWidth * 24) >> 3) & 3;


	map_buf = calloc(1, m_iImageDataSize);
	len = fread(map_buf, 1, m_iImageDataSize, p_file);
	len_t = m_iImageDataSize;
	uint8_t *p_line = calloc(1, iLineByteCnt);

	//for (int i = 0; i < infoheader.biHeight ; i++){
	//	printf("<<<<<<<<<<<<<<<<<<<<<<<\n");
	//	int act_len = iLineByteCnt -skip ;
	//	memmove(p_line, map_buf + i*iLineByteCnt, iLineByteCnt);
	//	for (int j = 1; j < act_len+1; j++){
	//		printf("0x%2X ", p_line[j-1]);
	//		if (j % 3 == 0) printf("\r\n");
	//	}

	//}

	printf("wudan\n");

	return;
}




//初始化头结点
static void _test_init()
{
#define ADD_WIDGET(W,H,X,Y,R,G,B) do{memset(&rect, 0, sizeof(ITURectangle));memset(&color, 0, sizeof(ITUColor));\
rect.width = W;rect.height = H; rect.x=X;rect.y=Y;color.red=R;color.green=G;color.blue=B;color.alpha=255;}while (0)
	ITUBackground* bg1 = NULL;
	ITULayer *layer1 = NULL;
	ITUButton* btn1 = NULL;
	
	ITURectangle rect;
	ITUColor color;
	//建立layer
	ADD_WIDGET(T_WIDTH, T_HEIGHT, 0, 0, 0, 0, 0);
	layer1 = _create_layer(&rect, &color, "layer1");
	theScene.root = layer1;

	//建立btn
	ADD_WIDGET(150, 100, 50, 60, 255, 0, 0);
	bg1 = _create_background(&rect, &color, "bk_wudan1");
	itcTreePushFront(layer1, bg1);


	//建立btn
	ADD_WIDGET(60, 60, 220, 200, 255, 0, 0);
	btn1 = _create_button(&rect, &color, "btn_wudan1");
	itcTreePushFront(layer1, btn1);



	//ITUIcon *bg1_icon = (ITUIcon  *)bg1;
	//bg1_icon->surf = calloc(1, sizeof(ITUSurface));
	//bg1_icon->surf->width = 300;
	//bg1_icon->surf->height = 200;
	//bg1_icon->surf->size = len_t;
	//bg1_icon->surf->addr = map_buf;


	//ITUButton* btn1 = _create_button("btn1");

	//ITULayer *layer = calloc(1, sizeof(ITULayer));
	//ituLayerInit(layer);


	//theScene.root = layer;
	//itcTreePushFront(layer, bg1);
	//theScene.root = bg1;

	//memset(&rect, 0, sizeof(ITURectangle));
	//memset(&color, 0, sizeof(ITUColor));
	//rect.height = 60;
	//rect.width = 80;
	//rect.x = 10;
	//rect.y = 20;
	//color.green = 255;
	//color.alpha = 255;
	//bg2 = _create_widget(&rect, &color, "wudan2");


	//itcTreePushFront(layer, bg1);
	return;


}




void test_wudan()
{

	HBITMAP hbitmap = LoadImage(NULL, "1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	assert(hbitmap);

	printf("wudan\n");
}

int main(void)
{

	//test_wudan();
	int flag = 0;
	//初始化时间
	SDL_StartTicks();

	//初始化事件循环
	flag = SDL_StartEventLoop();



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



	SDL_Event ev;
	// init itu
	ituLcdInit();
	ituSWInit();
	ituSceneInit(&theScene, NULL);
	screenSurf = ituGetDisplaySurface();

	
	_test_init();
	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//ituSceneUpdate(&theScene, ITU_EVENT_TIMER, 0, 0, 0);
		bool result = false;


		flag = SDL_PollEvent(&ev);
		if (flag > 0){
			switch (ev.type){
			case SDL_MOUSEMOTION:
				printf("mouse move...x=%d,y=%d\n", ev.button.x, ev.button.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("mouse down...x=%d,y=%d\n", ev.button.x, ev.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				printf("mouse up...x=%d,y=%d\n", ev.button.x, ev.button.y);
				result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOWN, ev.button.button, ev.button.x, ev.button.y);
				break;
			}


		}





		ituSceneDraw(&theScene, screenSurf);
		ituFlip(screenSurf);


		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
