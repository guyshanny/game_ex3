#pragma once

#include "Billboard.h"

class Asteroids
{
public:
	Asteroids(const char* textureIMG);
	~Asteroids();
	void init();
	void render(const glm::mat4& VP, const glm::vec3& CameraPos);

private:
	void createPositionBuffer();

	GLuint m_VB;
	Billboard m_billboard;
};

