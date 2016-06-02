#pragma once

#define PROGRAM_DEFAULT_ID			"default"
#define BEGIN_OPENGL				glUseProgram(_programID)
#define END_OPENGL					glUseProgram(0)
#define TURN_SPEED					(0.1f)
#define INVALID_OGL_VALUE			0xffffffff
#define INVALID_UNIFORM_LOCATION	0xffffffff

enum Commands
{
	TURN_UP,
	TURN_DOWN,
	TURN_LEFT,
	TURN_RIGHT,
	MOVE_FORWORD,
	NUM_OF_COMMANDS = 5
};