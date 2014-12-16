#version 330 core
#pragma optionNV unroll all

uniform sampler2D modelTex;

in vec4 vertColor;
in vec2 vertTexCoord;
in bool vertIsTextured;

out vec4 fragColor;

void main()
{
	if(vertIsTextured)
		{
			fragColor = texture(modelTex, vertTexCoord);		
		}
		else 
		{
			fragColor = vertColor;
		}
}