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
#include "Button.h"
#include "ProgressBar.h"

#define NUM_TEXTURES 2
#define GUI_WIDTH 1200
#define GUI_HEIGHT 200

using glm::vec2;

class Gui
{

public:

	Gui();
	~Gui();

	void init();

	void draw(Shader);
	void activateTextures(Shader);
	void deactivateTextures();

	void setTexture(Texture*);
	void setupTextures();

	void setupUniforms(Shader);

	int getWidth();
	int getHeight();

private:

	vec2 positions[4];
	vec2 texels[4];

	Button toolButton;

	ProgressBar sizeBar;
	ProgressBar roundnessBar;

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
	bool isTransformed;
	//---------------------------------------------------------

	//---------------------------------------
	// Textures
	//---------------------------------------
	std::string textureFilenames[NUM_TEXTURES];
	Texture* textures[NUM_TEXTURES];
	Texture* texture;
	//---------------------------------------
};

