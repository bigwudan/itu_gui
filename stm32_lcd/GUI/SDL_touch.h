#ifndef _SDL_touch_h
#define _SDL_touch_h
#include <stdint.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

struct ts_sample {
	int		x;
	int		y;
	unsigned int	pressure;
	unsigned int	id;
	unsigned int	finger;	
	void *next;
};


void Castor3_PumpTouchEvent(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif


#endif /* _SDL_timer_h */

/* vi: set ts=4 sw=4 expandtab: */
