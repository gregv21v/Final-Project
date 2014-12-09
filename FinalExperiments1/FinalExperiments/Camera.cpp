#include "Camera.h"

Camera::Camera()
{	
}

Camera::~Camera()
{
}

void Camera::camOut()
{
	float temp_x = eyePosition.x + eye_move * cos(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z - eye_move * sin(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, -eye_move));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camIn()
{
	float temp_x = eyePosition.x - eye_move * cos(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z + eye_move * sin(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, eye_move));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camUp()
{
	mat4 translate = glm::translate(glm::mat4(), vec3(0, -eye_move, 0));
	view = translate * view;
	eyePosition.y += eye_move;
}

void Camera::camDown()
{
	mat4 translate = glm::translate(glm::mat4(), vec3(0, eye_move, 0));
	view = translate * view;
	eyePosition.y -= eye_move;
}

void Camera::camLeft()
{
	float temp_x = eyePosition.x - eye_move * sin(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z - eye_move * cos(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(eye_move, 0, 0));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camRight()
{
	float temp_x = eyePosition.x + eye_move * sin(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z + eye_move * cos(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(-eye_move, 0, 0));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::zoomIn()
{
	float temp_x = eye_move * sin(totalRotation.z * PI / 180.0) * cos(totalRotation.y * PI / 180.0);
	float temp_z = eye_move * sin(totalRotation.z * PI / 180.0) * sin(totalRotation.y * PI / 180.0);
	float temp_y = eye_move * cos(totalRotation.z * PI / 180.0);

	mat4 translate = glm::translate(glm::mat4(), vec3(0, -temp_y, temp_z));
	view = translate * view;
	eyePosition = vec3(eyePosition.x - temp_x, eyePosition.y + temp_y, eyePosition.z - temp_z);
}

void Camera::zoomOut()
{
	float temp_x = eye_move * sin(totalRotation.z * PI / 180.0) * cos(totalRotation.y * PI / 180.0);
	float temp_z = eye_move * sin(totalRotation.z * PI / 180.0) * sin(totalRotation.y * PI / 180.0);
	float temp_y = eye_move * cos(totalRotation.z * PI / 180.0);

	mat4 translate = glm::translate(glm::mat4(), vec3(0,temp_y,-temp_z));
	view = translate * view;
	eyePosition = vec3(eyePosition.x + temp_x, eyePosition.y - temp_y, eyePosition.z + temp_z);
}


// rotate the view
void Camera::camLookLeft()
{
	mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3((float)0, (float)1, (float)0));
	view = rotate * view;
	totalRotation.y += -look_move;
	updateEyeDirection(glm::inverse(rotate));
}
void Camera::camLookRight()
{
	mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3((float)0, (float)1, (float)0));
	view = rotate * view;
	totalRotation.y += look_move;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::camLookUp()
{
	mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3((float)1, (float)0, (float)0));
	rotateMatrix = rotate * rotateMatrix;
	totalRotation.z += -look_move;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::camLookDown()
{
	mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3((float)1, (float)0, (float)0));
	rotateMatrix = rotate * rotateMatrix;
	totalRotation.z += look_move;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::setEyeMove(float move)
{
	eye_move = move;
}

void Camera::setLookAtMove(float move)
{
	look_move = move;
}

float Camera::getEyeX()
{
	return eyePosition.x;
}

float Camera::getEyeY()
{
	return eyePosition.y;
}

float Camera::getEyeZ()
{
	return eyePosition.z;
}

void Camera::init(float x, float y, float z)
{
	eye_move = EYE_MOVE_DEFAULT;
	look_move = LOOK_MOVE_DEFAULT;

	totalRotation = vec3(0.0,90.0,90.0);
	eyePosition = vec3(x, y, z);
	eyeDirection = vec3(0, 0, -1);

	mat4 translate = glm::translate(glm::mat4(), -eyePosition);
	rotateMatrix = glm::mat4();
	view = translate;

	frustum = glm::frustum((float)-1.0, (float)1.0, (float)-1.0, (float)1.0, (float)0.3, (float)10000);
	ortho = glm::ortho((float)-1.0, (float)1.0, (float)-1.0, (float)1.0, (float)0.3, (float)10000);

	is_ortho = false;
}

void Camera::updateEyeDirection(mat4 inMat)
{
	vec4 temp = vec4(eyeDirection, 1.0);
	temp = inMat * temp;
	eyeDirection = vec3(temp);
}

vec3 Camera::getEyeDirection()
{
	return eyeDirection;
}

vec3 Camera::getEyePosition()
{
	return eyePosition;
}

vec3 Camera::getTotalRotation()
{
	return totalRotation;
}

// Sets up uniform for the View Projection Matrix
void Camera::setUniforms(Shader shader)
{
	if (is_ortho)
	{
		glUniformMatrix4fv(shader.getUniformLocation("VPMatrix"), 1, GL_FALSE, glm::value_ptr(ortho * rotateMatrix * view));
	}
	else
	{
		glUniformMatrix4fv(shader.getUniformLocation("VPMatrix"), 1, GL_FALSE, glm::value_ptr(frustum * rotateMatrix * view));
	}
	
	glUniformMatrix4fv(shader.getUniformLocation("VMatrix"), 1, GL_FALSE, glm::value_ptr(rotateMatrix * view));
}

void Camera::setFrustumBack(float back)
{
	frustum = glm::frustum((float)-0.2, (float)0.2, (float)-0.2, (float)0.2, (float)0.3, (float)back);
}

void Camera::updateTarget()
{
	vec4 tempVec = vec4(look_x, look_y, look_z, 1.0);

	look_x = tempVec.x;
	look_y = tempVec.y;
	look_z = tempVec.z;

	cout << "Target: ( " << look_x << " , " << look_y << " , " << look_z << " )\n";
}

float Camera::getLookX()
{
	return look_x;
}

float Camera::getLookY()
{
	return look_y;
}

float Camera::getLookZ()
{
	return look_z;
}

mat4 Camera::getView()
{
	return view;
}

void Camera::setView(mat4 inView)
{
	view = inView;
}

void Camera::setFrustum(float in_left, float in_right, float in_bottom, float in_top, float in_near, float in_far)
{
	frustum = glm::frustum(in_left, in_right, in_bottom, in_top, in_near, in_far);
}

void Camera::setIsOrtho(bool in_ortho)
{
	is_ortho = in_ortho;
}

// from normalized device coordinates to eye space
vec4 Camera::convertToEyeSpace(vec4 point)
{
	if (is_ortho)
		return glm::inverse(ortho * rotateMatrix) * point;
	else
		return glm::inverse(frustum * rotateMatrix) * point;
}

// from eye to world space
vec4 Camera::convertToWorldSpace(vec4 point)
{
	return glm::inverse(view) * point;
}

vec4 Camera::unproject(vec4 point)
{
	if (is_ortho)
		return glm::inverse(ortho * rotateMatrix * view) * point;
	else
		return glm::inverse(frustum * rotateMatrix * view) * point;
		
}
