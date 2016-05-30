#pragma once

#include <glm\glm.hpp>
#include "globals.h"

class Camera
{
public:
	~Camera();
	void init();
	void update();

	const glm::mat4 getViewMatrix() const { return _view; }
	const glm::vec3 getPosition() const { return _position; }
	const glm::vec3 getUp() const { return glm::normalize(_up); }
	const glm::vec3 getPitchAxis() const { return glm::cross(glm::normalize(_direction), glm::normalize(_up)); }

	// Key functions
	void turnUp();
	void turnDown();
	void turnLeft();
	void turnRight();
	void moveForword();

	static Camera* instance()
	{
		if (nullptr == _instance)
		{
			_instance = new Camera();
		}
		return _instance;
	}

private:
	glm::vec3 _direction;
	glm::vec3 _position;
	glm::vec3 _up;
	glm::mat4 _view;

	Camera();
	static Camera* _instance;
	glm::mat4 Camera::_calculateViewMatrix();

	// Commands
	bool _commands[NUM_OF_COMMANDS];
	void _turnUpLogic();
	void _turnDownLogic();
	void _turnLeftLogic();
	void _turnRightLogic();
	void _moveForwordLogic();
};

