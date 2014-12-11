#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
}


ShadowMap::~ShadowMap()
{
}

void ShadowMap::init()
{
	glGenTextures(1, &depth_texture);
	
	glBindTexture(GL_TEXTURE_2D, depth_texture);

	glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT32,
		DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE,0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set up depth comparison mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	// Set up wrapping modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

	// Create FBO to render depth into
	glGenFramebuffers(1, &_fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	// Attach the depth texture to it
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT_EXT, depth_texture, 0);
	//**
	//glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	// Disable color rendering as there are no color attachments
	glDrawBuffer(GL_NONE);
	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
	}

	frustum = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, (float)1000);

	scaleBias = glm::mat4(
		vec4(0.5, 0.0, 0.0, 0.0),
		vec4(0.0, 0.5, 0.0, 0.0),
		vec4(0.0, 0.0, 0.5, 0.0),
		vec4(0.5, 0.5, 0.5, 1.0));
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ShadowMap::render(Shader shader)
{
	shader.use();
	//cout << "VPMatrix: " << shader.getUniformLocation("VPMatrix") << endl;
	glUniformMatrix4fv(shader.getUniformLocation("VPMatrix"), 1, GL_FALSE, glm::value_ptr(frustum * view));
}

void ShadowMap::use(Shader shader,int i)
{
	shader.use();
	//cout << "ShadowMatrix: " << shader.getUniformLocation("ShadowMatrix") << endl;
	glUniformMatrix4fv(shader.getUniformLocation("shadowMatrices[" + std::to_string(i) + "].ShadowMatrix"), 1, GL_FALSE, glm::value_ptr(scaleBias * frustum * view));

	glUniform1i(shader.getUniformLocation("Lights[" + std::to_string(i) + "].depth_texture"), 5 + i);
	//cout << "Active Texture Base: " << GL_TEXTURE1 << endl;
	glActiveTexture(GL_TEXTURE5 + i);
	//cout << "Active Texture: " << GL_TEXTURE5 + i << endl;
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	//cout << "depth_texture: " << shader.getUniformLocation("depth_texture") << endl;
	
	//cout << "Shader Location: " << shader.getUniformLocation("Lights[" + to_string(i) + "].depth_texture") << endl;
	//glUniform1i(shader.getUniformLocation("depth_texture"), 1 + i);
}

void ShadowMap::updateView(vec3 lightPosition)
{
	view = glm::lookAt(lightPosition, vec3(0, 0, 0), vec3(0, 1, 0));

	//mat4 trans = glm::translate(mat4(), vec3(0, 5, 0));
	//mat4 rotate = glm::rotate(mat4(), (float)90, vec3(0, 1, 0));
	//view = rotate * trans;
}

void ShadowMap::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(4.0f, 20.0f);
}

void ShadowMap::endDraw()
{
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}