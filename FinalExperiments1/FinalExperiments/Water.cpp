#include "Water.h"


Water::Water()
{
}


Water::~Water()
{
}


void Water::setUniforms(Shader in_shader)
{
	glUniform1i(in_shader.getUniformLocation("isWater"), true);
}


