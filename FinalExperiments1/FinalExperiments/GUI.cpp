#include "GUI.h"
#include <cstdio>

GUI::GUI()
{
}

void GUI::_GUI_initialize()
{
	raise = true;
	GLuint GUI_texture;

	GLfloat GUI_quad[24] = {
		-1.0, 1.0, 0.0, 1.0,
		1.0, 1.0, 0.0, 1.0,
		1.0, 0.833, 0.0, 1.0,
		-1.0, 0.833, 0.0, 1.0,

		0.0, 0.0,
		1.0, 0.0,
		1.0, 0.166,
		0.0, 0.166
	};

	GLfloat size_prog_fill[24] = {
		0.265, 0.0267, 0.0, 1.0,
		0.47875, 0.0267, 0.0, 1.0,
		0.47875, 0.07, 0.0, 1.0,
		0.265, 0.07, 0.0, 1.0,

		1.0, 1.0,
		1.0, 1.0,
		1.0, 1.0,
		1.0, 1.0
	};
	GLfloat roundness_prog_fill[24] = {
		0.265, 0.0933, 0.0, 1.0,
		0.47875, 0.0933, 0.0, 1.0,
		0.47875, 0.1366, 0.0, 1.0,
		0.265, 0.1366, 0.0, 1.0,

		1.0, 1.0,
		1.0, 1.0,
		1.0, 1.0,
		1.0, 1.0
	};
	GLfloat direction_button[24] = {
		0.8125, 0.01, 0.0, 1.0,
		0.88, 0.01, 0.0, 1.0,
		0.88, 0.1, 0.0, 1.0,
		0.8125, 0.1, 0.0, 1.0,

		// Up button tex coords
		0.48125, 0.535,
		0.54875, 0.535,
		0.54875, 0.625,
		0.48125, 0.625
	};

	GLfloat up_tex_coords[8] = {
		0.48125, 0.535,
		0.54875, 0.535,
		0.54875, 0.625,
		0.48125, 0.625
	};


	GLfloat down_tex_coords[8] = {
		0.48125, 0.6433,
		0.54875, 0.6433,
		0.54875, 0.7333,
		0.48125, 0.7333
	};

	
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

	_GUI_quad();
	_GUI_update_buffers();
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(size_prog_fill), size_prog_fill, GL_DYNAMIC_DRAW);
	
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roundness_prog_fill), roundness_prog_fill, GL_DYNAMIC_DRAW);
	
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(direction_button), direction_button, GL_DYNAMIC_DRAW);
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
	if (size_prog_fill[4] >= 0.47875)
		;
	else
	{
		size_prog_fill[4] += 0.0021375;
		size_prog_fill[8] += 0.0021375;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_radius_decrease()
{
	if (size_prog_fill[4] <= 0.265)
		;
	else
	{
		size_prog_fill[4] -= 0.0021375;
		size_prog_fill[8] -= 0.0021375;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_roundness_increase()
{
	if (roundness_prog_fill[4] >= 0.47875)
		;
	else
	{
		roundness_prog_fill[4] += 0.0021375;
		roundness_prog_fill[8] += 0.0021375;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_roundness_decrease()
{
	if (roundness_prog_fill[4] <= 0.265)
		;
	else
	{
		roundness_prog_fill[4] -= 0.0021375;
		roundness_prog_fill[8] -= 0.0021375;
	}
	_GUI_update_buffers();
}

void GUI::_GUI_draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindVertexArray(VAOs[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(VAOs[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}