#version 330 core
#pragma optionNV unroll all

uniform mat4 VPMatrix;		// view projection matrix
uniform mat4 VMatrix;

layout (triangles) in;
layout (triangle_strip, max_vertices = 36) out;

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



in vec4 vertPosition[];
in vec4 vertColor[];
in vec3 vertNormal[];
in vec2 vertTexCoord[];
in vec2 vertTexCoord_xy[];
in vec2 vertTexCoord_zy[];
in vec2 vertTexCoord_xz[];
in vec4 vertWorld_pos[];
in mat4 vertModelMatrix[];
flat in int vertIsTextured[];



out vec4 geomPosition;
out vec4 geomColor;
out vec3 geomNormal;
out vec2 geomTexCoord;
out vec2 geomTexCoord_xy;
out vec2 geomTexCoord_zy;
out vec2 geomTexCoord_xz;
out vec4 geomWorld_pos;
flat out int geomIsTextured;


const float PI = 3.14159265358979323846264;

void passThrough()
{
	for(int n = 0; n < gl_in.length(); n++)
	{
		gl_Position = gl_in[n].gl_Position;
		geomPosition = vertPosition[n];
		geomColor = vertColor[n];
		geomColor.a = 0.5;
		geomNormal = vertNormal[n];
		geomTexCoord = vertTexCoord[n];
		geomTexCoord_xy = vertTexCoord_xy[n];
		geomTexCoord_zy = vertTexCoord_zy[n];
		geomTexCoord_xz = vertTexCoord_xz[n];
		geomIsTextured = vertIsTextured[n];
		geomWorld_pos = vertWorld_pos[n];

		EmitVertex();
	}

	EndPrimitive();
}



void draw(Triangle triangle)
{
	geomNormal = triangle.normal;

	gl_Position = triangle.verts[0].position;
	geomColor = triangle.verts[0].color;
	EmitVertex();

	gl_Position = triangle.verts[1].position;
	geomColor = triangle.verts[1].color;
	EmitVertex();

	gl_Position = triangle.verts[2].position;
	geomColor = triangle.verts[2].color;
	EmitVertex();


	EndPrimitive();
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
	vert2.color = vec4(0.0,(1 + cos(time * PI/90))/3, (1.5 + cos(time * PI/30))/2, 1);
	vert3.color = vec4(0.0, 0.0, 1, 1);

	geomIsTextured = 0;

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

	two.position = gl_in[1].gl_Position;
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



