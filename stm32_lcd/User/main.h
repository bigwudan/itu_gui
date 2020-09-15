#ifndef __MAIN_H
#define __MAIN_H

#include "stm32h7xx_hal.h"
#include "heap_4.h"
static void SystemClock_Config(void);

#endif /* __MAIN_H */

#define calloc(n, s) pvPortCalloc((n), (s))
#define free(p) vPortFree( p)
#define malloc(s) pvPortMalloc((s))