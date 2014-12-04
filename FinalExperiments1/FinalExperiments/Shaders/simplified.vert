#version 330 core


uniform mat4 VMatrix;



layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;



out vec4 vertColor;


void main()
{
	gl_Position = VMatrix * in_position;
	vertColor = in_color;
}