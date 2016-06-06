#pragma once

#include "Billboard.h"

class Asteroids
{
public:
	Asteroids(const char* textureIMG);
	~Asteroids();
	void init();
	void render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp);

private:
	void createPositionBuffer();

	GLuint m_VB;
	Billboard m_billboard;
};

