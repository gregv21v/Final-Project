#version 330 core
#pragma optionNV unroll all

const int maxLights = 1;		// *** Change this when adding lights ***
const int maxTextures = 3;		// *** Change this when adding textures ***

struct ShadowMatrices {
	mat4 ShadowMatrix;
};

struct TerrainProperties{
	int width;
	int height;
	int tileFactor;
};

struct FloorProperties{
	int width;
	int height;
};

uniform ShadowMatrices shadowMatrices[maxLights];
uniform TerrainProperties terrainProperties;
uniform FloorProperties floorProperties;

uniform mat4 VPMatrix;		// view projection matrix
uniform mat4 VMatrix;

uniform int Type;

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texel;
layout(location = 2) in vec3 in_normal;
layout(location = 4) in int in_isTextured;
layout(location = 5) in int	in_isTransformed;		// if the object is allowed to translate

layout(location = 6) in mat4 ModelMatrix;			// the model translation
layout(location = 10) in vec4 in_color;
layout(location = 11) in mat3 NormalMatrix;

/*
out vec4 shadow_coord[maxLights];
out vec4 vertColor;
out vec3 vertNormal;
out vec2 vertTexCoord;
out vec2 vertTexCoord_xy;
out vec2 vertTexCoord_zy;
out vec2 vertTexCoord_xz;
out vec4 vertPosition;
out vec4 world_pos;
out int vertIsTextured;
*/

out VS_GS_INTERFACE {	
	vec4 shadow_coord[maxLights];
	vec4 vertColor;
	vec3 vertNormal;
	vec2 vertTexCoord;
	vec2 vertTexCoord_xy;
	vec2 vertTexCoord_zy;
	vec2 vertTexCoord_xz;
	vec4 vertPosition;
	vec4 world_pos;
	flat int vertIsTextured;
} geometry;

void main()
{
	/*	**** WITHOUT GEOM SHADER *****
	if(in_isTransformed == 1)
	{
		world_pos = ModelMatrix * in_position;
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
		world_pos = in_position;
		vec4 eye_pos = VMatrix * world_pos;
		vec4 clip_pos = VPMatrix * world_pos;

		for(int light = 0;light < maxLights;light++)
		{
			shadow_coord[light] = shadowMatrices[light].ShadowMatrix * world_pos;
		}

		vertPosition = VMatrix * in_position;

		mat4 normal_matrix = transpose(inverse(VMatrix));

		vertNormal = normalize(mat3(normal_matrix) * in_normal);
		vertNormal = in_normal;
		gl_Position = clip_pos;
	}

	vertColor = in_color;

	switch(Type)
	{
	case 0:
		vertTexCoord = in_texel;
		break;
	case 1:
		vertTexCoord = vec2(
				((in_position.x + (floorProperties.width - terrainProperties.width) / 2) / floorProperties.width) * (floorProperties.width / (terrainProperties.width / terrainProperties.tileFactor)),
				((in_position.z + (floorProperties.height - terrainProperties.height) / 2) / floorProperties.height) * (floorProperties.height / (terrainProperties.height / terrainProperties.tileFactor))
			);
		break;
	case 2:
		vertTexCoord_xz = vec2((in_position.x / terrainProperties.width),(in_position.z / terrainProperties.height)) * terrainProperties.tileFactor;
		vertTexCoord_xy = vec2((in_position.x / terrainProperties.width),(in_position.y / 100)) * terrainProperties.tileFactor;
		vertTexCoord_zy = vec2((in_position.z / terrainProperties.height),(in_position.y /100)) * terrainProperties.tileFactor;
		break;
	default:
		break;
	}

	vertIsTextured = in_isTextured;

	*/

	if(in_isTransformed == 1)
	{
		geometry.world_pos = ModelMatrix * in_position;
		vec4 eye_pos = VMatrix * geometry.world_pos;
		vec4 clip_pos = VPMatrix * geometry.world_pos;

		for(int light = 0;light < maxLights;light++)
		{
			geometry.shadow_coord[light] = shadowMatrices[light].ShadowMatrix * geometry.world_pos;
		}

		geometry.vertPosition = VMatrix * ModelMatrix * in_position;

		mat4 MVMatrix = VMatrix * ModelMatrix;
		mat4 normal_matrix = transpose(inverse(MVMatrix));

		geometry.vertNormal = normalize(mat3(normal_matrix) * in_normal);

		gl_Position = clip_pos;
	}
	else
	{
		geometry.world_pos = in_position;
		vec4 eye_pos = VMatrix * geometry.world_pos;
		vec4 clip_pos = VPMatrix * geometry.world_pos;

		for(int light = 0;light < maxLights;light++)
		{
			geometry.shadow_coord[light] = shadowMatrices[light].ShadowMatrix * geometry.world_pos;
		}

		geometry.vertPosition = VMatrix * in_position;

		mat4 normal_matrix = transpose(inverse(VMatrix));

		geometry.vertNormal = normalize(mat3(normal_matrix) * in_normal);
		geometry.vertNormal = in_normal;
		gl_Position = clip_pos;
	}

	geometry.vertColor = in_color;

	switch(Type)
	{
	case 0:
		geometry.vertTexCoord = in_texel;
		break;
	case 1:
		geometry.vertTexCoord = vec2(
				((in_position.x + (floorProperties.width - terrainProperties.width) / 2) / floorProperties.width) * (floorProperties.width / (terrainProperties.width / terrainProperties.tileFactor)),
				((in_position.z + (floorProperties.height - terrainProperties.height) / 2) / floorProperties.height) * (floorProperties.height / (terrainProperties.height / terrainProperties.tileFactor))
			);
		break;
	case 2:
		geometry.vertTexCoord_xz = vec2((in_position.x / terrainProperties.width),(in_position.z / terrainProperties.height)) * terrainProperties.tileFactor;
		geometry.vertTexCoord_xy = vec2((in_position.x / terrainProperties.width),(in_position.y / terrainProperties.width)) * terrainProperties.tileFactor;
		geometry.vertTexCoord_zy = vec2((in_position.z / terrainProperties.height),(in_position.y /terrainProperties.height)) * terrainProperties.tileFactor;
		break;
	default:
		break;
	}

	geometry.vertIsTextured = in_isTextured;
}