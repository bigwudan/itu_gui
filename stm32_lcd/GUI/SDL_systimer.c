

#include <stdint.h>
#include "SDL_timer.h"
#include "stm32h7xx.h"
#include "FreeRTOS.h"

static uint32_t start;

void
SDL_StartTicks(void)
{

}

uint32_t
SDL_GetTicks(void)
{
	uint32_t now, ticks;
	//ticks = HAL_GetTick();
	ticks = xTaskGetTickCount();
	return (ticks);
}