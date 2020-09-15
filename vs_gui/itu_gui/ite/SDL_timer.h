#ifndef _SDL_timer_h
#define _SDL_timer_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif
#define TIME_WRAP_VALUE	(~(DWORD)0)


void SDL_StartTicks(void);

uint32_t SDL_GetTicks(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif


#endif /* _SDL_timer_h */

/* vi: set ts=4 sw=4 expandtab: */
