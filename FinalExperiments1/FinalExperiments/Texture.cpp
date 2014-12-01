#include "Texture.h"
#include <GL\freeglut.h>


Texture::Texture() :
	_id(0),
	_width(0),
	_height(0)
{
	glGenTextures(1, &_id);
	//cout << "Tex ID: " << _id << endl;
}

Texture::~Texture()
{
	if (_id != 0)
		glDeleteTextures(1, &_id);
}

void Texture::activate(GLint uniformLocation)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
	glUniform1i(uniformLocation, 0);
}

void Texture::deactivate()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::loadFromFile(std::string filename)
{
	// loads the file into memory
	std::vector<unsigned char> buffer;
	lodepng::load_file(buffer, filename);

	// decodes the texture
	unsigned error = lodepng::decode(_image, _width, _height, buffer);

	path = filename;

	if (buffer.size() == 0)
	{
		std::cout << "No texture was loaded from " << filename << std::endl;
	}
}

void Texture::load()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D,
			0,									// level
			GL_RGBA,							// internal format
			_width, _height,					// width, height
			0,									// border
			GL_RGBA, GL_UNSIGNED_BYTE,			// format, type
			&(_image[0]));						// data

		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}