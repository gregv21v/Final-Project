#include "Block.h"


Block::Block()
{
	Model::Model();
	_ambient = glm::vec3(1.0, 1.0, 1.0);
	_diffuse = glm::vec3(0.0, 0.0, 0.0);
	_specular = glm::vec3(1.0, 1.0, 1.0);
	_specularPower = 2.5;
	
}



void Block::render(Shader shader)
{

	glUniform3fv(shader.getUniformLocation("material_ambient"), 1, glm::value_ptr(_ambient));
	glUniform3fv(shader.getUniformLocation("material_diffuse"), 1, glm::value_ptr(_diffuse));
	glUniform3fv(shader.getUniformLocation("material_specular"), 1, glm::value_ptr(_specular));
	glUniform1f(shader.getUniformLocation("material_specular_power"), _specularPower);


}