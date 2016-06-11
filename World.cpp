#include "World.h"
#include "SpaceShip.h"
#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective, lookAt
#include <GL\glut.h> //glutBitmapCharacter


World::World() : _camera(&_spaceship, glm::vec3(0, 1.5, 3)), _lightColor(1, 1, 0.88, 1),
				_spaceship(glm::vec3(0, 0, 0), glm::vec4(0, 0.1, 0.6, 1), 
							"shaders/phong_spaceship.vert", "shaders/phong.frag", 
							"textures/marble.bmp", "meshes/bunny_1k.off"),
				_asteroids("textures/asteroid.bmp")
{
	// params: fovy, aspect, zNear, zFar
	_projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

void World::init()
{
	_spaceship.init();
	_asteroids.init();
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
	_asteroids.render(_projection, view, camPos, _spaceship.getUp());
	_spaceship.draw(_projection, view, camPos, camPos, _lightColor);
	_skybox.draw(_projection, view, camPos, camPos, _lightColor);

	char* s = "LIVES: 10";	
	glRasterPos2f(-0.9f, 0.9f);
	print_bitmap_string(GLUT_BITMAP_9_BY_15, s);
}

void World::update()
{
	_spaceship.update();
	_camera.update();
	_skybox.update();
}

void World::resize(int width, int height)
{
	_width = (float)width;
	_height = (float)height;
	_offsetX = 0;
	_offsetY = 0;
}

#pragma region KeysHandling

void World::upKeyPressed() {
	_spaceship.turnUp();
}

void World::downKeyPressed() {
	_spaceship.turnDown();
}

void World::rightKeyPressed() {
	_spaceship.turnRight();
}

void World::leftKeyPressed() {
	_spaceship.turnLeft();
}
#pragma endregion
