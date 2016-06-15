#include "Billboard.h"
#include "globals.h"
#include <string>

Billboard::Billboard(const char* textureIMG) : ShadedObject(textureIMG)
{
}

Billboard::Billboard(const std::vector<char*> textures) : ShadedObject(std::string().c_str()),
														  _textures(textures)
{
}

void Billboard::init()
{
	ShadedObject::init();
	addShader(GL_VERTEX_SHADER, "shaders\\billboard.vs");
	addShader(GL_GEOMETRY_SHADER, "shaders\\billboard.gs");
	addShader(GL_FRAGMENT_SHADER, "shaders\\billboard.fs");
	finalize();

	glUseProgram(_programID);

	m_viewLocation = getUniformLocation("gView");
	m_projectionLocation = getUniformLocation("gProjection");
	m_cameraPosLocation = getUniformLocation("gCameraPos");
	m_upLocation = getUniformLocation("gUp");

	if (INVALID_UNIFORM_LOCATION == m_viewLocation ||
		INVALID_UNIFORM_LOCATION == m_projectionLocation ||
		INVALID_UNIFORM_LOCATION == m_cameraPosLocation ||
		INVALID_UNIFORM_LOCATION == m_upLocation) 
	{
		exit(EXIT_FAILURE);
	}

	_initTextures();
	glUseProgram(0);
}

void Billboard::_initTextures()
{
	for (GLuint i = 0; i < MAX_TEXTURES; i++)
	{
		// Getting uniforms' handle
		std::string param = std::string("gTextureSampler") +
			std::to_string(i);
		m_textureLocation[i] = getUniformLocation(param.c_str());
// 		if (INVALID_UNIFORM_LOCATION == m_textureLocation[i])
// 		{
// 			exit(EXIT_FAILURE);
// 		}

		// Setting uniforms' value
		glUniform1i(m_textureLocation[i], i);

		// Initiating and binding textures
		_texturesHandles[i] = initTexture(_textures[i]);
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

void Billboard::setUpVector(const glm::vec3 & up)
{
	glUniform3f(m_upLocation, up.x, up.y, up.z);
}

void Billboard::enable()
{
	BEGIN_OPENGL;
}
