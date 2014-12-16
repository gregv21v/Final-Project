/*
	Shader.h
*/

#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include <iostream>
#include "LoadShaders.h"

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	void init();
	void init(std::string vertPath, std::string fragPath);
	void init(std::string vertPath, std::string fragPath,std::string geomPath);
	GLint getUniformLocation(std::string);
	void use();
	void unuse();


private:
	GLuint _id;


};

