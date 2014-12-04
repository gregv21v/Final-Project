#pragma once
#include "Model.h"
#include "Color.h"
#include <glm\glm.hpp>


class Line : public Model
{
public:
	Line();
	~Line();


	void draw();
	void init();

	void setStart(glm::vec3 start);
	void setEnd(glm::vec3 end);
	
private:
	glm::vec3 _start;
	glm::vec3 _end;

	Color colors[2];

	GLuint colorBuffer;

};

