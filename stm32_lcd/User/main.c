/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   LTDC��Һ����ʾӢ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32H743 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */  
#include "stm32h7xx.h"
#include <string.h>
#include "main.h"
#include "cmsis_os.h"

#include "./led/bsp_led.h"
#include "./delay/core_delay.h" 
#include "./mpu/bsp_mpu.h" 
#include "./sdram/bsp_sdram.h" 
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"

#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"

#include "SDL_events.h"
#include "itu.h"
#define ITH_RGB565(r, g, b) \
    ((((uint16_t)(r) >> 3) << 11) | (((uint16_t)(g) >> 2) << 5) | ((uint16_t)(b) >> 3))
	


osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
	
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);	
	
void Delay(__IO uint32_t nCount); 


void LCD_Test(void);
ITUScene            theScene;
static ITUSurface   *screenSurf;




//static ITUBackground* _create_background(ITURectangle *rect, ITUColor *color, char *name)
//{
//	ITUBackground* bg = (ITUBackground* )calloc(1, sizeof(ITUBackground));
//	ituBackgroundInit(bg);
//	((ITUWidget *)bg)->visible = 1;
//	ITUWidget* widget = (ITUWidget*)bg;
//	widget->effectSteps = 10;
//	memmove(&widget->rect, rect, sizeof(ITURectangle));
//	memmove(&widget->color, color, sizeof(ITUColor));
//	strcpy(widget->name, name);
//	widget->alpha = 255;
//	return bg;
//}

//static ITULayer* _create_layer(ITURectangle *rect, ITUColor *color, char *name)
//{
//	ITULayer* layer = NULL;
//	int len = sizeof(ITULayer);
//	printf("len=%d\n", len);
//	layer = (ITULayer* )calloc(1, sizeof(ITULayer));
//	ituLayerInit(layer);
//	((ITUWidget *)layer)->visible = 1;
//	ITUWidget* widget = (ITUWidget*)layer;
//	memmove(&widget->rect, rect, sizeof(ITURectangle));
//	memmove(&widget->color, color, sizeof(ITUColor));
//	strcpy(widget->name, name);
//	widget->alpha = 255;
//	return layer;
//}

//static void _test_init()
//{
//#define ADD_WIDGET(W,H,X,Y,R,G,B) do{memset(&rect, 0, sizeof(ITURectangle));memset(&color, 0, sizeof(ITUColor));\
//rect.width = W;rect.height = H; rect.x=X;rect.y=Y;color.red=R;color.green=G;color.blue=B;color.alpha=255;}while (0)
//	ITUBackground* bg1 = NULL;
//	ITUBackground* bg2 = NULL;
//	ITULayer *layer1 = NULL;
//	ITUButton* btn1 = NULL;
//	
//	
//	ITURectangle rect;
//	ITUColor color;
//	//????layer
//	ADD_WIDGET(T_WIDTH, T_HEIGHT, 0, 0, 0, 0, 0);
//	layer1 = _create_layer(&rect, &color, "layer1");
//	theScene.root = (ITUWidget *)layer1;

//	//????back
//	ADD_WIDGET(50, 50, 0, 0, 0, 0, 255);
//	bg1 = _create_background(&rect, &color, "bk_wudan1");
//	itcTreePushFront((ITCTree *)layer1, bg1);

//	//????back
//	ADD_WIDGET(80, 80, 120, 120, 255, 0, 255);
//	bg2 = _create_background(&rect, &color, "bk_wudan2");
//	itcTreePushFront((ITCTree *)layer1, bg2);
//	
//	return;

//#undef ADD_WIDGET(W,H,X,Y,R,G,B)
//}


void test_event(){
	SDL_Event event_ev;
	event_ev.type = SDL_MOUSEMOTION;
	event_ev.button.x = 1;
	event_ev.button.y = 20;
	SDL_PushEvent(&event_ev);	

}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{  
	SDL_Event ev;
	/* ϵͳʱ�ӳ�ʼ����480MHz */
	SystemClock_Config();
  
  /* ���� MPU */
  Board_MPU_Config(0, MPU_Normal_WT, 0xD0000000, MPU_32MB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_512KB);
  
  SCB_EnableICache();    // ʹ��ָ�� Cache
  SCB_EnableDCache();    // ʹ������ Cache
	
	/* H750XBH6��ADC3_CH1ʹ�õ���PC3_C����PC3��������ͬ�����ţ�ͨ��һ��ģ�⿪�����ӣ�ʹ��ʱ��Ҫ�л� */
  /* PC3_C ------> ADC3_INP1  */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_CLOSE);  
	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	/* ���ô���1Ϊ��115200 8-N-1 */
	DEBUG_USART_Config();	
	
	printf("\r\n ��ӭʹ��Ұ��  STM32 H743 �����塣\r\n");		 
	printf("\r\nҰ��STM32H750 LTDCҺ����ʾӢ�Ĳ�������\r\n");
	


	
	/*������*/
	LED_BLUE;
	
	  /* ��ʼ�������� */
	GTP_Init_Panel(); 
	
	/* LCD �˿ڳ�ʼ�� */ 
	LCD_Init();
	/* LCD ��һ���ʼ�� */ 
	LCD_LayerInit(0, LCD_FB_START_ADDRESS,RGB565);
	/* LCD �ڶ����ʼ�� */ 
	LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),RGB565);
	/* ʹ��LCD������������ */ 
	LCD_DisplayOn(); 

	/* ѡ��LCD��һ�� */
	LCD_SelectLayer(0);

	/* ��һ����������ʾȫ�� */ 
	LCD_Clear(LCD_COLOR_BLACK);  

	/* ѡ��LCD�ڶ��� */
	LCD_SelectLayer(1);

	/* �ڶ�����������ʾȫ�� */ 
	LCD_Clear(LCD_COLOR_TRANSPARENT);

	/* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 0);

	ituLcdInit();
	ituSWInit();
//	while(1){
//		LCD_Test();
//	}
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	printf("defaultTaskHandle=0x%02X\n", defaultTaskHandle);

	/* definition and creation of myTask02 */
	osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 128);
	myTask02Handle = osThreadCreate(osThread(myTask02), NULL);
	printf("myTask02Handle=0x%02X\n", myTask02Handle);
	/* Start scheduler */
	osKernelStart();
	  
	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
//	ituSceneInit(&theScene, NULL);
//	screenSurf = ituGetDisplaySurface(); 
//	//_test_init();
//	uint32_t len = LCD_GetXSize()*LCD_GetYSize()*2;
//	LCD_SelectLayer(0);
//	//screenSurf->addr;
//	
//	//uint16_t *p_draw = (uint16_t *)screenSurf->addr;
//	

//			//clear
//	memset((void *)screenSurf->addr, 0, 800*480*2);
//	int flag = 0;
//	SDL_StartEventLoop();
//	test_event();
//	flag = SDL_PollEvent(&ev);
//	
//	
//	printf("x=%d,y=%d\n", ev.button.x, ev.button.y);
//	
//	while(1)
//	{		

//		
//		ituSceneDraw(&theScene, screenSurf);
//		LCD_DrawBuff((uint8_t *)screenSurf->addr,800*480*2);
//	}
}

/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
  /* ѡ��LCD��һ�� */
  LCD_SelectLayer(0);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*ѡ������*/
	LCD_SetFont(&LCD_DEFAULT_FONT);

	LCD_DisplayStringLine(1,(uint8_t* )"BH 5.0 inch LCD para:");
	LCD_DisplayStringLine(2,(uint8_t* )"Image resolution:800x480 px");
	LCD_DisplayStringLine(3,(uint8_t* )"Touch pad:5 point touch supported");
	LCD_DisplayStringLine(4,(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(5,(uint8_t* )"no need extern driver,RGB888,24bits data bus");
	LCD_DisplayStringLine(6,(uint8_t* )"Touch pad use IIC to communicate");
	
//	{		
	testCNT++;
	memset(dispBuff,0,100);
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(7);
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(7,(uint8_t* )dispBuff);


	/* ��ֱ�� */
	LCD_SetTextColor(LCD_COLOR_BLUE);

	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw line:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawLine(50,250,750,250);  
	LCD_DrawLine(50,300,750,300);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawLine(300,250,400,400);  
	LCD_DrawLine(600,250,600,400);

	Delay(0x1FFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_DrawRect(200,350,50,200);

	Delay(0x1FFFFFF);


	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Full Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillRect(200,350,50,200);

	Delay(0x1FFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	/* ��Բ */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
	LCD_DrawCircle(200,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
	LCD_DrawCircle(350,350,75);

	Delay(0x1FFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*���Բ*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw full circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillCircle(300,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);

	Delay(0x1FFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	LCD_ClearLine(8);
	
	/*͸��Ч�� ǰ�������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Transparency effect:");
	
	/*����ǰ���㲻͸����*/
	LCD_SetTransparency(1, 100);

  /* ѡ��LCD��һ�� */
  LCD_SelectLayer(1);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*��ǰ����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
	LCD_FillCircle(400,350,75);
	
	Delay(0x1FFFFFF);
	
	/*͸��Ч�� ���������*/

	/* ѡ��LCD������ */
	LCD_SelectLayer(0);	
	LCD_Clear(LCD_COLOR_BLACK);		
	/*���ñ����㲻͸��*/
	LCD_SetTransparency(1, 0);
	

	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);
	
	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillCircle(350,350,75);
	
	Delay(0x1FFFFFF);
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
//	}

}


/**
  * @brief  System Clock ����
  *         system Clock ��������: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 480000000 (CPU Clock)
	*            HCLK(Hz)             = 240000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  120MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  120MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  120MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  120MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 192
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** ���õ�Դ���ø���
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** ����������ѹ�������ѹ
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** ��ʼ��CPU��AHB��APB����ʱ��
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** ��ʼ��CPU��AHB��APB����ʱ��
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
		while(1);
  }
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
   printf("wudan start\n"); 
    

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	   LCD_Test();
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
	printf("wudan1 start\n"); 
  for(;;)
  {
	  printf(">>>>>>xxxwudan1\n"); 
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/****************************END OF FILE***************************/
