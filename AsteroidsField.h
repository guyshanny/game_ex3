#pragma once

#include "Billboard.h"
#include <vector>
#include <math.h>
#include "SpaceShip.h"
#include "Asteroid.h"

// class Asteroid
// {
// public:
// 	Asteroid(const GLuint& id, const GLuint& r, const glm::vec3& playerPos, const GLfloat& size);
// 	bool isExceedsRange(const glm::vec3 & playerPos, const GLuint& radius);
// 
// 
// private:
// 	GLuint _id;
// 	glm::vec3 _position;
// 	bool _isAlive;
// 	GLfloat _size;
// };

class AsteroidsField
{
public:
	AsteroidsField::AsteroidsField(std::vector<char*> textures);
	~AsteroidsField();
	void AsteroidsField::init(glm::vec3& center,
							  const GLfloat& minRadius,
							  const GLfloat& maxRadius,
							  const GLuint& maxNumOfAsteroids);
	void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp);
	void update();
	void setCamera(const glm::vec3& pos) { _cameraPos = pos; }
	void setCenter(const glm::vec3& pos) { _center = pos; }

private:
	glm::vec3 _center;
	std::vector<char*> _textures;
	GLuint _nAsteroids;
	std::vector<Asteroid> _asteroids;
	GLuint _lastUsedAsteroid;
	GLfloat _minRadius;
	GLfloat _maxRadius;
	glm::vec3 _cameraPos;
	GLuint _maxNumOfAsteroids;

	void _createRandomAsteroids(const GLuint& num, const bool& isInit);
	void _addAsteroid(const GLuint& id);
	void _addAsteroid();
	GLuint _findUnusedAsteroid();
	GLfloat _rand(const GLfloat& min, const GLfloat& max);
	void _cpu2gpu();

	GLuint _vb;
	Billboard _billboard;

	const char* _vShaderFile;
	const char* _fShaderFile;
	const char* _textureIMG;
};

