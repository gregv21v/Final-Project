/*
	Texture.h
*/

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <vector>

#include "vgl.h"

#include "lodepng.h"
using namespace std;
class Texture
{
	friend class TextureCache;
public:
	Texture();
	~Texture();

	void loadFromFile(std::string);
	void load();
	void activate(GLint, int);
	void deactivate();

	int getWidth();
	int getHeight();

private:
	GLuint _id;

	unsigned int _width;
	unsigned int _height;

	std::vector<unsigned char> _image;

	std::string path;
};