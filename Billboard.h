#pragma once

#include <glm/glm.hpp> //vec3, mat4...
#include "ShadedObject.h"

class Billboard : public ShadedObject
{
public:
	Billboard(const char* textureIMG);

	virtual void init();

	void setVP(const glm::mat4& VP);
	void setCameraPosition(const glm::vec3& pos);
	void setColorTextureUnit(unsigned int textureUnit);
	void setUpVector(const glm::vec3& up);
	void enable();
	void bindTexture();

private:
	GLuint m_VPLocation;
	GLuint m_cameraPosLocation;
	GLuint m_colorMapLocation;
	GLuint m_upLocation;
};

