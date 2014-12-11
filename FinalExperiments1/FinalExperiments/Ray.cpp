#include "Ray.h"


Ray::Ray()
{
}


Ray::~Ray()
{
}



void Ray::fromMouse(float x, float y, Camera * cam)
{
	vec4 rayClip, rayEye;
	rayClip = vec4(x, y, -1.0f, 1.0f);
	rayEye = cam->convertToEyeSpace(rayClip);
	rayEye = vec4(glm::vec2(rayEye), -1.0f, 0.0f);
	direction = vec3(cam->convertToWorldSpace(rayEye));
	direction = glm::normalize(direction);
	origin = cam->getEyePosition();
}


vec3 Ray::getIntersection()
{
	t = -origin.y / direction.y;
	return origin + t * direction;
}


bool Ray::isCollidingWithPlane(float minX, float maxX, float minZ, float maxZ)
{
	t = -origin.y / direction.y; // this seems redudent.
	vec3 intersection = origin + t * direction;
	return t != 0 && intersection.x < maxX && intersection.x > minX && intersection.z < maxZ && intersection.z > minZ;
}



