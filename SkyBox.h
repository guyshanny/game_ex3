#pragma once

#include <glm\glm.hpp>
#include "OpenMeshObject.h"
#include "CubemapTexture.h"

class SkyBox : public OpenMeshObject
{
public:
	SkyBox();
	
	void init();
	void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor);
	void update();

private:
	GLuint m_WVPLocation;
	GLuint m_textureLocation;
	CubemapTexture m_pCubemapTex;
};

