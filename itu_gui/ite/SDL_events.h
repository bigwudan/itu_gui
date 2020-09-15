#ifndef _SDL_events_h
#define _SDL_events_h
#include <stdint.h>
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

	/**
	* \brief The types of events that can be delivered.
	*/
	typedef enum
	{
		SDL_FIRSTEVENT = 0,     /**< Unused (do not remove) */

		/* Application events */
		SDL_QUIT = 0x100, /**< User-requested quit */

		/* Window events */
		SDL_WINDOWEVENT = 0x200, /**< Window state change */
		SDL_SYSWMEVENT,             /**< System specific event */

		/* Keyboard events */
		SDL_KEYDOWN = 0x300, /**< Key pressed */
		SDL_KEYUP,                  /**< Key released */
		SDL_TEXTEDITING,            /**< Keyboard text editing (composition) */
		SDL_TEXTINPUT,              /**< Keyboard text input */

		/* Mouse events */
		SDL_MOUSEMOTION = 0x400, /**< Mouse moved */
		SDL_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
		SDL_MOUSEBUTTONUP,          /**< Mouse button released */
		SDL_MOUSEWHEEL,             /**< Mouse wheel motion */

		/* Tablet or multiple mice input device events */
		SDL_INPUTMOTION = 0x500, /**< Input moved */
		SDL_INPUTBUTTONDOWN,        /**< Input button pressed */
		SDL_INPUTBUTTONUP,          /**< Input button released */
		SDL_INPUTWHEEL,             /**< Input wheel motion */
		SDL_INPUTPROXIMITYIN,       /**< Input pen entered proximity */
		SDL_INPUTPROXIMITYOUT,      /**< Input pen left proximity */

		/* Joystick events */
		SDL_JOYAXISMOTION = 0x600, /**< Joystick axis motion */
		SDL_JOYBALLMOTION,          /**< Joystick trackball motion */
		SDL_JOYHATMOTION,           /**< Joystick hat position change */
		SDL_JOYBUTTONDOWN,          /**< Joystick button pressed */
		SDL_JOYBUTTONUP,            /**< Joystick button released */

		/* Touch events */
		SDL_FINGERDOWN = 0x700,
		SDL_FINGERUP,
		SDL_FINGERMOTION,
		SDL_TOUCHBUTTONDOWN,
		SDL_TOUCHBUTTONUP,

		/* Gesture events */
		SDL_DOLLARGESTURE = 0x800,
		SDL_DOLLARRECORD,
		SDL_MULTIGESTURE,
		SDL_SLIDEGESTURE,

		/* Clipboard events */
		SDL_CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */

		/* Drag and drop events */
		SDL_DROPFILE = 0x1000, /**< The system requests a file open */

		/** Events ::SDL_USEREVENT through ::SDL_LASTEVENT are for your use,
		*  and should be allocated with SDL_RegisterEvents()
		*/
		SDL_USEREVENT = 0x8000,

		/**
		*  This last event is only for bounding internal arrays
		*/
		SDL_LASTEVENT = 0xFFFF
	} SDL_EventType;

	/**
	*  \brief Window state change event data (event.window.*)
	*/
	typedef struct SDL_WindowEvent
	{
		uint32_t type;        /**< ::SDL_WINDOWEVENT */
		uint32_t timestamp;
		uint32_t windowID;    /**< The associated window */
		uint8_t event;        /**< ::SDL_WindowEventID */
		uint8_t padding1;
		uint8_t padding2;
		uint8_t padding3;
		int data1;          /**< event dependent data */
		int data2;          /**< event dependent data */
	} SDL_WindowEvent;

	/**
	*  \brief Keyboard button event structure (event.key.*)
	*/
	typedef struct SDL_KeyboardEvent
	{
		uint32_t type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with keyboard focus, if any */
		uint8_t state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
		uint8_t repeat;       /**< Non-zero if this is a key repeat */
		uint8_t padding2;
		uint8_t padding3;
		uint32_t keysym;  /**< The key that was pressed or released */
	} SDL_KeyboardEvent;

#define SDL_TEXTEDITINGEVENT_TEXT_SIZE (32)
	/**
	*  \brief Keyboard text editing event structure (event.edit.*)
	*/
	typedef struct SDL_TextEditingEvent
	{
		uint32_t type;                                /**< ::SDL_TEXTEDITING */
		uint32_t timestamp;
		uint32_t windowID;                            /**< The window with keyboard focus, if any */
		char text[SDL_TEXTEDITINGEVENT_TEXT_SIZE];  /**< The editing text */
		int start;                                  /**< The start cursor of selected editing text */
		int length;                                 /**< The length of selected editing text */
	} SDL_TextEditingEvent;


#define SDL_TEXTINPUTEVENT_TEXT_SIZE (32)
	/**
	*  \brief Keyboard text input event structure (event.text.*)
	*/
	typedef struct SDL_TextInputEvent
	{
		uint32_t type;                              /**< ::SDL_TEXTINPUT */
		uint32_t timestamp;
		uint32_t windowID;                          /**< The window with keyboard focus, if any */
		char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];  /**< The input text */
	} SDL_TextInputEvent;

	/**
	*  \brief Mouse motion event structure (event.motion.*)
	*/
	typedef struct SDL_MouseMotionEvent
	{
		uint32_t type;        /**< ::SDL_MOUSEMOTION */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint8_t state;        /**< The current button state */
		uint8_t padding1;
		uint8_t padding2;
		uint8_t padding3;
		int x;              /**< X coordinate, relative to window */
		int y;              /**< Y coordinate, relative to window */
		int xrel;           /**< The relative motion in the X direction */
		int yrel;           /**< The relative motion in the Y direction */
	} SDL_MouseMotionEvent;

	/**
	*  \brief Mouse button event structure (event.button.*)
	*/
	typedef struct SDL_MouseButtonEvent
	{
		uint32_t type;        /**< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint8_t button;       /**< The mouse button index */
		uint8_t state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
		uint8_t padding1;
		uint8_t padding2;
		int x;              /**< X coordinate, relative to window */
		int y;              /**< Y coordinate, relative to window */
	} SDL_MouseButtonEvent;

	/**
	*  \brief Mouse wheel event structure (event.wheel.*)
	*/
	typedef struct SDL_MouseWheelEvent
	{
		uint32_t type;        /**< ::SDL_MOUSEWHEEL */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		int x;              /**< The amount scrolled horizontally */
		int y;              /**< The amount scrolled vertically */
	} SDL_MouseWheelEvent;

	/**
	*  \brief Joystick axis motion event structure (event.jaxis.*)
	*/
	typedef struct SDL_JoyAxisEvent
	{
		uint32_t type;        /**< ::SDL_JOYAXISMOTION */
		uint32_t timestamp;
		uint8_t which;        /**< The joystick device index */
		uint8_t axis;         /**< The joystick axis index */
		uint8_t padding1;
		uint8_t padding2;
		int value;          /**< The axis value (range: -32768 to 32767) */
	} SDL_JoyAxisEvent;

	/**
	*  \brief Joystick trackball motion event structure (event.jball.*)
	*/
	typedef struct SDL_JoyBallEvent
	{
		uint32_t type;        /**< ::SDL_JOYBALLMOTION */
		uint32_t timestamp;
		uint8_t which;        /**< The joystick device index */
		uint8_t ball;         /**< The joystick trackball index */
		uint8_t padding1;
		uint8_t padding2;
		int xrel;           /**< The relative motion in the X direction */
		int yrel;           /**< The relative motion in the Y direction */
	} SDL_JoyBallEvent;

	/**
	*  \brief Joystick hat position change event structure (event.jhat.*)
	*/
	typedef struct SDL_JoyHatEvent
	{
		uint32_t type;        /**< ::SDL_JOYHATMOTION */
		uint32_t timestamp;
		uint8_t which;        /**< The joystick device index */
		uint8_t hat;          /**< The joystick hat index */
		uint8_t value;        /**< The hat position value.
							*   \sa ::SDL_HAT_LEFTUP ::SDL_HAT_UP ::SDL_HAT_RIGHTUP
							*   \sa ::SDL_HAT_LEFT ::SDL_HAT_CENTERED ::SDL_HAT_RIGHT
							*   \sa ::SDL_HAT_LEFTDOWN ::SDL_HAT_DOWN ::SDL_HAT_RIGHTDOWN
							*
							*   Note that zero means the POV is centered.
							*/
		uint8_t padding1;
	} SDL_JoyHatEvent;

	/**
	*  \brief Joystick button event structure (event.jbutton.*)
	*/
	typedef struct SDL_JoyButtonEvent
	{
		uint32_t type;        /**< ::SDL_JOYBUTTONDOWN or ::SDL_JOYBUTTONUP */
		uint32_t timestamp;
		uint8_t which;        /**< The joystick device index */
		uint8_t button;       /**< The joystick button index */
		uint8_t state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
		uint8_t padding1;
	} SDL_JoyButtonEvent;


	/**
	*  \brief Touch finger motion/finger event structure (event.tfinger.*)
	*/
	typedef struct SDL_TouchFingerEvent
	{
		uint32_t type;        /**< ::SDL_FINGERMOTION OR
							SDL_FINGERDOWN OR SDL_FINGERUP*/
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint32_t touchId;        /**< The touch device id */
		uint32_t fingerId;
		uint8_t state;        /**< The current button state */
		uint8_t padding1;
		uint8_t padding2;
		uint8_t padding3;
		uint16_t x;
		uint16_t y;
		int16_t dx;
		int16_t dy;
		uint16_t pressure;
		uint16_t numFingers;
	} SDL_TouchFingerEvent;


	/**
	*  \brief Touch finger motion/finger event structure (event.tbutton.*)
	*/
	typedef struct SDL_TouchButtonEvent
	{
		uint32_t type;        /**< ::SDL_TOUCHBUTTONUP OR SDL_TOUCHBUTTONDOWN */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint32_t touchId;        /**< The touch device index */
		uint8_t state;        /**< The current button state */
		uint8_t button;        /**< The button changing state */
		uint8_t padding1;
		uint8_t padding2;
	} SDL_TouchButtonEvent;


	/**
	*  \brief Multiple Finger Gesture Event (event.mgesture.*)
	*/
	typedef struct SDL_MultiGestureEvent
	{
		uint32_t type;        /**< ::SDL_MULTIGESTURE */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint32_t touchId;        /**< The touch device index */
		float dTheta;
		float dDist;
		float x;  //currently 0...1. Change to screen coords?
		float y;
		uint16_t numFingers;
		uint16_t padding;
	} SDL_MultiGestureEvent;

	/* (event.dgesture.*) */
	typedef struct SDL_DollarGestureEvent
	{
		uint32_t type;        /**< ::SDL_DOLLARGESTURE */
		uint32_t timestamp;
		uint32_t windowID;    /**< The window with mouse focus, if any */
		uint32_t touchId;        /**< The touch device index */
		uint32_t gestureId;
		uint32_t numFingers;
		float error;
		uint32_t x;  //currently 0...1. Change to screen coords?
		uint32_t y;
	} SDL_DollarGestureEvent;


	/**
	*  \brief An event used to request a file open by the system (event.drop.*)
	*         This event is disabled by default, you can enable it with SDL_EventState()
	*  \note If you enable this event, you must free the filename in the event.
	*/
	typedef struct SDL_DropEvent
	{
		uint32_t type;        /**< ::SDL_DROPFILE */
		uint32_t timestamp;
		char *file;         /**< The file name, which should be freed with SDL_free() */
	} SDL_DropEvent;


	/**
	*  \brief The "quit requested" event
	*/
	typedef struct SDL_QuitEvent
	{
		uint32_t type;        /**< ::SDL_QUIT */
		uint32_t timestamp;
	} SDL_QuitEvent;


	/**
	*  \brief A user-defined event type (event.user.*)
	*/
	typedef struct SDL_UserEvent
	{
		uint32_t type;        /**< ::SDL_USEREVENT through ::SDL_NUMEVENTS-1 */
		uint32_t timestamp;
		uint32_t windowID;    /**< The associated window if any */
		int code;           /**< User defined event code */
		void *data1;        /**< User defined data pointer */
		void *data2;        /**< User defined data pointer */
	} SDL_UserEvent;


	struct SDL_SysWMmsg;
	typedef struct SDL_SysWMmsg SDL_SysWMmsg;

	/**
	*  \brief A video driver dependent system event (event.syswm.*)
	*         This event is disabled by default, you can enable it with SDL_EventState()
	*
	*  \note If you want to use this event, you should include SDL_syswm.h.
	*/
	typedef struct SDL_SysWMEvent
	{
		uint32_t type;        /**< ::SDL_SYSWMEVENT */
		uint32_t timestamp;
		SDL_SysWMmsg *msg;  /**< driver dependent data, defined in SDL_syswm.h */
	} SDL_SysWMEvent;

typedef union SDL_Event
{
	uint32_t type;                    /**< Event type, shared with all events */
	SDL_WindowEvent window;         /**< Window event data */
	SDL_KeyboardEvent key;          /**< Keyboard event data */
	SDL_TextEditingEvent edit;      /**< Text editing event data */
	SDL_TextInputEvent text;        /**< Text input event data */
	SDL_MouseMotionEvent motion;    /**< Mouse motion event data */
	SDL_MouseButtonEvent button;    /**< Mouse button event data */
	SDL_MouseWheelEvent wheel;      /**< Mouse wheel event data */
	SDL_JoyAxisEvent jaxis;         /**< Joystick axis event data */
	SDL_JoyBallEvent jball;         /**< Joystick ball event data */
	SDL_JoyHatEvent jhat;           /**< Joystick hat event data */
	SDL_JoyButtonEvent jbutton;     /**< Joystick button event data */
	SDL_QuitEvent quit;             /**< Quit request event data */
	SDL_UserEvent user;             /**< Custom event data */
	SDL_SysWMEvent syswm;           /**< System dependent window event data */
	SDL_TouchFingerEvent tfinger;   /**< Touch finger event data */
	SDL_TouchButtonEvent tbutton;   /**< Touch button event data */
	SDL_MultiGestureEvent mgesture; /**< Multi Finger Gesture data */
	SDL_DollarGestureEvent dgesture; /**< Multi Finger Gesture data */
	SDL_DropEvent drop;             /**< Drag and drop event data */
} SDL_Event;

typedef int SDL_mutex;



/**
*  These are the various supported windowing subsystems
*/
typedef enum
{
	SDL_SYSWM_UNKNOWN,
	SDL_SYSWM_WINDOWS,
	SDL_SYSWM_X11,
	SDL_SYSWM_DIRECTFB,
	SDL_SYSWM_COCOA,
	SDL_SYSWM_UIKIT,
	SDL_SYSWM_CASTOR3
} SDL_SYSWM_TYPE;

/**
*  The custom event structure.
*/
struct SDL_SysWMmsg
{
	uint32_t version;
	SDL_SYSWM_TYPE subsystem;
	union
	{
		/* Can't have an empty union */
		int dummy;
	} msg;
};

typedef int (* SDL_EventFilter) (void *userdata, SDL_Event * event);

/*@{*/
#define SDL_QUERY	-1
#define SDL_IGNORE	 0
#define SDL_DISABLE	 0
#define SDL_ENABLE	 1

/*@{*/
typedef enum
{
	SDL_ADDEVENT,
	SDL_PEEKEVENT,
	SDL_GETEVENT
} SDL_eventaction;

/*
启动事件循环
*/
int SDL_StartEventLoop(void);

/*
取出一个事件
*/
int SDL_PollEvent(SDL_Event * event);

//推入一个事件
int SDL_PushEvent(SDL_Event * event);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif


#endif /* _SDL_timer_h */

/* vi: set ts=4 sw=4 expandtab: */
