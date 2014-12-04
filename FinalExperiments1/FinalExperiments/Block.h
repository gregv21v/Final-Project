/*
	Gregory Venezia did all of this.
*/
#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#include "Model.h"



class Block : public Model
{


public:
	Block();
	void render(Shader shader);


private:
	// Material Properties
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _specularPower;





};

