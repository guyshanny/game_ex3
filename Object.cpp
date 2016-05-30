#include "Object.h"
#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
#include "bimage.h"


Object::Object(const char* vShaderFile, const char* fShaderFile,
	const glm::vec3 & position, const glm::vec4 & color, const char* textureIMG) :
	_vShaderFile(vShaderFile), _fShaderFile(fShaderFile), _position(position), _color(color), _textureImg(textureIMG)
{
	_camera = Camera::instance();
}

void Object::_useMVP(const glm::mat4 & projection, const glm::mat4 & view)
{
	// Get a handle for our "gProjection" uniform
	GLuint projectionMatrixID = glGetUniformLocation(_programID, "gProjection");
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projection[0][0]);
	// Get a handle for our "gView" uniform
	GLuint viewMatrixID = glGetUniformLocation(_programID, "gView");
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);
	// Get a handle for our "gModel" uniform
	GLuint modelMatrixID = glGetUniformLocation(_programID, "gModel");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &_model[0][0]);
}

void Object::calcBoundingSphere() {
	glm::vec3 center = calculateCenter();
	double maxDist = 0; // distance of farthest vertex from center
	double dist;
	glm::vec4 p;
	for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
		p = *it;
		dist = std::sqrt(std::pow(p.x - center.x, 2) +
			std::pow(p.y - center.y, 2) +
			std::pow(p.z - center.z, 2));
		if (dist > maxDist) {
			maxDist = dist;
		}
	}
	_boundingSphere = new Sphere(center, maxDist);
}

glm::vec3 Object::calculateCenter() {
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::vec4 p;
	for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
		center += glm::vec3(*it);
	}
	return center / (float)_vertices.size();
}

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile)
{
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint Object::initShader(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader
	{
		const char*  filename;
		GLenum       type;
		GLchar*      source;
	}
	shaders[2] = {
		{ vShaderFile, GL_VERTEX_SHADER, NULL },
		{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 2; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL)
		{
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);

		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	/* use program object */
	glUseProgram(program);

	return program;
}

void Object::setWorldUniforms(const glm::vec3 camPos, glm::vec3 lightPos, glm::vec4 lightColor)
{
	GLuint cameraID = glGetUniformLocation(_programID, "gEyePosition");
	glUniform3f(cameraID, camPos.x, camPos.y, camPos.z);

	GLuint lightPosID = glGetUniformLocation(_programID, "gLightPosition");
	glUniform4f(lightPosID, lightPos.x, lightPos.y, lightPos.z, 1);

	GLuint lightColorID = glGetUniformLocation(_programID, "gLightColor");
	glUniform4f(lightColorID, lightColor.r, lightColor.g, lightColor.b, 1);
}

GLuint Object::initTexture(const char* fName)
{
	//cv::Mat img = cv::imread(fName);
	//cv::imshow("CV debug", img);
	BImage img(fName);

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 
			0, //level
			GL_RGBA8, // internal representation
			img.width(), img.height(), // texture size
			0, // must be 0
			GL_BGR, GL_UNSIGNED_BYTE, // pixel data format
			img.getImageData()); // pixel data
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
