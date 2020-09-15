#include <windows.h>
#include <MMSystem.h>
#include <stdint.h>
#include "SDL_timer.h"
#pragma comment(lib, "winmm.lib")
static DWORD start;

void
SDL_StartTicks(void)
{
	timeBeginPeriod(1);     /* use 1 ms timer precision */
	start = timeGetTime();
}

uint32_t
SDL_GetTicks(void)
{
	DWORD now, ticks;
	now = timeGetTime();
	if (now < start) {
		ticks = (TIME_WRAP_VALUE - start) + now;
	}
	else {
		ticks = (now - start);
	}
	return (ticks);
}