#version 330 core

uniform int Type;
const int maxLights = 3;

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
	/*
	if(Type == 2)
	{
		vec4 middle_gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position);
		vec4 middle_world_pos = (geometry[0].world_pos + geometry[1].world_pos + geometry[2].world_pos) / 3;
		vec4 middle_vertPosition = (geometry[0].vertPosition + geometry[1].vertPosition + geometry[2].vertPosition) / 3;
		vec4 middle_vertColor = (geometry[0].vertColor + geometry[1].vertColor + geometry[2].vertColor) / 3;
		vec3 middle_vertNormal = (geometry[0].vertNormal + geometry[1].vertNormal + geometry[2].vertNormal) / 3;
		vec2 middle_vertTexCoord = (geometry[0].vertTexCoord + geometry[1].vertTexCoord + geometry[2].vertTexCoord) / 3;
		vec2 middle_vertTexCoord_xy = (geometry[0].vertTexCoord_xy + geometry[1].vertTexCoord_xy + geometry[2].vertTexCoord_xy) / 3;
		vec2 middle_vertTexCoord_zy = (geometry[0].vertTexCoord_zy + geometry[1].vertTexCoord_zy + geometry[2].vertTexCoord_zy) / 3;
		vec2 middle_vertTexCoord_xz = (geometry[0].vertTexCoord_xz + geometry[1].vertTexCoord_xz + geometry[2].vertTexCoord_xz) / 3;
		int middle_vertIsTextured = geometry[0].vertIsTextured;
		vec4 middle_shadow_coord[maxLights];

		for(int k = 0;k<maxLights;k++)
		{
			middle_shadow_coord[k] = (geometry[0].shadow_coord[k] + geometry[1].shadow_coord[k] + geometry[2].shadow_coord[k]) / 3;
		}

		for(int i = 0;i<3;i++)
		{
			for(int j = 0;j<2;j++)
			{
				gl_Position = gl_in[i + j].gl_Position;

				fragment.world_pos = geometry[i + j].world_pos;
				fragment.vertPosition = geometry[i + j].vertPosition;
				fragment.vertColor = geometry[i + j].vertColor;
				fragment.vertNormal = geometry[i + j].vertNormal;
				fragment.vertTexCoord = geometry[i + j].vertTexCoord;
				fragment.vertTexCoord_xy = geometry[i + j].vertTexCoord_xy;
				fragment.vertTexCoord_zy = geometry[i + j].vertTexCoord_zy;
				fragment.vertTexCoord_xz = geometry[i + j].vertTexCoord_xz;
				fragment.vertIsTextured = geometry[i + j].vertIsTextured;

				for(int k = 0;k<maxLights;k++)
				{
					fragment.shadow_coord[k] = geometry[i + j].shadow_coord[k];
				}

				EmitVertex();
			}

			gl_Position = middle_gl_Position;

			fragment.world_pos = middle_world_pos;
			fragment.vertPosition = middle_vertPosition;
			fragment.vertColor = middle_vertColor;
			fragment.vertNormal = middle_vertNormal;
			fragment.vertTexCoord = middle_vertTexCoord;
			fragment.vertTexCoord_xy = middle_vertTexCoord_xy;
			fragment.vertTexCoord_zy = middle_vertTexCoord_zy;
			fragment.vertTexCoord_xz = middle_vertTexCoord_xz;
			fragment.vertIsTextured = middle_vertIsTextured;

			for(int k = 0;k<maxLights;k++)
			{
				fragment.shadow_coord[k] = middle_shadow_coord[k];
			}

			EmitVertex();
		}
	}
	else
	{

		/*** PASS THROUGH ***/
		for(int i = 0;i<gl_in.length();i++)
		{
			gl_Position = gl_in[i].gl_Position;

			fragment.world_pos = geometry[i].world_pos;
			fragment.vertPosition = geometry[i].vertPosition;
			fragment.vertColor = geometry[i].vertColor;
			fragment.vertNormal = geometry[i].vertNormal;
			fragment.vertTexCoord = geometry[i].vertTexCoord;
			fragment.vertTexCoord_xy = geometry[i].vertTexCoord_xy;
			fragment.vertTexCoord_zy = geometry[i].vertTexCoord_zy;
			fragment.vertTexCoord_xz = geometry[i].vertTexCoord_xz;
			fragment.vertIsTextured = geometry[i].vertIsTextured;

			for(int j = 0;j<maxLights;j++)
			{
				fragment.shadow_coord[j] = geometry[i].shadow_coord[j];
			}

			EmitVertex();
		}
//	}



}