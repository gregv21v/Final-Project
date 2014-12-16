#include "Button.h"


Button::Button()
{
}


Button::~Button()
{
}

void Button::init()
{
	positions[0] = vec2(1010, 190);
	positions[1] = vec2(1010, 10);
	positions[2] = vec2(1190, 10);
	positions[3] = vec2(1190, 190);

	texels[0] = vec2(0, 0);
	texels[1] = vec2(0, 1);
	texels[2] = vec2(1, 1);
	texels[3] = vec2(1, 0);

	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);

	// POSITION BUFFER
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// TEXEL BUFFER
	glEnableVertexAttribArray(vTexel);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texels), texels, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// set base color
	color.red = .5;
	color.green = .5;
	color.blue = .5;
	color.alpha = 1;

	isTextured = false;
	up = true;
}

void Button::draw(Shader in_shader)
{
	glBindVertexArray(VAOs[0]);

	activateTextures(in_shader);

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vTexel);
	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttrib4fv(vColor, &color.red);
	glVertexAttribI1i(vUp, up);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	deactivateTextures();
}

// Texture Stuff
void Button::setTexture(Texture* inTexture)
{
	texture = inTexture;

	if (texture != nullptr)
	{
		isTextured = true;
	}
	else
	{
		isTextured = false;
	}
}

void Button::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("modelTex"), 10);
}

void Button::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}

void Button::flipButton()
{
	up = !up;
}