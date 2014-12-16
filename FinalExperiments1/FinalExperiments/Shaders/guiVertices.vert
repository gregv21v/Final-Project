#version 330 core
#pragma optionNV unroll all

struct GUIProperties{
	int width;
	int height;
};

uniform GUIProperties gui_properties;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texel;
layout(location = 2) in bool in_isTextured;
layout(location = 3) in vec4 in_color;
layout(location = 4) in bool in_up;

out vec4 vertColor;
out vec2 vertTexCoord;
out bool vertIsTextured;

void main()
{
	vertColor = in_color;

	gl_Position = vec4(vec2(in_position.x / gui_properties.width,in_position.y / gui_properties.height) * 2 - 1.0,0.0,1.0);

	if(in_up)
	{
		vertTexCoord = in_texel;
	}
	else
	{
		vertTexCoord.x = in_texel.x;
		vertTexCoord.y = -in_texel.y;
	}

	vertIsTextured = in_isTextured;
}