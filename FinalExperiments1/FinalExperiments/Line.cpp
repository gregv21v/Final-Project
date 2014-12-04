#include "Line.h"


Line::Line() : colorBuffer(0)
{
	colors[0].setColor(1, 0, 0, 1);
	colors[1].setColor(0, 1, 0, 1);
	vertices.resize(2);

	
}


Line::~Line()
{
}

void Line::setStart(glm::vec3 start)
{
	_start = start;
}
void Line::setEnd(glm::vec3 end)
{
	_end = end;
}

void Line::init()
{
	Model::init();

	glBindVertexArray(VAOs[0]);

	glGenBuffers(1, &colorBuffer);
	glEnableVertexAttribArray(vColor);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
}


void Line::draw()
{
	vertices[0] = vec4(_start, 1.0f);
	vertices[1] = vec4(_end, 1.0f);

	init();

	glBindVertexArray(VAOs[0]);

	glEnableVertexAttribArray(vColor);
	glEnableVertexAttribArray(vPosition);
	
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, vertices.size());
}