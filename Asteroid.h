#pragma once

#include "Object.h"

class Asteroid : public Object 
{
public:
	Asteroid(const char* vShaderFile,
			 const char* fShaderFile,
	 		 const char* textureIMG);

// 	Asteroid(const Asteroid& other);

	bool operator<(const Asteroid& other) const
	{
		return camDist > other.camDist;
	}

	// Getters & Setters
	void setPosition(const glm::vec3& pos) { _position = pos; }
	glm::vec3 getPosition() { return _position; }
	void setSpeed(const glm::vec3 speed) { _speed = speed; }
	glm::vec3 getSpeed() { return _speed; }

	virtual void update() {}
	virtual void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor) {}

	GLuint id;
	GLfloat size;
	bool isAlive;
	GLfloat radius;
	GLuint type;
	GLfloat camDist;
};