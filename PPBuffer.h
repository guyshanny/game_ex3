#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

namespace PPEffects
{
	enum Modes
	{
		CONVOLUTION,
		WAVE,
	};
}

class PPBuffer
{
	// Shaders' handles
	GLuint _vao;
	GLuint _vbo_fbo_vertices;
	GLuint _fbo, _fbo_texture, _rbo_depth;
	GLuint _program_postproc, _attribute_v_coord_postproc, _uniform_fbo_texture;
	GLuint _textureMode;
	GLuint _convMatrixHandle;
	GLuint _waveOffsetHandle;
	GLuint _swirlTimeHandle;
	GLuint _mousePosHandle;
	GLuint _shockWaveElapsedTimeHandle;
	GLuint _teapotPosHandle;

	// PP params
	int _texMode;
	glm::mat3 _convMatrix;
	float _time;
	float _shockedWaveElapsedTime;
	glm::vec2 _mousePos;
	bool _isShowckWaveEffect;
	glm::vec3 _teapotPos;

	// Window
	int _width;
	int _height;
	float _offsetX;
	float _offsetY;

	void _getShaderHandles();

public:
	PPBuffer();
	virtual ~PPBuffer();
	void init(const int& screenWidth, const int& screenHeight);
	void setup();
	void render();
	void reset();
	void resize(int width, int height);

	// Effects
	void setConvolutionMatrix(const glm::mat3& convMatrix);
	void wave();
};