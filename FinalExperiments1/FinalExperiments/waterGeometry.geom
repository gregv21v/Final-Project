#version 330 core

uniform mat4 VMatrix;
uniform float WaterWidth;
uniform float WaterHeight; 

const int maxLights = 1;

layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

in VS_GS_INTERFACE {	
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
} geometry[];

out GS_FS_INTERFACE {
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
} fragment;

void main()
{

	float change_x = WaterWidth / 18;
	float change_z = WaterHeight;

	for(int i = 0;i<36;i++)
	{
		fragment.world_pos = geometry[0].world_pos + vec4(i *change_x,0,(i%2) * change_z,0;
		fragment.vertPosition = VMatrix * fragment.world_pos;;
		fragment.vertColor = geometry[0].vertColor;
		fragment.vertNormal = geometry[0].vertNormal;
		fragment.vertTexCoord = geometry[0].vertTexCoord;
		fragment.vertTexCoord_xy = geometry[0].vertTexCoord_xy;
		fragment.vertTexCoord_zy = geometry[0].vertTexCoord_zy;
		fragment.vertTexCoord_xz = geometry[0].vertTexCoord_xz;
		fragment.vertIsTextured = geometry[0].vertIsTextured;

		gl_Position = fragment.world_pos;

		for(int j = 0;j<maxLights;j++)
		{
			fragment.shadow_coord[j] = geometry[0].shadow_coord[j];
		}

		EmitVertex();
	}
}