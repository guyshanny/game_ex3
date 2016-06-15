#include "SkyBox.h"
#include "globals.h"
#include <glm/gtc/matrix_transform.hpp>

SkyBox::SkyBox() :
	OpenMeshObject("shaders/skybox.vs", "shaders/skybox.fs", glm::vec3(0, 0, 0), glm::vec4(1, 1, 1, 1), "meshes/cube.off", ""), 
	m_pCubemapTex("textures/env-osirion-r02", "sky03_rt.bmp", "sky03_lf.bmp", "sky03_up.bmp", "sky03_dn.bmp", "sky03_ft.bmp", "sky03_bk.bmp")
{
}

void SkyBox::init()
{
	OpenMeshObject::init();
	
	// Create and bind the Mesh Vertex Array Object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create and load vertex data into a Vertex Buffer Object
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(glm::vec4) * _vertices.size(),
			&_vertices[0],
			GL_STATIC_DRAW);

		// Obtain attribute handles:
		GLint _posAttr = glGetAttribLocation(_programID, "position");
		glEnableVertexAttribArray(_posAttr);
		glVertexAttribPointer(_posAttr, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			sizeof(glm::vec4) * 2,
			0);

		GLint _normAttr = glGetAttribLocation(_programID, "norm");
		glEnableVertexAttribArray(_normAttr);
		glVertexAttribPointer(_normAttr, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			sizeof(glm::vec4) * 2,
			(GLvoid*)(sizeof(glm::vec4)));

		// Unbind vertex array:
		glBindVertexArray(0);
	}

	m_WVPLocation = getUniformLocation("gWVP");
	m_textureLocation = getUniformLocation("gCubemapTexture");
	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_textureLocation == INVALID_UNIFORM_LOCATION) {
		exit(EXIT_FAILURE);
	}

	BEGIN_OPENGL;
	glUniform1i(m_textureLocation, 0);
	m_pCubemapTex.load();
	//END_OPENGL;
}

void SkyBox::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor) {
	BEGIN_OPENGL;
	{
		GLint OldCullFaceMode;										
		glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);			
		GLint OldDepthFuncMode;										
		glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);			

		glCullFace(GL_FRONT);										
		glDepthFunc(GL_LEQUAL);										

		glm::mat4 model = glm::translate(_model, camPos);			
		glm::mat4 WVP = projection * view * model;					
		glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);	
		m_pCubemapTex.bind(GL_TEXTURE0);	

		//draw mesh
//		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);			//try
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
		// Unbind the Vertex Array object
		glBindVertexArray(0);

		glCullFace(OldCullFaceMode);								
		glDepthFunc(OldDepthFuncMode);								
	}
	END_OPENGL;		
}

GLuint SkyBox::update() 
{
	return 0;
}