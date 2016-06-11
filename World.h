#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Object.h"
#include "SpaceShip.h"
#include "AsteroidsField.h"
#include "Asteroids.h"
#include "SkyBox.h"

class Camera {
private:
	SpaceShip* _player;
	glm::mat4 _view;
	glm::vec3 _pos;
	glm::vec3 _offset;

public:
	Camera(SpaceShip* player, const glm::vec3& offset) : _player(player), _offset(offset) {	}

	void update() {
		//update position
		_pos = glm::vec3(_player->getModel() * glm::vec4(_offset, 1));
		//update view matrix
		glm::vec3 dir = _player->getDirection();
		glm::vec3 target = _pos + dir;
		_view = glm::lookAt(_pos, target, _player->getUp());
	}
	const glm::vec3 getPos() const {
		return _pos;
	}
	const glm::mat4 getView() const {
		return _view;
	}
};

class World
{
public:
	World();
	~World();
	void init();
	void draw();
	void update();
	void resize(int width, int height);
	void reset();

	// Keyboard handling
	void upKeyPressed();
	void downKeyPressed();
	void rightKeyPressed();
	void leftKeyPressed();
	void moveForwardKeyPressed();

private:
	// MVP matrices
	glm::mat4 _projection;

	Camera _camera;

	//Light properties
	glm::vec4 _lightColor;

	SpaceShip _spaceship;
	AsteroidsField _asteroids;
	Asteroids _asteroids;
	SkyBox _skybox;
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	void _updateCameraDependencies();
};

