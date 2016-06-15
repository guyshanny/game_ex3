#pragma once

#include "Billboard.h"
#include <vector>
#include <math.h>
#include "SpaceShip.h"
//#include "Asteroid.h"
#include "Sphere.h"

#define MAX_ASTEROIDS 100
#define NEW_AST_PER_SEC 5
#define MAX_SPEED 0.2f

 struct Asteroid
{
 	Asteroid() : isAlive(false) {}

	bool operator<(const Asteroid& other) const {
		return camDist > other.camDist;
	}

	void kill() { isAlive = false; }

	void update(const bool& newIsAlive,
				const GLfloat& newSize,
				const GLfloat& newRadius,
				const glm::vec3& newPosition,
				const glm::vec3& newSpeed)
	{
		isAlive = newIsAlive;
		size = newSize;
		radius = newRadius;
		position = newPosition;
		speed = newSpeed;

		_sphere = Sphere(position, radius);
	}

	bool isCollide(const Sphere& otherSphere)
	{
		return _sphere.isCollide(otherSphere);
	}
	
//	GLuint _id;
 	glm::vec3 position, speed;
 	GLfloat size, radius, camDist;
	bool isAlive;
	Sphere _sphere;
	GLuint type;
};

class AsteroidsField
{
public:
	AsteroidsField(std::vector<char*> textures);
	~AsteroidsField();
	void AsteroidsField::init(const glm::vec3& center,
							  const GLfloat& minRadius,
							  const GLfloat& maxRadius,
							  const GLuint& maxNumOfAsteroids);
	void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp);
	void update(int deltaTime, const glm::vec3& playerPos);
	void setCamera(const glm::vec3& pos) { _cameraPos = pos; }
	void setCenter(const glm::vec3& pos) { _center = pos; }
	GLuint handleCollisions(const Sphere& playerBoundingSphere);

private:
	glm::vec3 _center;
	std::vector<char*> _textures;
	GLuint _nAsteroids;
	//std::vector<Asteroid> _asteroids;
	Asteroid _asteroids[MAX_ASTEROIDS];
	GLuint _lastUsedAsteroid;
	GLfloat _minRadius;
	GLfloat _maxRadius;
	glm::vec3 _cameraPos;
//	GLuint _maxNumOfAsteroids;

//	void _createRandomAsteroids(const GLuint& num, const bool& isInit);
//	void _addAsteroid(const GLuint& id);
	void _addAsteroid(const GLuint& id);
	GLuint _findUnusedAsteroid();
	GLfloat _rand(const GLfloat& min, const GLfloat& max);
	void _cpu2gpu();
	void _sortParticles();
	

	GLuint _vb;				// vertex buffer
	Billboard _billboard;
};

