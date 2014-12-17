#version 330 core

uniform mat4 VMatrix;
uniform mat4 VPMatrix;
uniform int Type;

const int maxLights = 1;

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

struct Vertex 
{
	vec4 position;
	vec4 color;
};

struct Triangle 
{
	Vertex verts[3];
	vec3 normal;
};

uniform bool IsWater;
uniform float time;

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

const float PI = 3.14159265358979323846264;

void passThrough()
{
	for(int n = 0; n < gl_in.length(); n++)
	{
		gl_Position = gl_in[n].gl_Position;

		fragment.vertPosition = geometry[n].vertPosition;
		fragment.vertColor = geometry[n].vertColor;
		fragment.vertColor.a = 1.0;
		fragment.vertNormal = geometry[n].vertNormal;
		fragment.vertTexCoord = geometry[n].vertTexCoord;
		fragment.vertTexCoord_xy = geometry[n].vertTexCoord_xy;
		fragment.vertTexCoord_zy = geometry[n].vertTexCoord_zy;
		fragment.vertTexCoord_xz = geometry[n].vertTexCoord_xz;
		fragment.vertIsTextured = geometry[n].vertIsTextured;
		fragment.world_pos = geometry[n].world_pos;

		for(int j = 0;j<maxLights;j++)
		{
			fragment.shadow_coord[j] = geometry[n].shadow_coord[j];
		}

		EmitVertex();
	}

	//EndPrimitive();
}



void draw(Triangle triangle)
{
	fragment.vertNormal = triangle.normal;

	gl_Position = triangle.verts[0].position;
	fragment.vertColor = triangle.verts[0].color;
	EmitVertex();

	gl_Position = triangle.verts[1].position;
	fragment.vertColor = triangle.verts[1].color;
	EmitVertex();

	gl_Position = triangle.verts[2].position;
	fragment.vertColor = triangle.verts[2].color;
	EmitVertex();


	//EndPrimitive();
}

void draw(Triangle[3] triangles)
{
	for(int i = 0; i < 3; i++)
	{
		draw(triangles[i]);
	}
}

Triangle[3] subdivide(vec4 rectMid, Vertex vert1, Vertex vert2, Vertex vert3)
{
	Vertex mid;
	Triangle temp[3];

	mid.position = (vert1.position + vert2.position + vert3.position)/3;
	mid.color = (vert1.color + vert2.color + vert3.color)/3;


	vert1.color = vec4(0, 0, (1 + cos(time * PI/90))/4, (1 + cos(time * PI/180))/2);
	vert2.color = vec4(0.0,(1 + cos(time * PI/90))/3, (1 + cos(time * PI/30))/2, .5);
	vert3.color = vec4(0.0, 0.0, 1, .5);

	fragment.vertIsTextured = 0;

	temp[0].normal = vec3(0, 1, 0); 
	temp[0].verts[0] = vert1;
	temp[0].verts[1] = mid;
	temp[0].verts[2] = vert2;

	temp[1].normal = vec3(0, 1, 0); 
	temp[1].verts[0] = vert2;
	temp[1].verts[1] = mid;
	temp[1].verts[2] = vert3;

	temp[2].normal = vec3(0, 1, 0); 
	temp[2].verts[0] = vert1;
	temp[2].verts[1] = mid;
	temp[2].verts[2] = vert3;


	return temp;
}

Triangle[3] subdivide(vec4 rectMid, Triangle triangle)
{
	return subdivide(rectMid, triangle.verts[0], triangle.verts[1], triangle.verts[2]);
}

void water()
{
	Vertex one, two, three;


	one.position = gl_in[0].gl_Position;
	one.color = vec4(0, 0, (1 + sin(time * PI/180))/2, (1 + cos(time * PI/90))/3);

	two.position = gl_in[1].gl_Position + vec4(0,1,0,0);
	two.color = vec4(0, (1 + sin(time * PI/30))/2, 1, (1 + cos(time * PI/90))/3);

	three.position = gl_in[2].gl_Position;
	three.color = vec4(0, 0, (1 + sin(time * PI/30))/2, 1);

	vec4 rectMid = (one.position + three.position)/2;

	Triangle[3] subs = subdivide(rectMid, one, two, three);
		Triangle[3] subs1 = subdivide(rectMid, subs[0]);
		Triangle[3] subs2 = subdivide(rectMid, subs[1]);
		Triangle[3] subs3 = subdivide(rectMid, subs[2]);


	draw(subs1);
	draw(subs2);
	draw(subs3);

}

void main()
{
	if(IsWater == false)
		passThrough();
	else 
		water();
}

/*
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
		/*** PASS THROUGH **
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



}*/