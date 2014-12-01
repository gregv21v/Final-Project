/*
	ShadowMap.h
*/

#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"

#define DEPTH_TEXTURE_SIZE 4096
#define FRUSTUM_DEPTH 1000

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class ShadowMap
{


public:
	ShadowMap();
	~ShadowMap();

	void render(Shader); // set up uniforms
	void updateView(vec3);
	void use(Shader,int);
	void init(); 
	void draw();
	void endDraw();

private:

	GLuint depth_texture;
	GLuint _fboID;

	mat4 frustum;
	mat4 view;
	mat4 scaleBias;
};

