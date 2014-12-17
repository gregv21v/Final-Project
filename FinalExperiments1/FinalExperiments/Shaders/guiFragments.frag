#version 330 core
//#pragma optionNV unroll all

uniform sampler2D modelTex;

in vec4 vertColor;
in vec2 vertTexCoord;
flat in int vertIsTextured;

out vec4 fragColor;

void main()
{
	if(vertIsTextured == 1)
	{
		fragColor = texture(modelTex, vertTexCoord);		
	}
	else 
	{
		fragColor = vertColor;
	}

	//fragColor = vec4(1,0,0,1);
}