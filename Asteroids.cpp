#include "Asteroids.h"
#include "globals.h"

#define NUM_ROWS 2
#define NUM_COLUMNS 1

Asteroids::Asteroids(const char* textureIMG) : m_VB(INVALID_OGL_VALUE), m_billboard(textureIMG)
{
}

Asteroids::~Asteroids()
{
	if (m_VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &m_VB);
	}
}

void Asteroids::init()
{
	createPositionBuffer();
	m_billboard.init();
}

void Asteroids::createPositionBuffer()
{
	glm::vec3 positions[NUM_ROWS * NUM_COLUMNS];
	float s = 10;
	//for (unsigned int j = 0; j < NUM_ROWS; j++) {
	//	for (unsigned int i = 0; i < NUM_COLUMNS; i++) {
	//		glm::vec3 pos((i-1.5)*s, -5, (float)(j+2));
	//		positions[j * NUM_COLUMNS + i] = pos;
	//	}
	//}
	positions[0] = glm::vec3(-6, -5, -40);
	positions[1] = glm::vec3(6, -5, -20);

	glGenBuffers(1, &m_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(positions), 
		&positions[0], 
		GL_STATIC_DRAW);
}

void Asteroids::render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos, const glm::vec3& cameraUp)
{
	m_billboard.enable();
	m_billboard.setVP(projection, view);
	m_billboard.setCameraPosition(cameraPos);
	m_billboard.setUpVector(cameraUp);
	m_billboard.setColorTextureUnit(0);
	m_billboard.bindTexture();
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position	
	glDrawArrays(GL_POINTS, 0, NUM_ROWS * NUM_COLUMNS);
	glDisableVertexAttribArray(0);

	END_OPENGL;
}
