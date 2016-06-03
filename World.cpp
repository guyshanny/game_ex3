#include "World.h"
#include "SpaceShip.h"
#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective, lookAt
#include <GL\glut.h> //glutBitmapCharacter


World::World() : _camOffset(0, 1.5, 3), _lightColor(1, 1, 0.88, 1), 
				_spaceship(glm::vec3(0, 0, 0), glm::vec4(0, 0.1, 0.6, 1), 
							"shaders\\phong_spaceship.vert", "shaders\\phong.frag", 
							"textures\\marble.bmp", "meshes\\bunny_1k.off"),
				_asteroids("textures\\wall.bmp")
{
	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// params: fovy, aspect, zNear, zFar
	_projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	_static = new Wall(glm::vec3(0, 0, -80), glm::vec4(0.8, 0, 0, 1), "shaders\\phong_wall.vert", "shaders\\phong.frag", 50, 50, "textures\\wall.bmp");
}

glm::mat4 World::getViewMat() {
	glm::vec3 camPos = getCamPos();
	glm::vec3 dir = _spaceship.getDirection();
	glm::vec3 target = camPos + dir;
	return glm::lookAt(camPos, target, _spaceship.getUp());
}

glm::vec3 World::getCamPos()
{
	return glm::vec3(_spaceship.getModel() * glm::vec4(_camOffset, 1));
}

void World::init()
{
	_spaceship.init();
	_static->init();
	_asteroids.init();
}

World::~World()
{
	delete _static;
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
	glm::vec3 camPos = getCamPos();
	// Drawing scene objects
	_spaceship.draw(_projection, getViewMat(), camPos, camPos, _lightColor);
	_static->draw(_projection, getViewMat(), camPos, camPos, _lightColor);
	_asteroids.render(_projection*getViewMat(), camPos);

	char* s = "LIVES: 10";	
	glRasterPos2f(-0.9f, 0.9f);
	print_bitmap_string(GLUT_BITMAP_9_BY_15, s);
}

void World::update()
{
	_spaceship.update();
	_static->update();
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