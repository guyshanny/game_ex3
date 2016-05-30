#pragma once

#include "Object.h"
#include <vector>

class Wall : public Object
{
public:
	enum Commands
	{
		FLICKER_LIGHT,
		UNDO_FLICKER_LIGHT,

		NUM_OF_COMMANDS = 2
	};

	Wall(const glm::vec3 pos,
		 const glm::vec4 color,
		 const char* vShaderFile,
		 const char* fShaderFile,
		 const float& width,
		 const float& height,
		 const char* textureIMG);

	void draw(const glm::mat4 & projection, glm::vec3 lightPos, glm::vec4 lightColor);
	void update();
	void init();

	// Events
	void flickerLight(const bool& mode);

private:
	bool _commands[Commands::NUM_OF_COMMANDS];
	float _flickerLightIndicator;
	float _width;
	float _height;

	void _flickerLight(const bool& mode);
};
