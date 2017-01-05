#pragma once

// AppGlobals.h
// Values that are used throughout the app. Not actually global, ironically.
// I feel like there has to be an obvious, much better way to do this.

namespace AppGlobals {
	enum EVENTCODES {
		APP_NO_ERROR = 0,
		APP_GL_INIT_FAIL = 1,
		APP_GL_WINDOW_CREATE_FAIL = 2,
		APP_GL_LOAD_FUNCTIONS_FAIL = 3,
		APP_CLOSED = 4,
	};
	
	extern float deltaTime;
}