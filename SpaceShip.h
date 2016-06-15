#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\vector_angle.hpp>
#include "OpenMeshObject.h"
#include "globals.h"

#define TURN_SPEED (10.f)	//per second, not frame
#define MOVE_SPEED (100.f)	//per second, not frame
#define MAX_LIFE 5

class SpaceShip : public OpenMeshObject
{
public:
	enum LIFE_OPT
	{
		ALIVE,
		DEAD
	};

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
	GLuint update(int deltaTime);
	void init();
	const glm::vec3& getUp() const			{ return _up; }
	const glm::vec3& getDirection() const	{ return _front; }
	const glm::vec3& getPosition() const	{ return _position; }
	void collide() { _life -= 1; }
	GLuint getLife() { return _life; }

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
	GLuint _life;

	glm::vec3 getRight();

	// Commands
	bool _commands[NUM_OF_COMMANDS];
	void _pitchLogic(float speed, Commands comm);
	void _yawLogic(float speed, Commands comm);
	void _moveForwordLogic(const float& speed);
};

