#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "World.h"

int const WINDOW_WIDTH = 512;
int const WINDOW_HEIGHT = 512;

World* _world;

namespace Controls
{
	enum KeyControls
	{
		KEY_ESC = (27),
		KEY_RESET = ('r'),
		KEY_RELOAD = ('l'),

		// Teapot controls
		KEY_MOVE = (' '),

		// Camera controls
		KEY_UP = ('w'),
		KEY_DOWN = ('s'),
		KEY_LEFT = ('a'),
		KEY_RIGHT = ('d'),
	};
}

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void update();
void resizeWindowHandler(int width, int height);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE;
	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	//glutMouseWheelFunc(mouseWheel);
	//glutPassiveMotionFunc(mouseMove);
	//glutSpecialFunc(specialkey);
	glutIdleFunc(update);
	glutReshapeFunc(resizeWindowHandler);

	glutMainLoop();

	if (NULL != _world)
	{
		delete _world;
		_world = NULL;
	}
	return 0;
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Creating the world
	_world = new World();
	_world->init();
}

void display(void)
{
	//glClearColor(1.0, 0.4, 0.0, 0.0); //background color (default is black)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window

	_world->draw();

	// Swap those buffers so someone will actually see the results... //
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case Controls::KEY_UP:
		_world->upKeyPressed();
		break;
	case Controls::KEY_DOWN:
		_world->downKeyPressed();
		break;
	case Controls::KEY_RIGHT:
		_world->rightKeyPressed();
		break;
	case Controls::KEY_LEFT:
		_world->leftKeyPressed();
		break;
	case Controls::KEY_ESC:
		exit(0);
		break;
	case Controls::KEY_MOVE:
		//_world->changeColorKeyPressed();
		break;
	default:
		std::cerr << "Key " << tolower(key) << " undefined\n";
		break;
	}
	glutPostRedisplay();
}

void update()
{
	static int currentTime, deltaTime, prevTime = 0;
	static int startTime, elapsedTime = 0;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	_world->update();

	glutPostRedisplay();
}

void resizeWindowHandler(int width, int height)
{
	_world->resize(width, height);
	
	// set the new viewport
	glViewport(0, 0, width, height);

	// Refresh the display
	glutPostRedisplay();
}