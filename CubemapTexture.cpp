#include "CubemapTexture.h"
#include "bimage.h"

static const GLenum types[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
								GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
								GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
								GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
								GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
								GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

CubemapTexture::CubemapTexture(const std::string& directory,
	const std::string& posXFilename,
	const std::string& negXFilename,
	const std::string& posYFilename,
	const std::string& negYFilename,
	const std::string& posZFilename,
	const std::string& negZFilename)
{
	std::string::const_iterator it = directory.end();
	it--;
	std::string BaseDir = (*it == '/') ? directory : directory + "/";

	m_fileNames[0] = BaseDir + posXFilename;
	m_fileNames[1] = BaseDir + negXFilename;
	m_fileNames[2] = BaseDir + posYFilename;
	m_fileNames[3] = BaseDir + negYFilename;
	m_fileNames[4] = BaseDir + posZFilename;
	m_fileNames[5] = BaseDir + negZFilename;

	m_textureObj = 0;
}

CubemapTexture::~CubemapTexture()
{
	if (m_textureObj != 0) {
		glDeleteTextures(1, &m_textureObj);
	}
}

void CubemapTexture::load()
{
	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

	BImage* pImage = NULL;

	for (unsigned int i = 0; i < 6; i++) {
		pImage = new BImage(m_fileNames[i].c_str());

		glTexImage2D(types[i], 
			0,									// level
			GL_RGB,								// internal representation
			pImage->width(), pImage->height(),  // texture size
			0,									// must be 0
			GL_BGR, GL_UNSIGNED_BYTE,			// pixel data format
			pImage->getImageData());			// pixel data
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		delete pImage;
	}
}

void CubemapTexture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}