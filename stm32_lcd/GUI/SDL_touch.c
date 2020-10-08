/* General event handling code for SDL */
#include "SDL_events.h"
#include "itu.h"
#include "SDL_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include <stdbool.h>
#include "SDL_timer.h"
#define MAX_FINGER_NUM 1

static bool down[MAX_FINGER_NUM];
struct ts_sample c3samp[MAX_FINGER_NUM];
static ts_last_samp lstSmp;
static struct ts_sample OriSmp[1] = {0};

int ts_read(void *ts, struct ts_sample *samp, int nr)
{
	
	int result;
	result = GTP_TouchProcess_read(NULL, samp, nr);
	return result;

}

int
SDL_SendTouchMotion(int id, int fingerid, int relative, 
                    float xin, float yin, float pressurein)
{

    int posted;
    int xrel, yrel;
    uint16_t x;
    uint16_t y;
    uint16_t pressure;
    x = (uint16_t)xin;
    y = (uint16_t)yin;
    pressure = (uint16_t)pressurein;
	xrel = x - lstSmp.x;
	yrel = y - lstSmp.y;
	if (!xrel && !yrel) {
		return 0;
	}
	/* Post the event, if desired */
	posted = 0;
	SDL_Event event;
	event.tfinger.type = SDL_FINGERMOTION;
	event.tfinger.touchId = id;
	event.tfinger.fingerId = fingerid;
	event.tfinger.x = x;
	event.tfinger.y = y;
	event.tfinger.dx = xrel;
	event.tfinger.dy = yrel;            
		
	event.tfinger.pressure = pressure;
	event.tfinger.numFingers =1;
	event.tfinger.state = 1;
	event.tfinger.windowID = id;
	posted = (SDL_PushEvent(&event) > 0);
	return posted;
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
		event.tfinger.x = lstSmp.x; 
		event.tfinger.y = lstSmp.y;
		event.tfinger.dx = 0;
		event.tfinger.dy = 0;
		event.tfinger.pressure = pressure;
		event.tfinger.numFingers = 1;
		posted = (SDL_PushEvent(&event) > 0);
		return posted;
	} 
				   
}

static void update_last_sample(int tId, struct ts_sample* smp)
{
	lstSmp.p = (int)smp->pressure;
	lstSmp.x = (int)smp->x;
	lstSmp.y = (int)smp->y;	
	lstSmp.fn = (int)smp->finger;
}


/*
input: id the finger ID; smp the current sample structure pointer
return: true is the same, false is different
*/
static bool check_last_sample(int tId, struct ts_sample* smp)
{
	int tmpP;
	int tmpX;
	int tmpY;
	int tmpFN;
	bool tRst = true;	
	static uint32_t tc=0;
	tmpP = (int)smp->pressure;
	tmpX = (int)smp->x;
	tmpY = (int)smp->y;
	tmpFN = (int)smp->finger;	

	
	if( lstSmp.p != tmpP )	tRst = false;		
	if( lstSmp.x != tmpX )	tRst = false;
	if( lstSmp.y != tmpY )	tRst = false;

	//add this code to avoid from too many TP motion events to be filled up with
	if( (tmpFN > 1) && (tRst == false) )
	{
		if( (SDL_GetTicks()  - 16) >=  tc ){
			tRst = true;
			tc = SDL_GetTicks();
		}

	}


	
	return tRst;
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
				down[c3samp[0].id] = true;
				update_last_sample((int)c3samp[0].id, &c3samp[0]);
			}else{
				if( check_last_sample((int)c3samp[0].id, &c3samp[0]) == false )	//motion has changed
	        	{
	        		SDL_SendTouchMotion(c3samp[0].id, c3samp[0].id, false, (float)c3samp[0].x, (float)c3samp[0].y, 1);
	        		update_last_sample((int)c3samp[0].id, &c3samp[0]);
	        	}
				
				
			}
		
		} else{
			if(down[c3samp[0].id]==true)
	    	{
				SDL_SendFingerDown(c3samp[0].id, c3samp[0].id, false, (float)c3samp[0].x, (float)c3samp[0].y, 1);	
				down[c3samp[0].id] = false;
				if(!c3samp[0].pressure)
				{
					c3samp[0].x = 0;
					c3samp[0].y = 0;
					c3samp[0].finger = 0;
				}
				update_last_sample((int)c3samp[0].id, &OriSmp[0]);
	    	}
		}
	}
	return ;
}
