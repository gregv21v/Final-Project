#version 330 core

uniform mat4 VPMatrix;		// view projection matrix

layout(location = 0) in vec4 in_position;
layout(location = 5) in int	in_isTransformed;		// if the object is allowed to translate
layout(location = 6) in mat4 ModelMatrix;			// the model translation

void main()
{
	if(in_isTransformed == 1)
	{
		gl_Position = VPMatrix * ModelMatrix * in_position;
	}
	else
	{
		gl_Position = VPMatrix * in_position;
	}
}