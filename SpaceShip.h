#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\vector_angle.hpp>
#include "OpenMeshObject.h"
#include "globals.h"

#define TURN_SPEED (0.1f)
#define MOVE_SPEED (2.0f)

class SpaceShip : public OpenMeshObject
{
public:
	enum Commands
	{
		TURN_UP,
		TURN_DOWN,
		TURN_LEFT,
		TURN_RIGHT,
		MOVE_FORWORD,
		NUM_OF_COMMANDS = 5
	};

	SpaceShip(const glm::vec3 pos, const glm::vec4 & color, const char* vShaderFile, const char* fShaderFile,
		const char* textureIMG, const char*  meshPath);
	~SpaceShip();

	void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor);
	void update();
	void init();
	const glm::vec3 getUp() const;
	const glm::vec3 getDirection() const;
	glm::vec3& getPosition() { return _position; }

	// Key functions
	void turnUp();
	void turnDown();
	void turnLeft();
	void turnRight();
	void moveForword();
	void reset();
private:
	glm::vec3 _up;
	glm::vec3 _front;
	glm::vec3 _initialPos;

	glm::vec3 getRight();

	// Commands
	bool _commands[NUM_OF_COMMANDS];
	void _pitchLogic(float speed, Commands comm);
	void _yawLogic(float speed, Commands comm);
	void _moveForwordLogic(const float& speed);
};

