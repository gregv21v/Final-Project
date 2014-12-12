/*
	Model.h
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Object.h"

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::cout;

class Model : public Object
{

public:

	Model();
	~Model();

	void draw(Shader);
	void init(string);
	void loadObject(string);
	void normalizeNormals();

	void calculateDimentions();
	float getWidth();
	float getHeight();
	float getDepth();

	void activateTextures(Shader);
	void deactivateTextures();

	void setTexture(Texture*);
	void setColor(vec4 color);

	void scale(float);
	void translate(float, float, float);
	void rotate(float, vec3);
	void updateCenter();

	void updateTransform(mat4);
	void updateNormalMat();

	float getMaxY();

	vec4 center;
	mat4 transform;

private:

	vector<vec4> vertices;
	vector<vec2> texels;
	vector<vec3> normals;
	vector<int> textureIDs;
	Texture* texture;

	//---------------------------------------------------------
	// Declare Data (Vertex Arrays and Buffers)
	//---------------------------------------------------------
	enum { POS_BUFFER, TEXEL_BUFFER, NORMAL_BUFFER, TEXTURE_BUFFER, NUM_BUFFERS };

	GLuint VAOs[1];
	GLuint Buffers[NUM_BUFFERS];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Vertex Attributes
	//---------------------------------------------------------
	enum {
		vPosition, vTexel, vNormal, vTexture, vIsTextured, vIsTransformed,
		vModelMatrix0, vModelMatrix1, vModelMatrix2, vModelMatrix3, vColor,
		vNormalMatrix0, vNormalMatrix1, vNormalMatrix2
	};
	//---------------------------------------------------------

	int isTransformed;
	GLuint isTextured;

	vec4 color;

	float width;
	float height;
	float depth;

	glm::mat3 nTransform;

	float max_x;
	float max_y;
	float max_z;
	float min_x;
	float min_y;
	float min_z;

protected:

	string name;
};

