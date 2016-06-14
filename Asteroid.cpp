#include "Asteroid.h"

Asteroid2::Asteroid2(const char * vShaderFile, const char * fShaderFile, const char * textureIMG) :
	Object(vShaderFile, fShaderFile, glm::vec3(0.f), glm::vec4(0.f), textureIMG), size(0.5f), life(INITIAL_LIFE), camDist(-1.f)
{
}

// Asteroid2::Asteroid2(const Asteroid & other):
// 	Object(other._vShaderFile, other._fShaderFile, glm::vec3(0.f), glm::vec4(0.f), other._textureImg.c_str()), size(0.5f), life(INITIAL_LIFE), camDist(-1.f)
// {
// }
