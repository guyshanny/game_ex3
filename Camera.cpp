#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> // lookAt

Camera* Camera::_instance = nullptr;

Camera::Camera() : _direction(0, 0, -30), _position(0, 0, 0), _up(0, 1, 0) {
	for (int comm = 0; comm != NUM_OF_COMMANDS; comm++) {
		Commands command = static_cast<Commands>(comm);
		_commands[command] = false;
	}
}

Camera::~Camera()
{
}

void Camera::init()
{
	_calculateViewMatrix();
}

void Camera::update()
{
	/************************************************************************/
	/* All regular controls                                                 */
	/************************************************************************/
	if (_commands[TURN_UP])			{ _turnUpLogic(); }
	if (_commands[TURN_DOWN])		{ _turnDownLogic(); }
	if (_commands[TURN_RIGHT])		{ _turnRightLogic(); }
	if (_commands[TURN_LEFT])		{ _turnLeftLogic(); }
	if (_commands[MOVE_FORWORD])	{ _moveForwordLogic(); }
}

glm::mat4 Camera::_calculateViewMatrix()
{
	_view = lookAt(_position, _position + _direction, _up);
	return _view;
}

void Camera::_turnUpLogic()
{
	_commands[TURN_UP] = false;

	glm::vec3 axis = glm::cross(_direction, _up);
	glm::mat4 rotation = glm::rotate(glm::mat4(1), TURN_SPEED, axis);
	_direction = glm::vec3(rotation*glm::vec4(_direction, 1));
	_up = glm::vec3(rotation*glm::vec4(_up, 1));
	_calculateViewMatrix();

	std::cout << "turning up" << std::endl;
}

void Camera::_turnDownLogic()
{
	_commands[TURN_DOWN] = false;

	glm::vec3 axis = glm::cross(_direction, _up);
	glm::mat4 rotation = glm::rotate(glm::mat4(1), -TURN_SPEED, axis);
	_direction = glm::vec3(rotation*glm::vec4(_direction, 1));
	_up = glm::vec3(rotation*glm::vec4(_up, 1));
	_calculateViewMatrix();

	std::cout << "turning down" << std::endl;
}

void Camera::_turnRightLogic()
{
	_commands[TURN_RIGHT] = false;

	glm::mat4 rotation = glm::rotate(glm::mat4(1), -TURN_SPEED, _up);
	_direction = glm::vec3(rotation*glm::vec4(_direction, 1));

	_calculateViewMatrix();
	std::cout << "turning right" << std::endl;
}

void Camera::_turnLeftLogic()
{
	_commands[TURN_LEFT] = false;

	glm::mat4 rotation = glm::rotate(glm::mat4(1), TURN_SPEED, _up);
	_direction = glm::vec3(rotation*glm::vec4(_direction, 1));

	_calculateViewMatrix();
	std::cout << "turning left" << std::endl;
}

void Camera::_moveForwordLogic()
{
	_commands[MOVE_FORWORD] = false;
	//TODO: move position by speed*direction
	//_calculateViewMatrix();
	std::cout << "moving forword" << std::endl;
}

void Camera::turnUp()		{ _commands[TURN_UP] = true; }
void Camera::turnDown()		{ _commands[TURN_DOWN] = true; }
void Camera::turnLeft()		{ _commands[TURN_LEFT] = true; }
void Camera::turnRight()	{ _commands[TURN_RIGHT] = true; }
void Camera::moveForword()	{ _commands[MOVE_FORWORD] = true; }

