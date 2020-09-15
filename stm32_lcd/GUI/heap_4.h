#ifndef _HEAP_4_H
#define _HEAP_4_H

#include <stdlib.h>
#include <stdint.h>
#define portBYTE_ALIGNMENT  4

#define configTOTAL_HEAP_SIZE 512*1024

#define portBYTE_ALIGNMENT_MASK ( 0x0007 )

#define vTaskSuspendAll()
#define	xTaskResumeAll()
#define mtCOVERAGE_TEST_MARKER()

#define	traceMALLOC( A, B )

#define vApplicationMallocFailedHook()

#define configASSERT(A)
#define traceFREE(A,B)


void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );

void *pvPortCalloc( size_t nelem, size_t size_num );

size_t xPortGetFreeHeapSize( void );

size_t xPortGetMinimumEverFreeHeapSize( void );
#endif

