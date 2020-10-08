/* General event handling code for SDL */
#include "SDL_events.h"
#include "itu.h"
#include "SDL_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include <stdbool.h>

#define MAX_FINGER_NUM 1

static bool down[MAX_FINGER_NUM];
struct ts_sample c3samp[MAX_FINGER_NUM];

int ts_read(void *ts, struct ts_sample *samp, int nr)
{
	
	int result;
	result = GTP_TouchProcess_read(NULL, samp, nr);
	return result;

}

int
SDL_SendFingerDown(int id, int fingerid, bool down, 
                   float xin, float yin, float pressurein)
{
	int posted;
	uint16_t x;
	uint16_t y;
	uint16_t pressure;
        
    x = (uint16_t)xin;
    y = (uint16_t)yin;
    pressure = pressurein;

    if(down) {
		posted = 0;
		SDL_Event event;
		event.tfinger.type = SDL_FINGERDOWN;
		event.tfinger.touchId = id;
		event.tfinger.x = x;
		event.tfinger.y = y;
		event.tfinger.dx = 0;
		event.tfinger.dy = 0;
		event.tfinger.pressure = pressure;
		event.tfinger.numFingers = 1;
		event.tfinger.state = 1;
		event.tfinger.windowID = id;
		event.tfinger.fingerId = fingerid;
		posted = (SDL_PushEvent(&event) > 0);		
		return posted;
    }else{
		posted = 0;
		SDL_Event event;
		event.tfinger.type = SDL_FINGERUP;
		event.tfinger.touchId =  id;
		event.tfinger.state = 1;
		event.tfinger.windowID = id;
		event.tfinger.fingerId = fingerid;
		//I don't trust the coordinates passed on fingerUp
		event.tfinger.x = 0; 
		event.tfinger.y = 0;
		event.tfinger.dx = 0;
		event.tfinger.dy = 0;
		event.tfinger.pressure = pressure;
		event.tfinger.numFingers = 1;
		posted = (SDL_PushEvent(&event) > 0);
		return posted;
	} 
				   
}

void Castor3_PumpTouchEvent(void){
    int ret;
    int k=0;
    int curFgr = 0;
    bool cDn[10] = {false};
    int i;
    
    /* init samples */
    for(i=0; i<MAX_FINGER_NUM; i++)
    {
        c3samp[i].finger = 0;
        c3samp[i].pressure = 0;
    }    


	ret = ts_read(NULL, &c3samp[0], 1);
	if(ret > 0){
		
		//如果是点击
		if (c3samp[0].pressure){
			if (down[c3samp[0].id] == false){
				SDL_SendFingerDown(c3samp[0].id, c3samp[0].id, true, (float)c3samp[0].x, (float)c3samp[0].y, 1);
			}
		
		} else{
		
		
		}
		
	
	}
	
	
	return ;
}
