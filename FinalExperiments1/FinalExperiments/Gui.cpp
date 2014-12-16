#include "Gui.h"


Gui::Gui()
{
}


Gui::~Gui()
{
}

void Gui::init()
{
	positions[0] = vec2(GUI_HEIGHT, 0);
	positions[1] = vec2(0, 0);
	positions[2] = vec2(GUI_WIDTH, 0);
	positions[3] = vec2(GUI_WIDTH, GUI_HEIGHT);

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
	isTransformed = false;

	toolButton.init();
	//sizeBar.init();
	//roundnessBar.init();

	setupTextures();
}

void Gui::draw(Shader in_shader)
{

	glBindVertexArray(VAOs[0]);

	activateTextures(in_shader);

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vTexel);
	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttrib4fv(vColor, &color.red);
	glVertexAttribI1i(vUp, true);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	deactivateTextures();

	toolButton.draw(in_shader);
	//sizeBar.draw(in_shader);
	//roundnessBar.draw(in_shader);
}

// Texture Stuff
void Gui::setTexture(Texture* inTexture)
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

void Gui::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("modelTex"), 10);
}

void Gui::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}

void Gui::setupTextures()
{
	// Texture Files
	textureFilenames[0] = "Textures/gui.png";
	textureFilenames[1] = "Textures/tool.png";

	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i] = new Texture();
		textures[i]->loadFromFile(textureFilenames[i]);
	}

	textures[0]->load();
	textures[1]->load();

	setTexture(textures[0]);
	toolButton.setTexture(textures[1]);
}

void Gui::setupUniforms(Shader in_shader)
{
	glUniform1i(in_shader.getUniformLocation("gui_properties.width"), GUI_WIDTH);
	glUniform1i(in_shader.getUniformLocation("gui_properties.height"), GUI_HEIGHT);
}

int Gui::getWidth()
{
	return GUI_WIDTH;
}

int Gui::getHeight()
{
	return GUI_HEIGHT;
}