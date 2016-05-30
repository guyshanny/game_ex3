#pragma once

#include <glm\glm.hpp>
#include "OpenMeshObject.h"
#include "Camera.h"

class SpaceShip : public OpenMeshObject
{
private:
	float _angleYaw; 
	float _anglePitch;

	// Commands
	bool _commands[NUM_OF_COMMANDS];
	void _pitchLogic(float speed, Commands comm);
	void _yawLogic(float speed, Commands comm);
	void _moveForwordLogic();
	void _updateModel();
public:
	SpaceShip(const glm::vec3 pos, const glm::vec4 & color, const char* vShaderFile, const char* fShaderFile, 
				const char* textureIMG, const char*  meshPath);
	~SpaceShip();

	void draw(const glm::mat4& projection, glm::vec3 lightPos, glm::vec4 lightColor);
	void update();
	void init();

	// Key functions
	void turnUp();
	void turnDown();
	void turnLeft();
	void turnRight();
	void moveForword();
};

