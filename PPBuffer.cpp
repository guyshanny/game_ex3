#include "PPBuffer.h"

#include "ShaderIO.h"
#include "PPBuffer.h"
#include "Globals.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

PPBuffer::PPBuffer() : _texMode(0),
_convMatrix(0.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 0.f),
	_time(0.f),
	_teapotPos(0.f),
	_isShowckWaveEffect(false)
{
}

PPBuffer::~PPBuffer() {
	glDeleteRenderbuffers(1, &_rbo_depth);
	glDeleteTextures(1, &_fbo_texture);
	glDeleteFramebuffers(1, &_fbo);

	glDeleteBuffers(1, &_vbo_fbo_vertices);

	glDeleteProgram(_program_postproc);
}

void PPBuffer::init(const int& screenWidth, const int& screenHeight)
{
	_width = screenWidth;
	_height = screenHeight;

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	/* Texture  */

	// The texture we're going to render to
	glGenTextures(1, &_fbo_texture);

	glBindTexture(GL_TEXTURE_2D, _fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	// depth buffer
	glGenRenderbuffers(1, &_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _fbo_texture, 0); //use more of these for MRT
																				 // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fbo_texture1, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo_depth);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, DrawBuffers);

	// for MRT:  glDrawBuffers(2,{GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1}); 

	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "glCheckFramebufferStatus: error %d", status);
		exit(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// vertices 
	GLfloat fbo_vertices[] =
	{
		-1, -1,
		1, -1,
		-1,  1,
		1,  1,
	};

	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// shader
	programManager::sharedInstance()
		.createProgram("pp",
			 "shaders\\postproc.v.glsl",
			 "shaders\\postproc.f.glsl");
	_program_postproc = programManager::sharedInstance().programWithID("pp");

	_getShaderHandles();
}

void PPBuffer::_getShaderHandles()
{
	_program_postproc = programManager::sharedInstance().programWithID("pp");
	_attribute_v_coord_postproc = glGetAttribLocation(_program_postproc, "v_coord");
	_uniform_fbo_texture = glGetUniformLocation(_program_postproc, "fbo_texture");
	_textureMode = glGetUniformLocation(_program_postproc, "textureMode");
	_convMatrixHandle = glGetUniformLocation(_program_postproc, "convMatrix");
	_waveOffsetHandle = glGetUniformLocation(_program_postproc, "offset");
}

void PPBuffer::setup()
{
	// Render to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _width, _height);
}

void PPBuffer::render()
{
	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// 0 - render to the screen
	glViewport(0, 0, _width, _height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.3f, 0.f, 0.f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(_program_postproc);

	glActiveTexture(GL_TEXTURE0);

	// Binds shaders' variables
	glBindTexture(GL_TEXTURE_2D, _fbo_texture);
	glUniform1i(_uniform_fbo_texture, /*GL_TEXTURE*/0);
	glUniform1i(_textureMode, _texMode);
	glUniformMatrix3fv(_convMatrixHandle, 1, GL_FALSE, value_ptr(_convMatrix));
	GLfloat move = (GLfloat)((GLUT_ELAPSED_TIME) / 1000.0 * 2 * 3.14159 * .75);  // 3/4 of a wave cycle per second
	glUniform1f(_waveOffsetHandle, move);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(_attribute_v_coord_postproc);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_fbo_vertices);
	glVertexAttribPointer(
		_attribute_v_coord_postproc,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(_attribute_v_coord_postproc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void PPBuffer::reset()
{
	setConvolutionMatrix(glm::mat3(0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 0.f));
}

void PPBuffer::resize(int screen_width, int screen_height)
{
	/* onReshape */
	// Rescale FBO and RBO as well
	glBindTexture(GL_TEXTURE_2D, _fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width, screen_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	_width = screen_width;
	_height = screen_height;
}

#pragma region Effects
void PPBuffer::setConvolutionMatrix(const glm::mat3 & convMatrix)
{
	_texMode = PPEffects::Modes::CONVOLUTION;
	_convMatrix = convMatrix;
}

void PPBuffer::wave()
{
	_texMode = PPEffects::Modes::WAVE;
}
#pragma endregion