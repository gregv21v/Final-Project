#include "Light.h"


Light::Light()
{
	isEnabled = false;
	isLocal = false;
	isSpot = false;
	ambient = vec3(0, 0, 0);
	color = vec3(0, 0, 0);
	position = vec3(0, 0, 0);
	halfVector = vec3(0, 0, 0);
	coneDirection = vec3(0, 0, 0);
	spotCosCutoff = 0.0;
	spotExponent = 1;
	constantAttenuation = 0.25;
	linearAttenuation = 0.25;
	quadraticAttenuation = 0.1;
	isShadowMapEnabled = false;

	shadowMap.init();
}


Light::~Light()
{
}

//-----------------------------------------------
// Setters
//-----------------------------------------------
void Light::setIsEnabled(bool enabled)
{
	isEnabled = enabled;
}
void Light::setIsLocal(bool local)
{
	isLocal = local;
}
void Light::setIsSpot(bool spot)
{
	isSpot = spot;
}
void Light::setAmbient(vec3 inAmbient)
{
	ambient = inAmbient;
}
void Light::setColor(vec3 inColor)
{
	color = inColor;
}
void Light::setPosition(vec3 inPosition)
{
	position = inPosition;
}
void Light::setHalfVector(vec3 inHalfVector)
{
	halfVector = inHalfVector;
}
void Light::setConeDirection(vec3 inConeDirection)
{
	coneDirection = inConeDirection;
}
void Light::setSpotCosCutoff(float inSpotCosCutoff)
{
	spotCosCutoff = inSpotCosCutoff;
}
void Light::setSpotExponent(float inSpotExponent)
{
	spotExponent = inSpotExponent;
}
void Light::setConstantAttenuation(float inConstantAttenuation)
{
	constantAttenuation = inConstantAttenuation;
}
void Light::setLinearAttenuation(float inLinearAttenuation)
{
	linearAttenuation = inLinearAttenuation;
}
void Light::setQuadraticAttenuation(float inQuadraticAttenuation)
{
	quadraticAttenuation = inQuadraticAttenuation;
}
void Light::setIsShadowMapEnabled(bool enabled)
{
	isShadowMapEnabled = enabled;
}
//-----------------------------------------------

//-----------------------------------------------
// Getters
//-----------------------------------------------
bool Light::getIsEnabled()
{
	return isEnabled;
}
bool Light::getIsLocal()
{
	return isLocal;
}
bool Light::getIsSpot()
{
	return isSpot;
}
vec3 Light::getAmbient()
{
	return ambient;
}
vec3 Light::getColor()
{
	return color;
}
vec3 Light::getPosition()
{
	return position;
}
vec3 Light::getHalfVector()
{
	return halfVector;
}
vec3 Light::getConeDirection()
{
	return coneDirection;
}
float Light::getSpotCosCutoff()
{
	return spotCosCutoff;
}
float Light::getSpotExponent()
{
	return spotExponent;
}
float Light::getConstantAttenuation()
{
	return constantAttenuation;
}
float Light::getLinearAttenuation()
{
	return linearAttenuation;
}
float Light::getQuadraticAttenuation()
{
	return quadraticAttenuation;
}
bool Light::getIsShadowMapEnabled()
{
	return isShadowMapEnabled;
}
//-----------------------------------------------

void Light::setUniforms(Shader shader, int lightNumber)
{
	string numString = std::to_string(lightNumber);

	glUniform1i(shader.getUniformLocation("Lights[" + numString + "].isEnabled"), isEnabled);
	glUniform1i(shader.getUniformLocation("Lights[" + numString + "].isLocal"), isLocal);
	glUniform1i(shader.getUniformLocation("Lights[" + numString + "].isSpot"), isSpot);
	glUniform3f(shader.getUniformLocation("Lights[" + numString + "].ambient"), ambient.r, ambient.g, ambient.b);
	glUniform3f(shader.getUniformLocation("Lights[" + numString + "].color"), color.r, color.g, color.b);
	glUniform3f(shader.getUniformLocation("Lights[" + numString + "].position"), position.x, position.y, position.z);
	glUniform3f(shader.getUniformLocation("Lights[" + numString + "].halfVector"), halfVector.x, halfVector.y, halfVector.z);
	glUniform3f(shader.getUniformLocation("Lights[" + numString + "].coneDirection"), coneDirection.x, coneDirection.y, coneDirection.z);
	glUniform1f(shader.getUniformLocation("Lights[" + numString + "].spotCosCutoff"), spotCosCutoff);
	glUniform1f(shader.getUniformLocation("Lights[" + numString + "].spotExponent"), spotExponent);
	glUniform1f(shader.getUniformLocation("Lights[" + numString + "].constantAttenuation"), constantAttenuation);
	glUniform1f(shader.getUniformLocation("Lights[" + numString + "].linearAttenuation"), linearAttenuation);
	glUniform1f(shader.getUniformLocation("Lights[" + numString + "].quadraticAttenuation"), quadraticAttenuation);
	glUniform1i(shader.getUniformLocation("Lights[" + numString + "].isShadowMapEnabled"), isShadowMapEnabled);
	
	shadowMap.use(shader, lightNumber);

}

void Light::toggle()
{
	isEnabled = !isEnabled;
}

void Light::mapShadows(Shader shader)
{
	shadowMap.updateView(position);
	shadowMap.render(shader);
	shadowMap.draw();
}

void Light::endMapShadows()
{
	shadowMap.endDraw();
}