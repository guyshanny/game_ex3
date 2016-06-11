#pragma once

#include <string>
#include <GL/glew.h>

class CubemapTexture
{
public:
	CubemapTexture(const std::string& directory,
		const std::string& posXFilename,
		const std::string& negXFilename,
		const std::string& posYFilename,
		const std::string& negYFilename,
		const std::string& posZFilename,
		const std::string& negZFilename);

	~CubemapTexture();
	void load();
	void bind(GLenum textureUnit);

private:
	std::string m_fileNames[6];
	GLuint m_textureObj;
};

