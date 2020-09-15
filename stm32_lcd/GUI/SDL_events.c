/* General event handling code for SDL */
#include "SDL_events.h"
#include "itu.h"
#define SDL_arraysize(array)	(sizeof(array)/sizeof(array[0]))

typedef struct {
	uint32_t bits[8];
} SDL_DisabledEventBlock;

static SDL_DisabledEventBlock *SDL_disabled_events[256];
void *SDL_EventOKParam;
static uint32_t SDL_userevents = SDL_USEREVENT;
SDL_EventFilter SDL_EventOK = NULL;
typedef struct SDL_EventWatcher {
	SDL_EventFilter callback;
	void *userdata;
	struct SDL_EventWatcher *next;
} SDL_EventWatcher;

static SDL_EventWatcher *SDL_event_watchers = NULL;
/* Private data -- event queue */
#define MAXEVENTS	40
static struct
{
	SDL_mutex *lock;
	int active;
	int head;
	int tail;
	SDL_Event event[MAXEVENTS];
	int wmmsg_next;
	struct SDL_SysWMmsg wmmsg[MAXEVENTS];
} SDL_EventQ;


/*停止事件循环*/
void
SDL_StopEventLoop(void)
{
	int i;

	if (SDL_EventQ.lock) {
	}

	/* Clean out EventQ */
	SDL_EventQ.head = 0;
	SDL_EventQ.tail = 0;
	SDL_EventQ.wmmsg_next = 0;

	/* Clear disabled event state */
	for (i = 0; i < SDL_arraysize(SDL_disabled_events); ++i) {
		if (SDL_disabled_events[i]) {
			free(SDL_disabled_events[i]);
			SDL_disabled_events[i] = NULL;
		}
	}

	while (SDL_event_watchers) {
		SDL_EventWatcher *tmp = SDL_event_watchers;
		SDL_event_watchers = tmp->next;
		free(tmp);
	}
}

/* Cut an event, and return the next valid spot, or the tail */
/*                           -- called with the queue locked */
static int
SDL_CutEvent(int spot)
{
	if (spot == SDL_EventQ.head) {
		SDL_EventQ.head = (SDL_EventQ.head + 1) % MAXEVENTS;
		return (SDL_EventQ.head);
	}
	else if ((spot + 1) % MAXEVENTS == SDL_EventQ.tail) {
		SDL_EventQ.tail = spot;
		return (SDL_EventQ.tail);
	}
	else
		/* We cut the middle -- shift everything over */
	{
		int here, next;

		/* This can probably be optimized with SDL_memcpy() -- careful! */
		if (--SDL_EventQ.tail < 0) {
			SDL_EventQ.tail = MAXEVENTS - 1;
		}
		for (here = spot; here != SDL_EventQ.tail; here = next) {
			next = (here + 1) % MAXEVENTS;
			SDL_EventQ.event[here] = SDL_EventQ.event[next];
		}
		return (spot);
	}
	/* NOTREACHED */
}

void
SDL_FlushEvents(uint32_t minType, uint32_t maxType)
{
	/* Don't look after we've quit */
	if (!SDL_EventQ.active) {
		return;
	}
	int spot = SDL_EventQ.head;
	while (spot != SDL_EventQ.tail) {
		uint32_t type = SDL_EventQ.event[spot].type;
		if (minType <= type && type <= maxType) {
			spot = SDL_CutEvent(spot);
		}
		else {
			spot = (spot + 1) % MAXEVENTS;
		}
	}
}

void
SDL_FlushEvent(uint32_t type)
{
	SDL_FlushEvents(type, type);
}

uint8_t
SDL_EventState(uint32_t type, int state)
{
	uint8_t current_state;
	uint8_t hi = ((type >> 8) & 0xff);
	uint8_t lo = (type & 0xff);

	if (SDL_disabled_events[hi] &&
		(SDL_disabled_events[hi]->bits[lo / 32] & (1 << (lo & 31)))) {
		current_state = SDL_DISABLE;
	}
	else {
		current_state = SDL_ENABLE;
	}

	if (state != current_state)
	{
		switch (state) {
		case SDL_DISABLE:
			/* Disable this event type and discard pending events */
			if (!SDL_disabled_events[hi]) {
				SDL_disabled_events[hi] = (SDL_DisabledEventBlock*)calloc(1, sizeof(SDL_DisabledEventBlock));
				if (!SDL_disabled_events[hi]) {
					/* Out of memory, nothing we can do... */
					break;
				}
			}
			SDL_disabled_events[hi]->bits[lo / 32] |= (1 << (lo & 31));
			SDL_FlushEvent(type);
			break;
		case SDL_ENABLE:
			SDL_disabled_events[hi]->bits[lo / 32] &= ~(1 << (lo & 31));
			break;
		default:
			/* Querying state... */
			break;
		}
	}

	return current_state;
}

/* This function (and associated calls) may be called more than once */
int
SDL_StartEventLoop(void)
{
	/* Clean out the event queue */
	SDL_EventQ.lock = NULL;
	SDL_StopEventLoop();

	/* No filter to start with, process most event types */
	SDL_EventOK = NULL;
	SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_DISABLE);

	/* Create the lock and set ourselves active */
	SDL_EventQ.active = 1;

	return (0);
}

/* Add an event to the event queue -- called with the queue locked */
static int
SDL_AddEvent(SDL_Event * event)
{
	int tail, added;

	tail = (SDL_EventQ.tail + 1) % MAXEVENTS;
	if (tail == SDL_EventQ.head) {
		/* Overflow, drop event */
		added = 0;
	}
	else {
		SDL_EventQ.event[SDL_EventQ.tail] = *event;
		if (event->type == SDL_SYSWMEVENT) {
			/* Note that it's possible to lose an event */
			int next = SDL_EventQ.wmmsg_next;
			SDL_EventQ.wmmsg[next] = *event->syswm.msg;
			SDL_EventQ.event[SDL_EventQ.tail].syswm.msg =
				&SDL_EventQ.wmmsg[next];
			SDL_EventQ.wmmsg_next = (next + 1) % MAXEVENTS;
		}
		SDL_EventQ.tail = tail;
		added = 1;
	}
	return (added);
}


/* Lock the event queue, take a peep at it, and unlock it */
int
SDL_PeepEvents(SDL_Event * events, int numevents, SDL_eventaction action,
uint32_t minType, uint32_t maxType)
{
	int i, used;

	/* Don't look after we've quit */
	if (!SDL_EventQ.active) {
		return (-1);
	}
	/* Lock the event queue */
	used = 0;
	if (action == SDL_ADDEVENT) {
		for (i = 0; i < numevents; ++i) {
			used += SDL_AddEvent(&events[i]);
		}
	}
	else {
		SDL_Event tmpevent;
		int spot;

		/* If 'events' is NULL, just see if they exist */
		if (events == NULL) {
			action = SDL_PEEKEVENT;
			numevents = 1;
			events = &tmpevent;
		}
		spot = SDL_EventQ.head;
		while ((used < numevents) && (spot != SDL_EventQ.tail)) {
			uint32_t type = SDL_EventQ.event[spot].type;
			if (minType <= type && type <= maxType) {
				events[used++] = SDL_EventQ.event[spot];
				if (action == SDL_GETEVENT) {
					spot = SDL_CutEvent(spot);
				}
				else {
					spot = (spot + 1) % MAXEVENTS;
				}
			}
			else {
				spot = (spot + 1) % MAXEVENTS;
			}
		}
	}
	return (used);
}

bool
SDL_HasEvent(uint32_t type)
{
	return (SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, type, type) > 0);
}

bool
SDL_HasEvents(uint32_t minType, uint32_t maxType)
{
	return (SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, minType, maxType) > 0);
}


/* Run the system dependent event loops */
void
SDL_PumpEvents(void)
{

}
void SDL_Delay(uint32_t len)
{

}
int
SDL_WaitEventTimeout(SDL_Event * event, int timeout)
{
	uint32_t expiration = 0;

	if (timeout > 0)
		expiration = SDL_GetTicks() + timeout;

	for (;;) {
		SDL_PumpEvents();
		switch (SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
		case -1:
			return 0;
		case 1:
			return 1;
		case 0:
			if (timeout == 0) {
				/* Polling and no events, just return */
				return 0;
			}
			if (timeout > 0 && ((int)(SDL_GetTicks() - expiration) >= 0)) {
				/* Timeout expired and no events */
				return 0;
			}
			SDL_Delay(10);
			break;
		}
	}
}




int
SDL_PollEvent(SDL_Event * event)
{
	return SDL_WaitEventTimeout(event, 0);
}

int
SDL_WaitEvent(SDL_Event * event)
{
	return SDL_WaitEventTimeout(event, -1);
}

int
SDL_PushEvent(SDL_Event * event)
{
	SDL_EventWatcher *curr;
	event->window.timestamp = SDL_GetTicks();
	if (SDL_EventOK && !SDL_EventOK(SDL_EventOKParam, event)) {
		return 0;
	}

	for (curr = SDL_event_watchers; curr; curr = curr->next) {
		curr->callback(curr->userdata, event);
	}

	if (SDL_PeepEvents(event, 1, SDL_ADDEVENT, 0, 0) <= 0) {
		return -1;
	}
	return 1;
}

uint32_t
SDL_RegisterEvents(int numevents)
{
	uint32_t event_base;

	if (SDL_userevents + numevents <= SDL_LASTEVENT) {
		event_base = SDL_userevents;
		SDL_userevents += numevents;
	}
	else {
		event_base = (uint32_t)-1;
	}
	return event_base;
}