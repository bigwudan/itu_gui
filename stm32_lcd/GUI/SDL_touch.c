/* General event handling code for SDL */
#include "SDL_events.h"
#include "itu.h"
#include "SDL_touch.h"
#include <stdbool.h>

#define MAX_FINGER_NUM 1

extern struct ts_sample touch_samp;
static struct ts_sample c3samp[MAX_FINGER_NUM];

int ts_read(void *ts, struct ts_sample *samp, int nr)
{
	int result;
	
	//memcpy(samp, &touch_samp, sizeof(struct ts_sample));
	//printf("");
	
	printf("x=%d,y=%d,press=%d\n",touch_samp.x, touch_samp.y, touch_samp.pressure );
	
	
	return result;

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
	return ;
}
