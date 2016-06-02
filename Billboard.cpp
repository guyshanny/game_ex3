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

	//m_VPLocation = getUniformLocation("gVP");
	//m_cameraPosLocation = getUniformLocation("gCameraPos");
	m_colorMapLocation = getUniformLocation(TEXTURE_SAMPLER);

	if (/*m_VPLocation == INVALID_UNIFORM_LOCATION ||
		m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||*/
		m_colorMapLocation == INVALID_UNIFORM_LOCATION) {
		exit(EXIT_FAILURE);
	}
}

void Billboard::setVP(const glm::mat4& VP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, &VP[0][0]);
}

void Billboard::setCameraPosition(const glm::vec3& pos)
{
	glUniform3f(m_cameraPosLocation, pos.x, pos.y, pos.z);
}

void Billboard::setColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorMapLocation, TextureUnit);
}

void Billboard::enable()
{
	BEGIN_OPENGL;
}

void Billboard::bindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}
