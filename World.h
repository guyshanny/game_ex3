#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Camera.h"
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

	//Light properties
	glm::vec3 _lightPos;
	glm::vec4 _lightColor;

	Object* _static;
	SpaceShip _spaceship;
	Asteroids _asteroids;
	Camera* _camera;
	// View port frame:
	float _width, _height, _offsetX, _offsetY;
};

