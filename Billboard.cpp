#include "Billboard.h"
#include "globals.h"

Billboard::Billboard(const char* textureIMG) : ShadedObject(textureIMG)
{
}

void Billboard::init()
{
	ShadedObject::init();
	addShader(GL_VERTEX_SHADER, "shaders\\billboard.vs");
	addShader(GL_GEOMETRY_SHADER, "shaders\\billboard.gs");
	addShader(GL_FRAGMENT_SHADER, "shaders\\billboard.fs");
	finalize();

	m_viewLocation = getUniformLocation("gView");
	m_projectionLocation = getUniformLocation("gProjection");
	m_cameraPosLocation = getUniformLocation("gCameraPos");
	m_upLocation = getUniformLocation("gUp");
	m_colorMapLocation = getUniformLocation(TEXTURE_SAMPLER);

	if (INVALID_UNIFORM_LOCATION == m_viewLocation ||
		INVALID_UNIFORM_LOCATION == m_projectionLocation ||
		INVALID_UNIFORM_LOCATION == m_cameraPosLocation ||
		INVALID_UNIFORM_LOCATION == m_colorMapLocation ||
		INVALID_UNIFORM_LOCATION == m_upLocation) {
		exit(EXIT_FAILURE);
	}
}

void Billboard::setVP(const glm::mat4& projection, const glm::mat4& view)
{
	glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, &view[0][0]);
}

void Billboard::setCameraPosition(const glm::vec3& pos)
{
	glUniform3f(m_cameraPosLocation, pos.x, pos.y, pos.z);
}

void Billboard::setColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorMapLocation, TextureUnit);
}

void Billboard::setUpVector(const glm::vec3 & up)
{
	glUniform3f(m_upLocation, up.x, up.y, up.z);
}

void Billboard::enable()
{
	BEGIN_OPENGL;
}

void Billboard::bindTexture(const char* textureIMG)
{
	_textureID = initTexture(textureIMG);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}
