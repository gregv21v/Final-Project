#include "Shader.h"


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::init(std::string vertPath, std::string fragPath)
{
	// load shader info
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertPath.c_str() },
		{ GL_FRAGMENT_SHADER, fragPath.c_str() },
		{ GL_NONE, NULL },
	};

	// set shader program
	_id = LoadShaders(shaders);
	//cout << _id << endl;
	glUseProgram(_id);
}

void Shader::use()
{
	glUseProgram(_id);
	//glLinkProgram(_id);
}

void Shader::unuse()
{
	glUseProgram(0);
}

GLint Shader::getUniformLocation(std::string name)
{
	return glGetUniformLocation(_id, name.c_str());
}
