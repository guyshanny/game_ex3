#include "AsteroidsField.h"
#include "globals.h"
#include "Camera.h"
#include <iostream>

AsteroidsField::AsteroidsField(std::vector<char*> textures) :
	_textures(textures),
	_nAsteroids(0),
	_lastUsedAsteroid(0),
	_vb(INVALID_OGL_VALUE), _tb(INVALID_OGL_VALUE),
	_billboard(textures)
{
}

AsteroidsField::~AsteroidsField()
{
	if (_vb != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &_vb);
	}
	if (_tb != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &_tb);
}
}

void AsteroidsField::init(const glm::vec3& center, 
						  const GLfloat& minRadius, const GLfloat& maxRadius,
						  const GLuint& maxNumOfAsteroids)
{
	// Params for the asteroid field
	_center = center;
	_minRadius = minRadius;
	_maxRadius = maxRadius;
//	_maxNumOfAsteroids = maxNumOfAsteroids;

	for (GLuint i = 0; i < MAX_ASTEROIDS; i++) {
		//_asteroids[i].isAlive = false;
		//_asteroids[i].camDist = -1;
		_addAsteroid(i);
	}

	_billboard.init();

	glGenBuffers(1, &_vb);
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * MAX_ASTEROIDS,
		NULL,
		GL_STATIC_DRAW);

	glGenBuffers(1, &_tb);
	glBindBuffer(GL_ARRAY_BUFFER, _tb);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLuint) * MAX_ASTEROIDS,
		NULL,
		GL_STATIC_DRAW);
}

void AsteroidsField::_cpu2gpu()
{
	std::vector<glm::vec4> positions;
	std::vector<GLuint> types;
	for (Asteroid& asteroid : _asteroids)
	{
		if (asteroid.isAlive)
		{
			positions.push_back(glm::vec4(asteroid.position, asteroid.size));
			types.push_back(asteroid.type);
		}
	}

	_nAsteroids = positions.size();

	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_ASTEROIDS, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * _nAsteroids, &positions[0]);	

	glBindBuffer(GL_ARRAY_BUFFER, _tb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * MAX_ASTEROIDS, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLuint) * _nAsteroids, &types[0]);
}

GLuint AsteroidsField::handleCollisions(const Sphere& playerBoundingSphere)
{
	GLuint count = 0;
	for (Asteroid& asteroid : _asteroids)
	{
		if (asteroid.isCollide(playerBoundingSphere))
		{
 			count += 1;
			asteroid.kill();
		}
	}

	return count;
}

void AsteroidsField::update(int deltaTime, const glm::vec3& playerPos)
{
	float delta = deltaTime * 0.001f;
	for (GLuint i = 0; i < MAX_ASTEROIDS; i++)
	{
		Asteroid& a = _asteroids[i];

		// There wasn't collision 
		if (a.isAlive)
		{
			// Moving asteroid
			a.position += a.speed * delta;
			a._sphere.setCenter(a.position);

			// Handle killing if reaches grid's end
			if (glm::length(a.position - _center) <= _maxRadius)
			{
				a.camDist = glm::length(a.position - _cameraPos);
			}
			else // Kill
			{
				a.isAlive = false;
				_addAsteroid(i);
			}
		}
		// There was a collision with the asteroid
		else
		{
			_addAsteroid(i);
			a.camDist = -1;
		}
	}

	_sortParticles();
	_cpu2gpu();
}

void AsteroidsField::_addAsteroid(const GLuint& id)
{
// 	GLuint index = _findUnusedAsteroid();
	Asteroid& asteroid = _asteroids[id];
	GLfloat size = _rand(0.1f, 5.f);
	GLfloat radius = size / 2.f;

	// Handle location
	GLfloat r = _rand(_minRadius, _maxRadius);
	GLfloat theta = _rand((GLfloat)0, (GLfloat)(M_PI*2.f));
	GLfloat phi = _rand((GLfloat)0, (GLfloat)(M_PI*1.f));
	glm::vec3 position = _center + glm::vec3(r * sinf(phi) * cosf(theta),
											r * cosf(phi),
											r * sinf(phi) * sinf(theta));
	glm::vec3 speed = glm::vec3(_rand(-MAX_SPEED, MAX_SPEED), 
							   _rand(-MAX_SPEED, MAX_SPEED), 
							   _rand(-MAX_SPEED, MAX_SPEED));

	GLuint min = 0;
	GLuint max = MAX_TEXTURES-1;
	GLuint type = min + (rand() % (int)(max - min + 1));

	asteroid.update(true, size, radius, position, speed, type);
}

void AsteroidsField::_sortParticles() {
	std::sort(&_asteroids[0], &_asteroids[MAX_ASTEROIDS]);
}

GLuint AsteroidsField::_findUnusedAsteroid()
{
	for (GLuint i = _lastUsedAsteroid; i < MAX_ASTEROIDS; i++)
	{
		// if asteroid is dead
		if (!_asteroids[i].isAlive)
		{
			_lastUsedAsteroid = i;
			return i;
		}
	}

	for (GLuint i = 0; i < _lastUsedAsteroid; i++) {
		if (!_asteroids[i].isAlive) {
			_lastUsedAsteroid = i;
			return i;
		}
	}

	return 0;  // All particles are taken, override the first one
}

GLfloat AsteroidsField::_rand(const GLfloat& min, const GLfloat& max)
{
	float result = (rand() % 10000) / 10000.f;
	return result * (max - min) + min;
}

void AsteroidsField::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp)
{
	_billboard.enable();
	_billboard.setVP(projection, view);
	_billboard.setCameraPosition(cameraPos);
	_billboard.setUpVector(cameraUp);

	GLuint* texturesHandles = _billboard.getTexturesHandles();
	for (GLuint i = 0; i < MAX_TEXTURES; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texturesHandles[i]);
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);   // position + size
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _tb);
	glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);   // type
	
	glDrawArrays(GL_POINTS, 0, _nAsteroids);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	END_OPENGL;
}
