#include "Asteroids.h"
#include "globals.h"
#include "Camera.h"

#define NUM_ROWS 10
#define NUM_COLUMNS 10

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
	//glm::vec3 positions[NUM_ROWS * NUM_COLUMNS];

	//for (unsigned int j = 0; j < NUM_ROWS; j++) {
	//	for (unsigned int i = 0; i < NUM_COLUMNS; i++) {
	//		glm::vec3 pos((i - NUM_COLUMNS / 2)*10.0f, 0.0f, (j - NUM_ROWS / 2)*10.0f);
	//		positions[j * NUM_COLUMNS + i] = pos;
	//	}
	//}

	glm::vec3 positions[3];
	positions[0] = glm::vec3(-25, 0, -60);
	positions[1] = glm::vec3(25, 0, -40);
	positions[2] = glm::vec3(0, 0, 40);

	glGenBuffers(1, &m_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(positions), 
		&positions[0], 
		GL_STATIC_DRAW);
}

void Asteroids::render(const glm::mat4& VP, const glm::vec3& cameraPos)
{
	m_billboard.enable();
	//m_billboard.setVP(VP);
	//m_billboard.setCameraPosition(cameraPos);
	m_billboard.setColorTextureUnit(0);
	m_billboard.bindTexture();
	
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VB);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position

	glDrawArrays(GL_POINTS, 0, 3);//NUM_ROWS * NUM_COLUMNS);

	glDisableVertexAttribArray(0);

	END_OPENGL;
}
