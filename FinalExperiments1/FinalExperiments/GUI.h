#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

class GUI {
public:
	int w, h;
	GLuint _gui_texture_loc;
	GLuint VAOs[4];
	GLuint Buffers[4];
	
	GLfloat GUI_quad[24];
	
	GLfloat size_prog_fill[24];
	GLfloat roundness_prog_fill[24];
	GLfloat direction_button[24];
	
	GLfloat up_tex_coords[8];
	GLfloat down_tex_coords[8];

	bool raise;
	float aspectRatio;

	GUI();
	void _GUI_quad();

	void _GUI_roundness_increase();
	void _GUI_roundness_decrease();
	void _GUI_radius_increase();
	void _GUI_radius_decrease();

	void _GUI_swap_direction();

	void _GUI_update_buffers();
	void _GUI_initialize();
	void _GUI_draw();
	
	
};