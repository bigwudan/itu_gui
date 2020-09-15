#include "itu_cfg.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <stdint.h>

#define MMIO_BASE 0xB0000000
#define MMIO_SIZE 0x3000000
static uint8_t mmio[MMIO_SIZE];
static __declspec(align(4096)) uint8_t vram[0x4000000];

int SpiOpen(DWORD dwClockRate)
{
    // default values
    return 0;
}

uint16_t ithReadRegH(uint16_t addr)
{
    //assert(0);
    return 0x0;
}

void ithWriteRegH(uint16_t addr, uint16_t data)
{
    //assert(0);
}

static uint32_t ToMmioAddr(uint32_t addr)
{
	uint32_t addrHi = addr & 0xFFFF0000;
	uint32_t addrLo = addr & 0x0000FFFF;
	assert(addr >= MMIO_BASE);

	addr = ((addrHi - MMIO_BASE) >> 4) | addrLo;
	assert(addr < MMIO_SIZE);

	return addr;
}

uint32_t ithReadRegA(uint32_t addr)
{
	uint32_t addr2 = ToMmioAddr(addr);
	uint32_t data = *(uint32_t*)&mmio[addr2];
	return mmio;
}

void ithWriteRegA(uint32_t addr, uint32_t data)
{


}

uint8_t ithReadReg8(uint32_t addr)
{

    return 0;
}

void ithWriteReg8(uint32_t addr, uint8_t data)
{

}

uint16_t ithReadReg16(uint32_t addr)
{

    return 0;
}

void ithWriteReg16(uint32_t addr, uint16_t data)
{

}

uint16_t ithReadShortRegA(uint32_t addr)
{
    return 0;
}

void *ithMapVram(uint32_t addr, uint32_t size, uint32_t flags)
{
	return addr;
}

void ithUnmapVram(void *ptr, uint32_t size)
{}

void ithFlushDCacheRange(void *addr, uint32_t size)
{}

uint32_t ithSysAddr2VramAddr(void *sys_addr)
{
  

    return 0;
}

void* ithVramAddr2SysAddr(uint32_t vram_addr)
{


    return NULL;
}
