#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Object.h"
#include "SpaceShip.h"
#include "Asteroids.h"

class World
{
public:
	World();
	~World();
	void init();
	void draw();
	void update();
	void resize(int width, int height);

	// Keyboard handling
	void upKeyPressed();
	void downKeyPressed();
	void rightKeyPressed();
	void leftKeyPressed();

private:
	// MVP matrices
	glm::mat4 _projection;

	const glm::vec3 _camOffset;

	//Light properties
	glm::vec4 _lightColor;

	Object* _static;
	SpaceShip _spaceship;
	Asteroids _asteroids;
	// View port frame:
	float _width, _height, _offsetX, _offsetY;
	
	glm::mat4 getViewMat();
	glm::vec3 getCamPos();
};

