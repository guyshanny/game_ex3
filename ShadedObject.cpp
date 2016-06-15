#include <iostream>
#include "ShadedObject.h"
#include "globals.h"
#include "bimage.h"

ShadedObject::ShadedObject(const char* textureIMG) : _programID(0), _textureImg(textureIMG)
{
}

ShadedObject::~ShadedObject()
{
	// Delete the intermediate shader objects that have been added to the program
	// The list will only contain something if shaders were compiled but the object itself
	// was destroyed prior to linking.
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}

	if (_programID != 0)
	{
		glDeleteProgram(_programID);
		_programID = 0;
	}
}

void ShadedObject::init()
{
	_programID = glCreateProgram();
	if (_programID == 0) {
		std::cerr << "Error creating shader program" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (0 < _textureImg.size())
	{
		_textureID = initTexture(_textureImg.c_str());
	}
}

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile)
{
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

// Use this method to add shaders to the program. When finished - call finalize()
void ShadedObject::addShader(GLenum shaderType, const char* fileName)
{
	GLchar* source = readShaderSource(fileName);
	if (source == NULL) {
		std::cerr << "Failed to read " << fileName << std::endl;
		exit(EXIT_FAILURE);
	}

	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) {
		std::cerr << "Error creating shader type " << shaderType << std::endl;
		exit(EXIT_FAILURE);
	}

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(shaderObj);

	glShaderSource(shaderObj, 1, (const GLchar**)&source, NULL);
	glCompileShader(shaderObj);

	GLint compiled;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cerr << fileName << " failed to compile:" << std::endl;
		GLint  logSize;
		glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(shaderObj, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;
		exit(EXIT_FAILURE);
	}

	delete[] source;
	glAttachShader(_programID, shaderObj);
}

/* After all the shaders have been added to the program call this 
   function to link and validate the program. */
void ShadedObject::finalize()
{
	GLint linked = 0;
	//GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(_programID);
	glGetProgramiv(_programID, GL_LINK_STATUS, &linked);
	if (linked == 0) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(_programID, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	// Delete the intermediate shader objects that have been added to the program
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();
}

GLint ShadedObject::getUniformLocation(const char* pUniformName)
{
	GLuint location = glGetUniformLocation(_programID, pUniformName);
	return location;
}

GLint ShadedObject::getProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(_programID, param, &ret);
	return ret;
}

GLuint ShadedObject::initTexture(const char* fName)
{
	BImage img(fName);

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D,
			0,							// level
			GL_RGBA8,					// internal representation
			img.width(), img.height(),	// texture size
			0,							// must be 0
			GL_BGR, GL_UNSIGNED_BYTE,	// pixel data format
			img.getImageData());		// pixel data
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}