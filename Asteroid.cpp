#include "Asteroid.h"

Asteroid::Asteroid(const char * vShaderFile, const char * fShaderFile, const char * textureIMG) :
	Object(vShaderFile, fShaderFile, glm::vec3(0.f), glm::vec4(0.f), textureIMG), size(0.5f), isAlive(true), camDist(-1.f)
{
}

// Asteroid::Asteroid(const Asteroid & other):
// 	Object(other._vShaderFile, other._fShaderFile, glm::vec3(0.f), glm::vec4(0.f), other._textureImg.c_str()), size(0.5f), isAlive(true), camDist(-1.f)
// {
// }
