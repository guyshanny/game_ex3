#include "World.h"
#include "SpaceShip.h"
#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective, lookAt
#include <GL\glut.h> //glutBitmapCharacter


World::World() : _camera(&_spaceship, glm::vec3(0, 1.5, 3)), _lightColor(1, 1, 0.88, 1),
				_spaceship(glm::vec3(0, 0, 0), glm::vec4(0, 0.1, 0.6, 1), 
							"shaders\\phong_spaceship.vert", "shaders\\phong.frag", 
							"textures\\marble.bmp", "meshes\\bunny_1k.off"),
	_asteroids({ "textures\\asteroids\\asteroid1.bmp", 
				 "textures\\asteroids\\asteroid3.bmp", 
				 "textures\\asteroids\\asteroid4.bmp",
				 "textures\\asteroids\\asteroid5.bmp",
				 "textures\\asteroids\\asteroid6.bmp",
				 "textures\\asteroids\\asteroid7.bmp",
				 "textures\\asteroids\\asteroid8.bmp",
				 "textures\\asteroids\\asteroid9.bmp" })
{
	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// params: fovy, aspect, zNear, zFar
	_projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

void World::init()
{
	_spaceship.init();
	_asteroids.init(_spaceship.getPosition(), 10.f, 20.f, 20);
	_skybox.init();
}

World::~World()
{
}

void print_bitmap_string(void* font, char* s) {
	if (s && strlen(s)) {
		while (*s) {
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

void World::draw()
{
	glm::vec3 camPos = _camera.getPos();
	glm::mat4 view = _camera.getView();
	// Drawing scene objects
	_spaceship.draw(_projection, view, camPos, camPos, _lightColor);
	_skybox.draw(_projection, view, camPos, camPos, _lightColor);
	_asteroids.draw(_projection, view, camPos, _spaceship.getUp());

	char* s = "LIVES: 10";	
	glRasterPos2f(-0.9f, 0.9f);
	print_bitmap_string(GLUT_BITMAP_9_BY_15, s);
}

void World::_updateCameraDependencies()
{
	_asteroids.setCamera(_camera.getPos());
}

void World::update(int deltaTime)
{
	_spaceship.update();
	_camera.update();

	_updateCameraDependencies();

	_asteroids.update(deltaTime);
	_skybox.update();
}

void World::resize(int width, int height)
{
	_width = (float)width;
	_height = (float)height;
	_offsetX = 0;
	_offsetY = 0;
}

void World::reset()
{
	_spaceship.reset();
	_asteroids.setCenter(_spaceship.getPosition());
}

#pragma region KeysHandling

void World::upKeyPressed() { _spaceship.turnUp(); }
void World::downKeyPressed() { _spaceship.turnDown(); }
void World::rightKeyPressed() { _spaceship.turnRight(); }
void World::leftKeyPressed() { _spaceship.turnLeft(); }
void World::moveForwardKeyPressed() 
{ 
	_spaceship.moveForword(); 
	_asteroids.setCenter(_spaceship.getPosition());
}
#pragma endregion
