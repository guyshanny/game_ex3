#pragma once

#include <glm\glm.hpp>
#include "globals.h"

class Camera
{
public:
	~Camera();
	void init(glm::vec3 pos);
	void update();

	const glm::mat4 getViewMatrix() const { return _view; }
	const glm::vec3 getPosition() const { return _position; }

	static Camera* instance()
	{
		if (nullptr == _instance)
		{
			_instance = new Camera();
		}
		return _instance;
	}

private:
	glm::vec3 _position;
	glm::mat4 _view;

	Camera();
	static Camera* _instance;
};

