/*
 * Copyright (c) 2011 ITE Tech. Inc. All Rights Reserved.
 */
/** @file
 * PAL Posix WIN32 functions.
 *
 * @author Jim Tan
 * @version 1.0
 */
#include "itu.h"
#include <windows.h>
uint32_t itpGetTickCount(void)
{
	return GetTickCount();
}

uint32_t itpGetTickDuration(uint32_t tick)
{
	return GetTickCount() - tick;
}