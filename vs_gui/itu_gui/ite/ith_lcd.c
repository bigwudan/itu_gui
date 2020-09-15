/*
 * Copyright (c) 2016 ITE Tech. Inc. All Rights Reserved.
 */
/** @file
 * HAL LCD functions.
 *
 * @author Irene Wang
 * @version 1.0
 */
#include <stdint.h>
#include "stdbool.h"
#include "itu_cfg.h"
#include "ith_lcd.h"

#define CMD_DELAY   0xFFFFFFFF

static const uint32_t* lcdScript;
static unsigned int lcdScriptCount, lcdScriptIndex;
static uint32_t mipiHSTable[16];
static uint32_t mipiHSPLLFrange[4];
static uint32_t pinShareTable[8];

void ithLcdReset(void)
{

}

void ithLcdEnable(void)
{
  
}

void ithLcdDisable(void)
{
 
}

void ithLcdSetBaseAddrA(uint32_t addr)
{
    
}

void ithLcdSetBaseAddrB(uint32_t addr)
{
    
}

uint32_t ithLcdGetBaseAddrB(void)
{
    return 0;
}

void ithLcdSetBaseAddrC(uint32_t addr)
{
    
}

uint32_t ithLcdGetBaseAddrC(void)
{
    return 0;
}

void ithLcdEnableHwFlip(void)
{
    
}

void ithLcdDisableHwFlip(void)
{
    
}

void ithLcdEnableVideoFlip(void)
{
    
}

void ithLcdDisableVideoFlip(void)
{
  
}

void ithLcdLoadScriptFirst(const uint32_t* script, unsigned int count)
{
  
}

void ithLcdLoadScriptNext(void)
{
}

void ithLcdCursorSetBaseAddr(uint32_t addr)
{
    
}

void ithLcdCursorSetColorWeight(ITHLcdCursorColor color, uint8_t value)
{
  
}

ITHLcdFormat ithLcdGetFormat(void)
{
    return 0;
}

void ithLcdSetWidth(uint32_t width)
{
    
}


void ithLcdSetHeight(uint32_t height)
{
    
}

void ithLcdSetPitch(uint32_t pitch)
{
    
}

unsigned int ithLcdGetXCounter(void)
{
    return 0;
}

unsigned int ithLcdGetYCounter(void)
{
    return 0;
}

void ithLcdSyncFire(void)
{
    
}

 bool ithLcdIsSyncFired(void)
{
     return true;
}

bool ithLcdIsEnabled(void)
{
    return true;
}

unsigned int ithLcdGetFlip(void)
{
    return 0;
}

unsigned int ithLcdGetMaxLcdBufCount(void)
{
#if CFG_VIDEO_ENABLE
    return 3;
#else
    return 2;
#endif
}

void ithLcdSwFlip(unsigned int index)
{
  
}

void ithLcdCursorEnable(void)
{
    
}

void ithLcdCursorDisable(void)
{
   
}

void ithLcdCursorCtrlEnable(ITHLcdCursorCtrl ctrl)
{

}

void ithLcdCursorCtrlDisable(ITHLcdCursorCtrl ctrl)
{
   
}

void ithLcdCursorSetWidth(unsigned int width)
{
    
}

void ithLcdCursorSetHeight(unsigned int height)
{
   
}

void ithLcdCursorSetPitch(unsigned int pitch)
{
    
}

void ithLcdCursorSetX(unsigned int x)
{
  
}

void ithLcdCursorSetY(unsigned int y)
{
   
}

void ithLcdCursorSetColor(ITHLcdCursorColor color, uint16_t value)
{
   
}

void ithLcdCursorUpdate(void)
{
}

 bool ithLcdCursorIsUpdateDone(void)
{
     return true;
}

void ithLcdIntrCtrlEnable(ITHLcdIntrCtrl ctrl)
{

}

void ithLcdIntrCtrlDisable(ITHLcdIntrCtrl ctrl)
{
  
}

void ithLcdIntrEnable(void)
{
    
}

void ithLcdIntrDisable(void)
{

}

void ithLcdIntrClear(void)
{
    
}

void ithLcdIntrSetScanLine1(unsigned int line)
{
   
}

void ithLcdIntrSetScanLine2(unsigned int line)
{

}

void ithLcdSetRotMode(ITHLcdScanType type, ITHLcdRotMode mode)
{
  
}

ITHLcdPanelType ithLcdGetPanelType(void)
{


	return ITH_LCD_LVDS;

}

void ithLcdSaveMipiHSInfo(void)
{
   
}

void ithLcdResetMipiHSInfo(void)
{
    
}

void ithLcdSavePinShareInfo(void)
{
    
}

void ithLcdResetPinShareInfo(void)
{
   
}



