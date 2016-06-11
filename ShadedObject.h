#pragma once

#include <list>
#include <gl\glew.h>

#define TEXTURE_SAMPLER "gTextureSampler"

class ShadedObject
{
public:
	ShadedObject(const char* textureIMG);
	virtual ~ShadedObject();
	virtual void init();

protected:
	void addShader(GLenum ShaderType, const char* pFilename);
	void finalize();
	GLint getUniformLocation(const char* pUniformName);
	GLint getProgramParam(GLint param);
	GLuint _programID; 
	GLuint _textureID;
	const std::string _textureImg;
	static GLuint initTexture(const char* fName);

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

