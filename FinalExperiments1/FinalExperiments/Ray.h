#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Camera.h"


class Ray
{
public:
	Ray();
	~Ray();

	void fromMouse(float x, float y, Camera * cam);
	vec3 getIntersection();
	bool isCollidingWithPlane(float minX, float maxX, float minZ, float maxZ);

	//void at(float t);

private:
	vec3 origin;
	vec3 direction;

	float t;
};

