#include "GUI.h"
#include <cstdio>

GUI::GUI()
{
}

void GUI::_GUI_initialize()
{
	raise = true;
	GLuint GUI_texture;

	// QUAD
	GUI_quad[0] = -1.0;
	GUI_quad[1] = 1.0;
	GUI_quad[2] = 0.0;
	GUI_quad[3] = 1.0;
	GUI_quad[4] = 1.0;
	GUI_quad[5] = 1.0;
	GUI_quad[6] = 0.0;
	GUI_quad[7] = 1.0;
	GUI_quad[8] = 1.0;
	GUI_quad[9] = 0.833;
	GUI_quad[10] = 0.0;
	GUI_quad[11] = 1.0;
	GUI_quad[12] = -1.0;
	GUI_quad[13] = 0.833;
	GUI_quad[14] = 0.0;
	GUI_quad[15] = 1.0;

	GUI_quad[16] = 0.0;
	GUI_quad[17] = 0.0;
	GUI_quad[18] = 1.0;
	GUI_quad[19] = 0.0;
	GUI_quad[20] = 1.0;
	GUI_quad[21] = 0.16;
	GUI_quad[22] = 0.0;
	GUI_quad[23] = 0.166;
	
	// SIZE
	size_prog_fill[0] = BAR_LOW;
	size_prog_fill[1] = SIZE_BAR_TOP;
	size_prog_fill[2] = 0.0;
	size_prog_fill[3] = 1.0;
	size_prog_fill[4] = BAR_LOW;
	size_prog_fill[5] = SIZE_BAR_TOP;
	size_prog_fill[6] = 0.0;
	size_prog_fill[7] = 1.0;
	size_prog_fill[8] = BAR_LOW;
	size_prog_fill[9] = SIZE_BAR_BOTTOM;
	size_prog_fill[10] = 0.0;
	size_prog_fill[11] = 1.0;
	size_prog_fill[12] = BAR_LOW;
	size_prog_fill[13] = SIZE_BAR_BOTTOM;
	size_prog_fill[14] = 0.0;
	size_prog_fill[15] = 1.0;

	size_prog_fill[16] = 1.0;
	size_prog_fill[17] = 1.0;
	size_prog_fill[18] = 1.0;
	size_prog_fill[19] = 1.0;
	size_prog_fill[20] = 1.0;
	size_prog_fill[21] = 1.0;
	size_prog_fill[22] = 1.0;
	size_prog_fill[23] = 1.0;

	// ROUNDNESS
	roundness_prog_fill[0] = BAR_LOW;
	roundness_prog_fill[1] = ROUND_BAR_TOP;
	roundness_prog_fill[2] = 0.0;
	roundness_prog_fill[3] = 1.0;
	roundness_prog_fill[4] = BAR_LOW;
	roundness_prog_fill[5] = ROUND_BAR_TOP;
	roundness_prog_fill[6] = 0.0;
	roundness_prog_fill[7] = 1.0;
	roundness_prog_fill[8] = BAR_LOW;
	roundness_prog_fill[9] = ROUND_BAR_BOTTOM;
	roundness_prog_fill[10] = 0.0;
	roundness_prog_fill[11] = 1.0;
	roundness_prog_fill[12] = BAR_LOW;
	roundness_prog_fill[13] = ROUND_BAR_BOTTOM;
	roundness_prog_fill[14] = 0.0;
	roundness_prog_fill[15] = 1.0;

	roundness_prog_fill[16] = 1.0;
	roundness_prog_fill[17] = 1.0;
	roundness_prog_fill[18] = 1.0;
	roundness_prog_fill[19] = 1.0;
	roundness_prog_fill[20] = 1.0;
	roundness_prog_fill[21] = 1.0;
	roundness_prog_fill[22] = 1.0;
	roundness_prog_fill[23] = 1.0;

	direction_button[0] = -1 + 2 * 0.8125;
	direction_button[1] = 1 - 0.01;
	direction_button[2] = 0.0;
	direction_button[3] = 1.0;
	direction_button[4] = -1 + 2 * 0.88;
	direction_button[5] = 1 - 0.01;
	direction_button[6] = 0.0;
	direction_button[7] = 1.0;
	direction_button[8] = -1 + 2 * 0.88;
	direction_button[9] = 1 - 0.1;
	direction_button[10] = 0.0;
	direction_button[11] = 1.0;
	direction_button[12] = -1 + 2 * 0.8125;
	direction_button[13] = 1 - 0.1;
	direction_button[14] = 0.0;
	direction_button[15] = 1.0;

	direction_button[16] = 0.48125;
	direction_button[17] = 0.535;
	direction_button[18] = 0.54875;
	direction_button[19] = 0.535;
	direction_button[20] = 0.54875;
	direction_button[21] = 0.625;
	direction_button[22] = 0.48125;
	direction_button[23] = 0.625;

	down_tex_coords[0] = 0.48125;
	down_tex_coords[1] = 0.535;
	down_tex_coords[2] = 0.54875;
	down_tex_coords[3] = 0.535;
	down_tex_coords[4] = 0.54875;
	down_tex_coords[5] = 0.625;
	down_tex_coords[6] = 0.48125;
	down_tex_coords[7] = 0.625;

	up_tex_coords[0] = 0.48125;
	up_tex_coords[1] = 0.625;
	up_tex_coords[2] = 0.54875;
	up_tex_coords[3] = 0.625;
	up_tex_coords[4] = 0.54875;
	up_tex_coords[5] = 0.535;
	up_tex_coords[6] = 0.48125;
	up_tex_coords[7] = 0.535;
	
	glGenTextures(1, &_gui_texture_loc);
	glGenVertexArrays(4, VAOs);
	glGenBuffers(4, Buffers);
	
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GUI_quad), GUI_quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) (16 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(size_prog_fill), size_prog_fill, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roundness_prog_fill), roundness_prog_fill, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(direction_button), direction_button, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//_GUI_quad();
	//_GUI_update_buffers();

	isTextured = false;
}

void GUI::_GUI_quad()
{
	// GUI Backdrop 
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GUI_quad), GUI_quad, GL_STATIC_DRAW);
	
}

void GUI::_GUI_update_buffers()
{
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(size_prog_fill), size_prog_fill);
	
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roundness_prog_fill), roundness_prog_fill);
	
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(direction_button), direction_button);
}

void GUI::_GUI_swap_direction()
{
	if (raise)
	{
		direction_button[16] = down_tex_coords[0];
		direction_button[17] = down_tex_coords[1];
		direction_button[18] = down_tex_coords[2];
		direction_button[19] = down_tex_coords[3];
		direction_button[20] = down_tex_coords[4];
		direction_button[21] = down_tex_coords[5];
		direction_button[22] = down_tex_coords[6];
		direction_button[23] = down_tex_coords[7];
		raise = false;
	}
	else
	{
		direction_button[16] = up_tex_coords[0];
		direction_button[17] = up_tex_coords[1];
		direction_button[18] = up_tex_coords[2];
		direction_button[19] = up_tex_coords[3];
		direction_button[20] = up_tex_coords[4];
		direction_button[21] = up_tex_coords[5];
		direction_button[22] = up_tex_coords[6];
		direction_button[23] = up_tex_coords[7];
		raise = true;
	}

	_GUI_update_buffers();
}
void GUI::_GUI_radius_increase()
{
	cout << size_prog_fill[4] << endl;

	if (size_prog_fill[4] >= BAR_HIGH)
		;
	else
	{
		cout << "Increase\n";
		size_prog_fill[4] += BAR_WIDTH / 50;
		size_prog_fill[8] += BAR_WIDTH / 50;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_radius_decrease()
{
	if (size_prog_fill[4] <= BAR_LOW)
		;
	else
	{
		size_prog_fill[4] -= BAR_WIDTH / 50;
		size_prog_fill[8] -= BAR_WIDTH / 50;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_roundness_increase()
{
	if (roundness_prog_fill[4] >= BAR_HIGH)
		;
	else
	{
		roundness_prog_fill[4] += BAR_WIDTH / 20;
		roundness_prog_fill[8] += BAR_WIDTH / 20;
	}
	_GUI_update_buffers();

	
}

void GUI::_GUI_roundness_decrease()
{
	if (roundness_prog_fill[4] <= BAR_LOW)
		;
	else
	{
		roundness_prog_fill[4] -= BAR_WIDTH / 20;
		roundness_prog_fill[8] -= BAR_WIDTH / 20;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_draw(Shader in_shader)
{
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	activateTextures(in_shader);

	glBindVertexArray(VAOs[1]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribI1i(2, isTextured);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[2]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribI1i(2, isTextured);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[3]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribI1i(2, isTextured);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribI1i(2, isTextured);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	deactivateTextures();
}

// Texture Stuff
void GUI::setTexture(Texture* inTexture)
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

void GUI::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("modelTex"), 10);
}

void GUI::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}