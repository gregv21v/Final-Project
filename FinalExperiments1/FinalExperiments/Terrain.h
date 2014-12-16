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

#define TERR_WIDTH 300
#define TERR_HEIGHT 300


using glm::vec3;

class Terrain
{
public:

	Terrain();
	~Terrain();

	void draw(Shader);
	void init();

	void setSize(int);
	void increaseSize();
	void decreaseSize();

	void setRoundness(int);
	void increaseRoundness();
	void decreaseRoundness();

	void mound(int, int, float);
	void mountain(int,int,float);

	void updatePosBuffer(int, int);
	void updateNormBuffer(int, int);

	void updateNormals(int, int);

	void setIsTextured(bool);
	void setIsTransformed(bool);
	void setTileFactor(int);

	bool getIsTextured();
	bool getIsTransformed();
	int getWidth();
	int getHeight();

	void setupUniforms(Shader);

private:

	//---------------------------------------------------------
	// Height Data
	//---------------------------------------------------------
	GLfloat positions[TERR_WIDTH][TERR_HEIGHT];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Normal Data
	//---------------------------------------------------------
	vec3 normals[TERR_WIDTH][TERR_HEIGHT];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Tools
	//---------------------------------------------------------
	int tool; // 0: Mound; 1: Mountain ...etc
	int size;
	int roundness;
	//---------------------------------------------------------

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
	int tile_factor;		// how many tiled across each direction
	//---------------------------------------------------------
};

