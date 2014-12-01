/*
	Light.h
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

#include "Shader.h"
#include "ShadowMap.h"

#include "LoadShaders.h"
#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;

class Light
{

public:
	Light();
	~Light();

	void setIsEnabled(bool);
	void setIsLocal(bool);
	void setIsSpot(bool);
	void setAmbient(vec3);
	void setColor(vec3);
	void setPosition(vec3);
	void setHalfVector(vec3);
	void setConeDirection(vec3);
	void setSpotCosCutoff(float);
	void setSpotExponent(float);
	void setConstantAttenuation(float);
	void setLinearAttenuation(float);
	void setQuadraticAttenuation(float);
	void setShadowMap(ShadowMap);
	void setIsShadowMapEnabled(bool);

	bool getIsEnabled();
	bool getIsLocal();
	bool getIsSpot();
	vec3 getAmbient();
	vec3 getColor();
	vec3 getPosition();
	vec3 getHalfVector();
	vec3 getConeDirection();
	float getSpotCosCutoff();
	float getSpotExponent();
	float getConstantAttenuation();
	float getLinearAttenuation();
	float getQuadraticAttenuation();
	ShadowMap getShadowMap();
	bool getIsShadowMapEnabled();

	void setUniforms(Shader, int);
	void toggle();

	void mapShadows(Shader);
	void endMapShadows();


private:

	bool isEnabled;
	bool isLocal;
	bool isSpot;
	vec3 ambient;
	vec3 color;
	vec3 position;
	vec3 halfVector;
	vec3 coneDirection;
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	ShadowMap shadowMap;
	bool isShadowMapEnabled;
};

