#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>

#include "vgl.h"

#include "Shader.h"

#define TERR_WIDTH 100
#define TERR_HEIGHT 100

class Terrain
{
public:

	Terrain();
	~Terrain();

	void draw(Shader);
	void init();

private:

	//---------------------------------------------------------
	// Height Data
	//---------------------------------------------------------
	GLfloat positions[TERR_WIDTH][TERR_HEIGHT];
	//---------------------------------------------------------

	int tool; // 0: Mound; 1: Mountain ...etc

	//---------------------------------------------------------
	// Vertex Arrays and Buffers
	//---------------------------------------------------------
	enum { POS_BUFFER, INDEX_BUFFER, NUM_BUFFERS };

	GLuint VAOs[1];
	GLuint Buffers[NUM_BUFFERS];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Vertex Attributes
	//---------------------------------------------------------
	enum { vPosition };
	//---------------------------------------------------------

};

