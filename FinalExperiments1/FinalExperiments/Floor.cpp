#include "Floor.h"


Floor::Floor()
{
}


Floor::~Floor()
{
}

void Floor::init(float terrain_width,float terrain_height)
{
	float low_x = (terrain_width / 2) - (MAX_WIDTH / 2);
	float low_z = (terrain_height / 2) - (MAX_HEIGHT / 2);
	float high_x = (terrain_width / 2) + (MAX_WIDTH / 2);
	float high_z = (terrain_height / 2) + (MAX_HEIGHT / 2);

	positions[0] = vec4(0, 0, 0, 1);
	positions[1] = vec4(0, 0, terrain_height, 1);
	positions[2] = vec4(terrain_width, 0, terrain_height, 1);
	positions[3] = vec4(terrain_width, 0, 0, 1);
	positions[4] = vec4(-DEPTH, -DEPTH, -DEPTH, 1);
	positions[5] = vec4(-DEPTH, -DEPTH, terrain_height + DEPTH, 1);
	positions[6] = vec4(terrain_width + DEPTH, -DEPTH, terrain_height + DEPTH, 1);
	positions[7] = vec4(terrain_width + DEPTH, -DEPTH, -DEPTH, 1);
	positions[8] = vec4(low_x,-DEPTH,low_z,1);
	positions[9] = vec4(low_x,-DEPTH,high_z,1);
	positions[10] = vec4(high_x,-DEPTH,high_z,1);
	positions[11] = vec4(high_x,-DEPTH,low_z,1);
	/*
	normals[0] = glm::normalize(
		glm::normalize(vec3(0, 1, -1)) + 
		glm::normalize(vec3(-1, 1, 0)) + 
		glm::normalize(vec3(0, 1, 0)));
	normals[1] = glm::normalize(
		glm::normalize(vec3(0, 1, 1)) +
		glm::normalize(vec3(-1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));
	normals[2] = glm::normalize(
		glm::normalize(vec3(0, 1, 1)) +
		glm::normalize(vec3(1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));
	normals[3] = glm::normalize(
		glm::normalize(vec3(0, 1, -1)) +
		glm::normalize(vec3(1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));

	normals[4] = glm::normalize(
		glm::normalize(vec3(0, 1, -1)) +
		glm::normalize(vec3(-1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));
	normals[5] = glm::normalize(
		glm::normalize(vec3(0, 1, 1)) +
		glm::normalize(vec3(-1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));
	normals[6] = glm::normalize(
		glm::normalize(vec3(0, 1, 1)) +
		glm::normalize(vec3(1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));
	normals[7] = glm::normalize(
		glm::normalize(vec3(0, 1, -1)) +
		glm::normalize(vec3(1, 1, 0)) +
		glm::normalize(vec3(0, 1, 0)));

	normals[8] = glm::normalize(vec3(0, 1, 0));
	normals[9] = glm::normalize(vec3(0, 1, 0));
	normals[10] = glm::normalize(vec3(0, 1, 0));
	normals[11] = glm::normalize(vec3(0, 1, 0));
	*/
	for (int i = 0; i < 12; i++)
	{
		normals[i] = glm::normalize(vec3(0, 1, 0));
	}

	/*
	for (int i = 0; i < 4; i++)
	{
		normals[i] = glm::normalize(
			glm::cross(
			(vec3(positions[(i + 1) % 4].x, positions[(i + 1) % 4].y, positions[(i + 1) % 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 3) % 4].x, positions[(i + 3) % 4].y, positions[(i + 3) % 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z))) +
			glm::cross(
			(vec3(positions[(i) % 4 + 4].x, positions[(i) % 4 + 4].y, positions[(i) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 1) % 4 + 4].x, positions[(i + 1) % 4 + 4].y, positions[(i + 1) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z))) +
			glm::cross(
			(vec3(positions[(i) % 4 + 4].x, positions[(i) % 4 + 4].y, positions[(i) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 3) % 4 + 4].x, positions[(i + 3) % 4 + 4].y, positions[(i + 3) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)))
			);

		normals[i + 4] = glm::normalize(
			glm::cross(
			(vec3(positions[(i + 1) % 4].x, positions[(i + 1) % 4].y, positions[(i + 1) % 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 3) % 4].x, positions[(i + 3) % 4].y, positions[(i + 3) % 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z))) +
			glm::cross(
			(vec3(positions[(i) % 4 + 4].x, positions[(i) % 4 + 4].y, positions[(i) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 1) % 4 + 4].x, positions[(i + 1) % 4 + 4].y, positions[(i + 1) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z))) +
			glm::cross(
			(vec3(positions[(i) % 4 + 4].x, positions[(i) % 4 + 4].y, positions[(i) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)),
			(vec3(positions[(i + 3) % 4 + 4].x, positions[(i + 3) % 4 + 4].y, positions[(i + 3) % 4 + 4].z) - vec3(positions[i].x, positions[i].y, positions[i].z)))
			);
	}
	*/

	GLuint indices[48] = {
		0, 5, 1, 0, 4, 5,
		1, 6, 2, 1, 5, 6,
		2, 7, 3, 2, 6, 7,
		3, 4, 0, 3, 7, 4,

		4, 9, 5, 4, 8, 9,
		5, 10, 6, 5, 9, 10,
		6, 11, 7, 6, 10, 11,
		7, 8, 4, 7, 11, 8,
	};

	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);

	// POSITION BUFFER
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// NORMAL BUFFER
	glEnableVertexAttribArray(vNormal);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORM_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// INDEX BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	// set base color
	color.red = 0;
	color.green = 1;
	color.blue = 0;
	color.alpha = 1;

	isTextured = false;
	isTransformed = false;
}

void Floor::draw(Shader in_shader)
{
	

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);

	activateTextures(in_shader);

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribI1i(vIsTransformed, isTransformed);
	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttrib4fv(vColor, &color.red);

	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

	deactivateTextures();
}

// Texture Stuff
void Floor::setTexture(Texture* inTexture)
{
	texture = inTexture;

	if (texture != nullptr)
	{
		isTextured = true;
	}
	else
	{
		isTextured = false;
	}
}

void Floor::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("modelTex"), 10);
}

void Floor::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}

void Floor::setupUniforms(Shader in_shader)
{
	glUniform1i(in_shader.getUniformLocation("floorProperties.width"), MAX_WIDTH);
	glUniform1i(in_shader.getUniformLocation("floorProperties.height"), MAX_HEIGHT);
}