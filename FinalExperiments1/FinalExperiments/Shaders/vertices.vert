#version 330 core
#pragma optionNV unroll all


const int maxLights = 3;		// *** Change this when adding lights ***
const int maxTextures = 3;		// *** Change this when adding textures ***

struct ShadowMatrices {
	mat4 ShadowMatrix;
};

struct TerrainProperties{
	int width;
	int height;
	int tileFactor;
};

uniform ShadowMatrices shadowMatrices[maxLights];
uniform TerrainProperties terrainProperties;

uniform mat4 VPMatrix;		// view projection matrix
uniform mat4 VMatrix;

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texel;
layout(location = 2) in vec3 in_normal;
layout(location = 4) in int in_isTextured;
layout(location = 5) in int	in_isTransformed;		// if the object is allowed to translate

layout(location = 6) in mat4 ModelMatrix;			// the model translation
layout(location = 10) in vec4 in_color;
layout(location = 11) in mat3 NormalMatrix;


out vec4 shadow_coord[maxLights];
out vec4 vertColor;
out vec3 vertNormal;
out vec2 vertTexCoord;
out vec2 vertTexCoord_xy;
out vec2 vertTexCoord_zy;
out vec2 vertTexCoord_xz;
out vec4 vertPosition;
out vec4 vertWorld_pos;
out mat4 vertModelMatrix;
flat out int vertIsTextured;

void main()
{
	
	if(in_isTransformed == 1)
	{
		vertWorld_pos = ModelMatrix * in_position;
		vec4 eye_pos = VMatrix * vertWorld_pos;
		vec4 clip_pos = VPMatrix * vertWorld_pos;

		for(int light = 0;light < maxLights;light++)
		{
			shadow_coord[light] = shadowMatrices[light].ShadowMatrix * vertWorld_pos;
		}

		vertPosition = VMatrix * ModelMatrix * in_position;

		mat4 MVMatrix = VMatrix * ModelMatrix;
		mat4 normal_matrix = transpose(inverse(MVMatrix));

		vertNormal = normalize(mat3(normal_matrix) * in_normal);

		gl_Position = clip_pos;
	}
	else
	{
		vertWorld_pos = in_position;
		vec4 eye_pos = VMatrix * vertWorld_pos;
		vec4 clip_pos = VPMatrix * vertWorld_pos;

		for(int light = 0;light < maxLights;light++)
		{
			shadow_coord[light] = shadowMatrices[light].ShadowMatrix * vertWorld_pos;
		}

		vertPosition = VMatrix * in_position;
		vertNormal = in_normal;
		gl_Position = clip_pos;
	}

	vertColor = in_color;

	vertTexCoord = in_texel;

	vertTexCoord_xz = vec2((in_position.x / terrainProperties.width),(in_position.z / terrainProperties.height)) * terrainProperties.tileFactor;
	vertTexCoord_xy = vec2((in_position.x / terrainProperties.width),(in_position.y / 100)) * terrainProperties.tileFactor;
	vertTexCoord_zy = vec2((in_position.z / terrainProperties.height),(in_position.y /100)) * terrainProperties.tileFactor;
	//vertTexCoord = in_position.xz;

	vertModelMatrix = ModelMatrix;
	vertIsTextured = in_isTextured;
}


