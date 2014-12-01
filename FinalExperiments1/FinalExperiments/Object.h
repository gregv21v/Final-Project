/*
	Object.h
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;

using namespace std;

class Object
{

public:
	Object();
	~Object();

	virtual void scale(float) = 0;
	virtual void translate(float, float, float) = 0;
	virtual void rotate(float, vec3) = 0;
	virtual void updateCenter() = 0;

protected:

	vec4 center;
};

