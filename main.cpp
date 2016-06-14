#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "World.h"
//#include "InputManager.h"

int const WINDOW_WIDTH = 512;
int const WINDOW_HEIGHT = 512;

World* _world;
//InputManager* _inputManager;

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void update();
void resizeWindowHandler(int width, int height);
void timer(int value);

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
//	glutTimerFunc(100, timer, 0);   // uint millis int value

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
	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Creating the world
	_world = new World();
	_world->init();

	// Creating input manager
	//_inputManager = new InputManager(_world);
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
	unsigned int lowerKey = tolower(key);

	switch (tolower(lowerKey))
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
	case Controls::KEY_MOVE_FORWARD:
		_world->moveForwardKeyPressed();
		break;
	case Controls::KEY_RESET:
		_world->reset();
		break;
	case Controls::KEY_ESC:
		exit(0);
		break;
	default:
		//_inputManager.longTermKeyDown(lowerKey, x, y);
		break;
	}
}

void update()
{
	static int currentTime, deltaTime, prevTime = 0;
	static int startTime, elapsedTime = 0;
	currentTime = glutGet(GLUT_ELAPSED_TIME); // milliseconds
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	_world->update(deltaTime);

	glutPostRedisplay();
}

void resizeWindowHandler(int width, int height)
{
	_world->resize(width, height);
	
	// set the new viewport
	glViewport(0, 0, width, height);
}

void timer(int value)
{
	glutTimerFunc(25, timer, ++value);   // uint millis int value

	_world->update(value);

// 	_inputManager->timeEvents(); // Input timed events (e.g. smooth moving)

	glutPostRedisplay();
}