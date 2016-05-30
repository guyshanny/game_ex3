#include "World.h"
#include "SpaceShip.h"
#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <GL\glut.h> //glutBitmapCharacter


World::World() : _lightPos(0, 0, 0), _lightColor(1, 1, 0.88, 1), 
				_spaceship(glm::vec3(0, -1.5, -3), glm::vec4(1, 0, 0, 1), 
					"shaders\\phong_teapotV3.3.vert", "shaders\\phong.frag", "textures\\marble.bmp", "meshes\\bunny_1k.off")
{
	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	_projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	_camera = Camera::instance();
	_static = new Wall(glm::vec3(0, 0, -80), glm::vec4(0.8, 0, 0, 1), "shaders\\phong_teapotV3.3.vert", "shaders\\phong.frag", 50, 50, "textures\\marble.bmp");
}

void World::init()
{
	_camera->init();
	_spaceship.init();
	_static->init();
}

World::~World()
{
	delete _camera;
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
	// Camera handling
	glm::mat4 view = _camera->getViewMatrix();

	// Drawing scene objects
	_spaceship.draw(_projection, _lightPos, _lightColor);
	_static->draw(_projection, _lightPos, _lightColor);

	GLfloat x, y, yild;
	char* s = "LIVES: 10";	
	glRasterPos2f(-0.9, 0.9);
	print_bitmap_string(GLUT_BITMAP_9_BY_15, s);
}

void World::update()
{
	_camera->update();
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
	_camera->turnUp();
	_spaceship.turnUp();
}

void World::downKeyPressed() {
	_camera->turnDown();
	_spaceship.turnDown();
}

void World::rightKeyPressed() {
	_camera->turnRight();
	_spaceship.turnRight();
}

void World::leftKeyPressed() {
	_camera->turnLeft();
	_spaceship.turnLeft();
}
#pragma endregion