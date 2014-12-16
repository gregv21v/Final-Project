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

using glm::vec2;

class Button
{
public:
	Button();
	~Button();

	void init();

	void draw(Shader);
	void activateTextures(Shader);
	void deactivateTextures();

	void setTexture(Texture*);

	void flipButton();

private:

	vec2 positions[4];
	vec2 texels[4];

	Color color;

	//---------------------------------------------------------
	// Vertex Arrays and Buffers
	//---------------------------------------------------------
	enum { POS_BUFFER, TEX_BUFFER, NUM_BUFFERS };

	GLuint VAOs[1];
	GLuint Buffers[NUM_BUFFERS];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Vertex Attributes
	//---------------------------------------------------------
	enum {
		vPosition, vTexel, vIsTextured, vColor, vUp
	};
	//---------------------------------------------------------

	//---------------------------------------------------------
	// States
	//---------------------------------------------------------
	bool isTextured;
	bool up;
	//---------------------------------------------------------

	Texture* texture;
};

