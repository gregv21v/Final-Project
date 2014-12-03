#version 330 core
#pragma optionNV unroll all

uniform mat4 VPMatrix;		// view projection matrix
uniform mat4 VMatrix;

const int maxLights = 3;		// *** Change this when adding lights ***

struct ShadowMatrices {
	mat4 ShadowMatrix;
};

uniform ShadowMatrices shadowMatrices[maxLights];

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texel;
layout(location = 2) in vec3 in_normal;
layout(location = 4) in int	in_isTextured;
layout(location = 5) in int	in_isTransformed;		// if the object is allowed to translate

layout(location = 6) in mat4 ModelMatrix;			// the model translation
layout(location = 10) in vec4 in_color;
layout(location = 11) in mat3 NormalMatrix;


out vec4 shadow_coord[maxLights];
out vec4 vertColor;
out vec3 vertNormal;
out vec2 vertTexCoord;
out vec4 vertPosition;

flat out int vertIsTextured;

void main()
{
	
	if(in_isTransformed == 1)
	{
		vec4 world_pos = ModelMatrix * in_position;
		vec4 eye_pos = VMatrix * world_pos;
		vec4 clip_pos = VPMatrix * world_pos;

		for(int light = 0;light < maxLights;light++)
		{
			shadow_coord[light] = shadowMatrices[light].ShadowMatrix * world_pos;
		}

		vertPosition = VMatrix * ModelMatrix * in_position;

		mat4 MVMatrix = VMatrix * ModelMatrix;
		mat4 normal_matrix = transpose(inverse(MVMatrix));

		vertNormal = normalize(mat3(normal_matrix) * in_normal);

		gl_Position = clip_pos;
	}
	else
	{
		vec4 world_pos = in_position;
		vec4 eye_pos = VMatrix * world_pos;
		vec4 clip_pos = VPMatrix * world_pos;

		for(int light = 0;light < maxLights;light++)
		{
			shadow_coord[light] = shadowMatrices[light].ShadowMatrix * world_pos;
		}

		vertPosition = VMatrix * in_position;
		vertNormal = in_normal;
		gl_Position = clip_pos;
	}

	vertColor = in_color;
	vertTexCoord = in_texel;
	vertIsTextured = in_isTextured;
}