#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <queue>
#include <iostream>
#include "Object.h"
#include "SpaceShip.h"
#include "AsteroidsField.h"
#include "SkyBox.h"

class Camera {
private:
	SpaceShip* _player;
	glm::mat4 _view;
	glm::vec3 _pos;
	glm::vec3 _offset;
	glm::vec3 _direction;
	glm::vec3 _up;

	glm::vec3 interp(glm::vec3 cur, glm::vec3 final, float dt) {
		glm::vec3 velocity = final - cur;
		glm::clamp(velocity, glm::vec3(-0.9), glm::vec3(0.9));
		glm::vec3 delta = velocity * dt;
		if (glm::length(delta) < glm::distance(final, cur)) {
			return cur + (velocity * dt);
		}
		else {
			return final;
		}
	}

public:
	Camera(SpaceShip* player, const glm::vec3& offset) : _player(player), _offset(offset), _direction(0, 0, -1), _up(0, 1, 0) { }

	void update(int deltaTime) {
		float dt = deltaTime * 0.001f;
		//update position
		_pos = interp(_pos, glm::vec3(_player->getModel() * glm::vec4(_offset, 1)), dt);
		_direction = interp(_direction, _player->getDirection(), dt);
		//update view matrix
		glm::vec3 target = _pos + _direction;
		_up = interp(_up, _player->getUp(), dt);
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
	void update(int deltaTime);
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

	// World objects
	SpaceShip _spaceship;
	AsteroidsField _asteroids;
	SkyBox _skybox;
	bool _isPlayerAlive;

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	void _updateCameraDependencies();
	void _updateLivesText(const char* text);
};

