#pragma once

#define PROGRAM_DEFAULT_ID			"default"
#define BEGIN_OPENGL				glUseProgram(_programID)
#define END_OPENGL					glUseProgram(0)
#define INVALID_OGL_VALUE			0xffffffff
#define INVALID_UNIFORM_LOCATION	0xffffffff


namespace Controls
{
	enum KeyControls
	{
		KEY_ESC = (27),
		KEY_RESET = ('r'),
		KEY_RELOAD = ('l'),

		// Ship (camera) controls
		KEY_UP = ('w'),
		KEY_DOWN = ('s'),
		KEY_LEFT = ('a'),
		KEY_RIGHT = ('d'),
		KEY_MOVE_FORWARD = (' '),
		KEY_TEST = ('t')
	};
}