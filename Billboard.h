#pragma once

#include <glm/glm.hpp>
#include "ShadedObject.h"
#include <vector>

#define MAX_TEXTURES 8

class Billboard : public ShadedObject
{
public:
	Billboard(const char* textureIMG);
	Billboard(const std::vector<char*> textures);

	virtual void init();

	void setVP(const glm::mat4& projection, const glm::mat4& view);
	void setCameraPosition(const glm::vec3& pos);
	void setUpVector(const glm::vec3& up);
	void enable();
	GLuint* getTexturesHandles() { return _texturesHandles; }

private:
	GLuint m_viewLocation;
	GLuint m_projectionLocation;
	GLuint m_cameraPosLocation;
	GLuint m_textureLocation[MAX_TEXTURES];
	GLuint m_upLocation;
	std::vector<char*> _textures;
	GLuint _texturesHandles[MAX_TEXTURES];

	void _initTextures();
};

