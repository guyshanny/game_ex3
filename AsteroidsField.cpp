#include "AsteroidsField.h"
#include "globals.h"
#include "Camera.h"

// Asteroid::Asteroid(const GLuint& id, const GLuint& radius, const glm::vec3& playerPos, const GLfloat& size = 2) :
// 	_id(id), _isAlive(true), _size(size)
// {
// 	double randNum = (double)rand() / (double)RAND_MAX;						// between 0 and 1
// 	double r = 3 + randNum * (radius - 3);
// 	GLfloat l = (GLfloat)(randNum * (2.0f * M_PI));						//longitude
// 	GLfloat h = (GLfloat)(randNum * M_PI / 2.f * (randNum > 0.5));			//latitude
// 	_position = playerPos + glm::vec3(r*glm::cos(l)*glm::cos(h), r*glm::sin(h), r*glm::sin(l)*glm::cos(h));
// }
// 
// bool Asteroid::isExceedsRange(const glm::vec3 & playerPos, const GLuint& radius)
// {
// 	float z = glm::distance(_position, playerPos);
// 	return (glm::distance(_position, playerPos) > radius);
// }


AsteroidsField::AsteroidsField(std::vector<char*> textures) :
	_textures(textures),
	_nAsteroids(0),
	_lastUsedAsteroid(0),
	_vb(INVALID_OGL_VALUE),
	_billboard(textures.at(0))
{
}

AsteroidsField::~AsteroidsField()
{
	if (_vb != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &_vb);
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
		_asteroids[i].isAlive = false;
		_asteroids[i].camDist = -1;
	}

	//_createRandomAsteroids(_maxNumOfAsteroids, true);
	//_cpu2gpu();
	_billboard.init();

	glGenBuffers(1, &_vb);
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * MAX_ASTEROIDS,
		NULL,
		GL_STATIC_DRAW);
}

void AsteroidsField::_cpu2gpu()
{
	std::vector<glm::vec4> positions;
	for (Asteroid asteroid : _asteroids)
	{
		// if asteroid is alive
		if (asteroid.isAlive)
		{
			positions.push_back(glm::vec4(asteroid.position, asteroid.size));
		}
	}

	_nAsteroids = positions.size();

	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_ASTEROIDS, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * _nAsteroids, &positions[0]);	
}

void AsteroidsField::update(int deltaTime)
{
	_addAsteroid();

	for (GLuint i = 0; i < MAX_ASTEROIDS; i++)
	{
		Asteroid& a = _asteroids[i];

		// There wasn't collision 
		if (a.isAlive)
		{
			// Moving asteroid
			a.position += a.speed;// *(GLfloat)deltaTime;

			// Handle killing if reaches grid's end
			if (glm::length(a.position - _center) <= _maxRadius)
			{
				a.camDist = glm::length(a.position - _cameraPos);
			}
			else // Kill
			{
				a.isAlive = false;
				//_createRandomAsteroids(1, false);
			}
		}
		else
		{
			a.camDist = -1;
		}
	}

	_sortParticles();
	_cpu2gpu();
}

void AsteroidsField::_addAsteroid()
{
	GLuint index = _findUnusedAsteroid();
	Asteroid& asteroid = _asteroids[index];
	asteroid.isAlive = true;
	asteroid.size = _rand(0.1f, 5.f);
	asteroid.radius = asteroid.size * glm::sqrt(0.5f);

	// Handle location
	GLfloat radius = _rand(_minRadius, _maxRadius);
	GLfloat theta = _rand((GLfloat)0, (GLfloat)(M_PI*2.f));
	GLfloat phi = _rand((GLfloat)0, (GLfloat)(M_PI*1.f));
	asteroid.position = _center + glm::vec3(radius * sinf(phi) * cosf(theta),
											radius * cosf(phi),
											radius * sinf(phi) * sinf(theta));
	asteroid.speed = glm::vec3(_rand(-MAX_SPEED, MAX_SPEED), 
							   _rand(-MAX_SPEED, MAX_SPEED), 
							   _rand(-MAX_SPEED, MAX_SPEED));
}

/*void AsteroidsField::_addAsteroid(const GLuint& id)
{
	GLuint asteroidType = rand() % _textures.size();

	Asteroid asteroid(_vShaderFile, _fShaderFile, _textures[asteroidType]);
	asteroid.life = INITIAL_LIFE;
	asteroid.size = _rand(0.04f, 4.f);
	asteroid.radius = asteroid.size * glm::sqrt(0.5f);

	// Handle location
	GLfloat radius = _rand(_minRadius, _maxRadius);
	GLfloat theta = _rand((GLfloat)0, (GLfloat)M_PI*2.f);
	GLfloat phi = _rand((GLfloat)0, (GLfloat)M_PI*1.f);
	asteroid.setPosition(_center + glm::vec3(radius * sinf(phi) * cosf(theta),
											 radius * cosf(phi),
											 radius * sinf(phi) * sinf(theta)));
	asteroid.setSpeed(glm::vec3(_rand(-0.3f, 0.3f), _rand(-0.3f, 0.3f), _rand(-0.3f, 0.3f)));
	asteroid.type = asteroidType;

	_asteroids.push_back(asteroid);
}*/

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

/*void AsteroidsField::_createRandomAsteroids(const GLuint& num, const bool& isInit = false)
{
	for (GLuint i = 0; i < num; i++)
	{
		if (isInit) { _addAsteroid(i); }
		else { _addAsteroid(); }
	}
}*/

void AsteroidsField::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp)
{
	_billboard.enable();
	_billboard.setVP(projection, view);
	_billboard.setCameraPosition(cameraPos);
	_billboard.setUpVector(cameraUp);
	_billboard.setColorTextureUnit(0);
	_billboard.bindTexture(_textures[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);   // position
	
	glDrawArrays(GL_POINTS, 0, _nAsteroids);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	END_OPENGL;
}
