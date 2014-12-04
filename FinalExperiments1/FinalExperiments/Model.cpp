#include "Model.h"


Model::Model()
{
	
}

Model::~Model()
{
}

// Draw
void Model::draw(Shader shader)
{
	glBindVertexArray(VAOs[0]);

	activateTextures(shader);

	glEnableVertexAttribArray(vPosition);

	if (texels.size() > 0)
	{
		glEnableVertexAttribArray(vTexel);
	}
	if (normals.size() > 0)
	{
		glEnableVertexAttribArray(vNormal);
	}
	if (textureIDs.size() > 0)
	{
		glEnableVertexAttribArray(vTexture);
	}

	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttribI1i(vIsTransformed, isTransformed);
	glVertexAttrib4fv(vModelMatrix0, &transform[0][0]);
	glVertexAttrib4fv(vModelMatrix1, &transform[1][0]);
	glVertexAttrib4fv(vModelMatrix2, &transform[2][0]);
	glVertexAttrib4fv(vModelMatrix3, &transform[3][0]);
	glVertexAttrib4fv(vColor, &color._red);
	glVertexAttrib4fv(vNormalMatrix0, &nTransform[0][0]);
	glVertexAttrib4fv(vNormalMatrix1, &nTransform[1][0]);
	glVertexAttrib4fv(vNormalMatrix2, &nTransform[2][0]);


	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	deactivateTextures();
}


// Transformation Stuff
void Model::scale(float scaleFactor)
{
	// Translate to center
	mat4 translate1 = glm::translate(mat4(),vec3(0 - center.x, 0 - center.y, 0 - center.z));
	mat4 scale = glm::scale(mat4(), vec3(scaleFactor, scaleFactor, scaleFactor));
	mat4 translate2 = glm::translate(mat4(), vec3(center.x,center.y,center.z));

	transform = (translate2 * scale * translate1) * transform;
	updateCenter();
	//updateNormalMat();
}

void Model::translate(float x, float y, float z)
{
	mat4 translate = glm::translate(mat4(),vec3(x, y, z));
	transform = translate * transform;

	updateCenter();
	//updateNormalMat();
}

void Model::rotate(float angle, vec3 inAxis)
{
	// Translate to center
	mat4 translate1 = glm::translate(mat4(), vec3(0 - center.x, 0 - center.y, 0 - center.z));
	mat4 rotate = glm::rotate(mat4(), angle, inAxis);
	mat4 translate2 = glm::translate(mat4(), vec3(center.x, center.y, center.z));

	transform = (translate2 * rotate * translate1) * transform;
	updateCenter();
	//updateNormalMat();
}

void Model::updateCenter()
{
	center = transform[3];
}

// Loading Stuff
void Model::init(string filename)
{
	cout << "Loading Data...";
	loadObject(filename);
	cout << "Done.\n";

	cout << "Normalizing Normals...";
	//normalizeNormals();
	cout << "Done.\n";

	init();

	transform = mat4();
	//updateNormalMat();
	center = vec4(0.0, 0.0, 0.0, 1.0);

	isTransformed = 1;
	calculateDimentions();

	color = { 1, 1, 1, 1 };
}

void Model::init()
{
	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);

	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec4), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	if (texels.size() > 0)
	{
		glEnableVertexAttribArray(vTexel);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXEL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, texels.size() * sizeof(vec2), &texels[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vTexel, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (normals.size() > 0)
	{
		glEnableVertexAttribArray(vNormal);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (textureIDs.size() > 0)
	{
		glEnableVertexAttribArray(vTexture);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, textureIDs.size() * sizeof(int), &textureIDs[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vTexture, 1, GL_INT, GL_FALSE, 0, 0);
	}
}

// Gets vertex, texel, normal data
void Model::loadObject(string filename)
{
	name = filename;

	vector<vec4> in_vertices;
	vector<vec3> in_normals;
	vector<vec2> in_texels;
	vector<string> in_materials;

	bool isTexel = false;
	bool isNormal = false;
	int curTextID = -1;

	string matFileName;

	ifstream in(filename, ios::in);

	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
	}

	string line;

	while (getline(in, line))
	{
		//-----------------------------------
		// Get Material File Name
		//-----------------------------------
		if (line.substr(0, 7) == "mtllib ")
		{
			istringstream s(line.substr(7));
			s >> matFileName;
		}
		//-----------------------------------
		else
		{
			//-----------------------------------
			// Read Vertex Positions
			//-----------------------------------
			if (line.substr(0, 2) == "v ")
			{
				istringstream s(line.substr(2));
				vec4 v;
				float x, y, z, w;
				s >> x;
				s >> y;
				s >> z;
				w = 1.0f;
				v = vec4(x, y, z, w);
				in_vertices.push_back(v);
			}
			//-----------------------------------
			else
			{
				//-----------------------------------
				// Read Texels
				//-----------------------------------
				if (line.substr(0, 3) == "vt ")
				{
					isTexel = true;
					istringstream s(line.substr(3));
					vec2 v;
					float x, y;
					s >> x;
					s >> y;
					y = 1 - y;
					v = vec2(x, y);
					in_texels.push_back(v);
					//-----------------------------------
				}
				else
				{
					//-----------------------------------
					// Read Normals
					//-----------------------------------
					if (line.substr(0, 3) == "vn ")
					{
						isNormal = true;
						istringstream s(line.substr(3));
						vec3 v;
						float x, y, z;
						s >> x;
						s >> y;
						s >> z;
						v = vec3(x, y, z);
						in_normals.push_back(v);
						//-----------------------------------
					}
					else
					{
						if (line.substr(0, 7) == "usemtl ")
						{
							istringstream s(line.substr(7));
							string temp;
							s >> temp;

							if (temp.find("NONE") == -1)
							{
								curTextID = 1;
							}
							else
							{
								curTextID = 0;
							}
						}
						else
						{
							//-----------------------------------
							// Read Face Info
							//-----------------------------------
							if (line.substr(0, 2) == "f ")
							{
								istringstream s(line.substr(2));

								if (!isNormal && !isTexel)
								{
									GLushort av, bv, cv;
									s >> av;
									s >> bv;
									s >> cv;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
								}
								else if (!isTexel)
								{
									GLushort av, an, bv, bn, cv, cn;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> an;
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bn;
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cn;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									normals.push_back(in_normals[--an]);
									normals.push_back(in_normals[--bn]);
									normals.push_back(in_normals[--cn]);
								}
								else if (!isNormal)
								{
									GLushort av, at, bv, bt, cv, ct;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> at;
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bt;
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> ct;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									texels.push_back(in_texels[--at]);
									texels.push_back(in_texels[--bt]);
									texels.push_back(in_texels[--ct]);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
								}
								else if (isNormal && isTexel)
								{
									GLushort av, at, an, bv, bt, bn, cv, ct, cn;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> at;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> an;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bt;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bn;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> ct;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cn;
									while (s.peek() == '/')
									{
										s.get();
									}
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									texels.push_back(in_texels[--at]);
									texels.push_back(in_texels[--bt]);
									texels.push_back(in_texels[--ct]);
									normals.push_back(in_normals[--an]);
									normals.push_back(in_normals[--bn]);
									normals.push_back(in_normals[--cn]);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
								}

							}// end "f " if else

						}// end "usemtl " else

					}// end "vn " if else

				}// end "vt " if else

			}// end "v " if else

		}// end getting material file name

	}// end parse while loop for .obj files

	in.close();
}

void Model::normalizeNormals()
{
	vec4 tempVert;
	vector<vec3> tempNorms;
	vector<bool> checked;
	vector<bool> normalMarked;

	for (int i = 0; i < vertices.size(); i++)
	{
		checked.push_back(false);
		normalMarked.push_back(false);
	}

	// for every vertex
	for (int i = 0; i < vertices.size(); i++)
	{
		// if already checked, continue to next iteration
		if (checked.at(i) == true)
			continue;

		// set a temp vert for the current vertex we are updating
		tempVert = vertices.at(i);

		// push the corresponding vertex to a temp array
		tempNorms.push_back(normals.at(i));
		normalMarked.at(i) = true;

		// set the vertex already checked
		checked.at(i) = true;

		//cout << "Vertex " << i << endl;
		// for every vertex matching the coordinates of the selected vertex
		for (int j = i + 1; j < vertices.size(); j++)
		{
			if (vertices.at(j) == tempVert)
			{
				bool add = true;
				for (int k = 0; k < tempNorms.size(); k++)
				{
					if (normals.at(j) == tempNorms.at(k))
						add = false;
				}

				if (add == true)
					tempNorms.push_back(normals.at(j));

				normalMarked.at(j) = true;
				checked.at(j) = true;
			}
		}

		vec3 calcNormal = vec3(0,0,0);

		for (int j = 0; j < tempNorms.size(); j++)
		{
			calcNormal += tempNorms.at(j);
		}

		calcNormal /= tempNorms.size();
		//cout << "( " << calcNormal.x << " , " << calcNormal.y << " , " << calcNormal.z << " )" << endl;
		for (int j = 0; j < normals.size(); j++)
		{
			if (normalMarked.at(j) == true)
			{
				normals.at(j) = calcNormal;
				normalMarked.at(j) = false;
			}
		}

		tempNorms.clear();
	}
}

void Model::calculateDimentions()
{
	max_x = 0;
	max_y = 0;
	max_z = 0;
	min_x = INFINITY;
	min_y = INFINITY;
	min_z = INFINITY;

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		if (vertices.at(i)[0] > max_x)
			max_x = vertices.at(i)[0];
		if (vertices.at(i)[1] > max_y)
			max_y = vertices.at(i)[1];
		if (vertices.at(i)[2] > max_z)
			max_z = vertices.at(i)[2];

		if (vertices.at(i)[0] < min_x)
			min_x = vertices.at(i)[0];
		if (vertices.at(i)[1] < min_y)
			min_y = vertices.at(i)[1];
		if (vertices.at(i)[2] < min_z)
			min_z = vertices.at(i)[2];
	}

	height = max_x - min_x;
	depth = max_y - min_y;
	width = max_z - min_z;
}

float Model::getWidth()
{
	return width;
}

float Model::getHeight()
{
	return height;
}

float Model::getDepth()
{
	return depth;
}

// Texture Stuff
void Model::setTexture(Texture* inTexture)
{
	texture = inTexture;

	if (texture != nullptr)
	{
		isTextured = 1;
	}
	else
	{
		isTextured = 0;
	}
}

void Model::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("tex"));
}

void Model::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}

void Model::updateTransform(mat4 inTransform)
{
	transform = inTransform * transform;
	updateCenter();
	//updateNormalMat();
}

void Model::setColor(Color inColor)
{
	color = inColor;
}

void Model::updateNormalMat()
{
	//cout << "Update Normal\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nTransform[j][i] = transform[j][i];
			//cout << nTransform[j][i] << "\t";
		}
		//cout << endl;
	}
	//cout << endl;

	nTransform =glm::transpose(glm::inverse(nTransform));
}

float Model::getMaxY()
{
	return max_y;
}