#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vgl.h"

#include "Shader.h"
#include "Color.h"
#include "Texture.h"

#define MAX_WIDTH 4500
#define MAX_HEIGHT 4500
#define DEPTH 0

using glm::vec3;
using glm::vec4;

class Floor
{

public:

	Floor();
	~Floor();

	void init(float,float);
	void draw(Shader);

	void activateTextures(Shader);
	void deactivateTextures();

	void setTexture(Texture*);

	void setupUniforms(Shader);

private:

	vec4 positions[12];

	vec3 normals[12];

	Color color;

	//---------------------------------------------------------
	// Vertex Arrays and Buffers
	//---------------------------------------------------------
	enum { POS_BUFFER, NORM_BUFFER, INDEX_BUFFER, NUM_BUFFERS };

	GLuint VAOs[1];
	GLuint Buffers[NUM_BUFFERS];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Vertex Attributes
	//---------------------------------------------------------
	enum {
		vPosition, vTexel, vNormal, vTexture, vIsTextured, vIsTransformed,
		vModelMatrix0, vModelMatrix1, vModelMatrix2, vModelMatrix3, vColor,
		vNormalMatrix0, vNormalMatrix1, vNormalMatrix2
	};
	//---------------------------------------------------------

	//---------------------------------------------------------
	// States
	//---------------------------------------------------------
	bool isTextured;
	bool isTransformed;
	//---------------------------------------------------------

	Texture* texture;
};

