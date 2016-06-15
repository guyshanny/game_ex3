#include "Object.h"
#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
#include <glm/gtc/matrix_transform.hpp>


Object::Object(const char* vShaderFile, const char* fShaderFile,
	const glm::vec3 & position, const glm::vec4 & color, const char* textureIMG) :
	ShadedObject(textureIMG), _vShaderFile(vShaderFile), _fShaderFile(fShaderFile), _position(position), _color(color)
{
}

void Object::_useMVP(const glm::mat4 & projection, const glm::mat4 & view)
{
	// Get a handle for our "gProjection" uniform
	GLuint projectionMatrixID = getUniformLocation("gProjection");
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projection[0][0]);
	// Get a handle for our "gView" uniform
	GLuint viewMatrixID = getUniformLocation("gView");
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);
	// Get a handle for our "gModel" uniform
	GLuint modelMatrixID = getUniformLocation("gModel");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &_model[0][0]);
}

void Object::calcBoundingSphere() {
	glm::vec3 center = calculateCenter();
	double maxDist = 0; // distance of farthest vertex from center
	double dist;
	glm::vec4 p;
	for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
		p = *it;
		dist = std::sqrt(std::pow(p.x - center.x, 2) +
			std::pow(p.y - center.y, 2) +
			std::pow(p.z - center.z, 2));
		if (dist > maxDist) {
			maxDist = dist;
		}
	}
	_boundingSphere = Sphere(center, maxDist);
}

glm::vec3 Object::calculateCenter() {
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::vec4 p;
	for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
		center += glm::vec3(*it);
	}
	return center / (float)_vertices.size();
}


void Object::setWorldUniforms(const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor)
{
	GLuint cameraID = getUniformLocation("gEyePosition");
	glUniform3f(cameraID, camPos.x, camPos.y, camPos.z);

	GLuint lightPosID = getUniformLocation("gLightPosition");
	glUniform4f(lightPosID, lightPos.x, lightPos.y, lightPos.z, 1);

	GLuint lightColorID = getUniformLocation("gLightColor");
	glUniform4f(lightColorID, lightColor.r, lightColor.g, lightColor.b, 1);
}

