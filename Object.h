#pragma once

#include <glm/glm.hpp> //glm::vec3
#include <GL/glew.h> //GLuint
#include <vector>
#include "Sphere.h"
#include "ShadedObject.h"

#define MATERIAL_COLOR "gMaterialColor"

class Object : public ShadedObject
{
public:
	Object(const char* vShaderFile,
		const char* fShaderFile,
		const glm::vec3 & position,
		const glm::vec4 & color,
		const char* textureIMG);
	virtual ~Object() {};

	// Updates the object's params if needed (does in each render frame)
	virtual GLuint update(int deltaTime) = 0;
	virtual void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor) = 0;

	// Draws the object
	virtual void init() {
		ShadedObject::init();
		addShader(GL_VERTEX_SHADER, _vShaderFile);
		addShader(GL_FRAGMENT_SHADER, _fShaderFile);
		finalize();
	}

	glm::mat4 getModel() { return _model; }
	glm::vec3 getPosition() { return _position; }

protected:
	glm::vec3 _position;
	glm::vec4 _color;
	glm::vec3 _speed;
	// Shaders' stuff
	GLuint _vao, _vbo, _ebo;
	const char* _vShaderFile;
	const char* _fShaderFile;
	// MVP
	glm::mat4 _model;	

	std::vector<glm::vec4> _vertices;
	Sphere _boundingSphere;

	void _useMVP(const glm::mat4& projection, const glm::mat4& view);
	void setWorldUniforms(const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor);
	void calcBoundingSphere();
	glm::vec3 calculateCenter();
	void _setPosition(const glm::vec4& pos) { _position = glm::vec3(pos); }
	void _setPosition(const glm::vec3& pos) { _position = pos; }
};

