#include "Terrain.h"

//--------------------------------------------------------
// Constructor / Destructor
//--------------------------------------------------------
Terrain::Terrain()
{
	
}
Terrain::~Terrain()
{
}
//--------------------------------------------------------

//--------------------------------------------------------
// Draw Function
//--------------------------------------------------------
void Terrain::draw(Shader in_shader)
{
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribI1i(vIsTransformed, isTransformed);
	glVertexAttribI1i(vIsTextured,isTextured);
	glVertexAttrib4fv(vColor,&color.red);

	for (int i = 0; i < TERR_WIDTH - 1; i++)
	{
		for (int j = 0; j < TERR_HEIGHT - 1; j++)
		{
			// Solid:
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 6 * ((i*(TERR_WIDTH - 1)) + j)));
			//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 12 * ((i*(TERR_WIDTH - 1)) + j)));

			// Mesh:
			//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 6 * ((i*(TERR_WIDTH - 1)) + j)));
			//glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 12 * ((i*(TERR_WIDTH - 1)) + j)));
		}
	}

	//cout << "draw" << endl;
}
//--------------------------------------------------------

//--------------------------------------------------------
// Init Function
//--------------------------------------------------------
void Terrain::init()
{
	for (int i = 0; i < TERR_WIDTH; i++)
	{
		for (int j = 0; j < TERR_HEIGHT; j++)
		{
			positions[i][j] = 0.0;
			normals[i][j] = vec3(0, 1, 0);

			/*
			switch(i)
			{
			case 0:
				switch (j)
				{
				case 0:
					normals[i][j] = glm::normalize(vec3(-1, 3, -1));
					break;
				case TERR_HEIGHT:
					normals[i][j] = glm::normalize(vec3(-1, 3, 1));
					break;
				default:
					normals[i][j] = glm::normalize(vec3(-1, 2, 0));
					break;
				}
				break;
			case TERR_WIDTH:
				switch (j)
				{
				case 0:
					normals[i][j] = glm::normalize(vec3(1, 3, -1));
					break;
				case TERR_HEIGHT:
					normals[i][j] = glm::normalize(vec3(1, 3, 1));
					break;
				default:
					normals[i][j] = glm::normalize(vec3(1, 2, 0));
					break;
				}
				break;
			default:
				normals[i][j] = vec3(0, 1, 0);
				break;
			}
			*/
		}
	}

	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);

	// POSITION BUFFER
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(positions), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < TERR_WIDTH; i++)
	{
		for (int j = 0; j < TERR_HEIGHT; j++)
		{
			GLfloat temp[4] = { i, positions[i][j], j, 1.0 };
			glBufferSubData(GL_ARRAY_BUFFER, ((i * TERR_WIDTH) + j) * 4 * sizeof(GLfloat), 4 * sizeof(GLfloat), temp);
		}
	}

	// NORMAL BUFFER
	glEnableVertexAttribArray(vNormal);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORM_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// INDEX BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint) * (TERR_WIDTH - 1) * (TERR_HEIGHT - 1), NULL, GL_DYNAMIC_DRAW);

	int processed = 0;
	for (int i = 0; i < TERR_WIDTH - 1; i++)
	{
		for (int j = 0; j < TERR_HEIGHT - 1; j++)
		{
			
			GLuint temp[6] = {
				(i * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1
			};
			
				/*
			GLuint temp[12] = {
				(i * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1,

				((i + 1) * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j + 1,
				((i + 1) * TERR_WIDTH) + j,
				(i * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j
			};*/

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,processed * sizeof(temp),sizeof(temp),temp);

			processed++;
		}
	}

	size = 0;
	roundness = 0;

	// set base color
	color.red = 1;
	color.green = 1;
	color.blue = 1;
	color.alpha = 1;

	isTextured = false;
	isTransformed = false;
	tile_factor = 1;
}
//--------------------------------------------------------

//--------------------------------------------------------
// Size Functions
//--------------------------------------------------------
void Terrain::setSize(int in_size)
{
	size = in_size;
}
bool Terrain::increaseSize()
{
	if (size < MAX_SIZE)
	{
		size++;
		return true;
	}
	return false;
}
bool Terrain::decreaseSize()
{
	if (size > 0)
	{
		size--;
		return true;
	}
	return false;
}
//--------------------------------------------------------

//--------------------------------------------------------
// Roundness Functions
//--------------------------------------------------------
void Terrain::setRoundness(int in_roundness)
{
	roundness = in_roundness;
}
bool Terrain::increaseRoundness()
{
	if (size < MAX_ROUND)
	{
		roundness++;
		return true;
	}
	return false;
}
bool Terrain::decreaseRoundness()
{
	if (roundness > 0)
	{
		roundness--;
		return true;
	}
	return false;
}
//--------------------------------------------------------

//--------------------------------------------------------
// Terrain Manipulation
//--------------------------------------------------------
void Terrain::mound(int x, int z, float y)
{
	for (int i = 0; i <= 2 * size; i++)
	{
		for (int j = 0; j <= 2 * size; j++)
		{
			int temp_x = x - size + i;
			int temp_z = z - size + j;

			if (temp_x > 1 && temp_x < TERR_WIDTH - 2 &&
				temp_z > 1 && temp_z < TERR_HEIGHT - 2)
			{
				// y minus ( ( ( distance from center ) / (level / 2 ) ) times y )
				float temp_pos =
					y - pow((sqrt(pow(temp_x - x, 2) + pow(temp_z - z, 2)) / size), roundness) * y;

				if (y > 0)
				{
					if (temp_pos > 0)
					{
						positions[temp_x][temp_z] += temp_pos;
					}
				}
				else
				{
					if (y < 0)
					{
						if (temp_pos < 0)
						{
							positions[temp_x][temp_z] += temp_pos;
						}
					}
				}

				updatePosBuffer(temp_x, temp_z);
			}
		}
	}
	updateNormals(x,z);
}
void Terrain::mountain(int x, int z, float y)
{
	// for each vertex in a grid around the center vertex
	for (int i = 0; i <= 2 * size; i++)
	{
		for (int j = 0; j <= 2 * size; j++)
		{
			// coordinates of the vertex we are looking at
			int temp_x = x - size + i;
			int temp_z = z - size + j;

			// if the coordinate is within the terrain
			if (temp_x >= 0 && temp_x < TERR_WIDTH &&
				temp_z >= 0 && temp_z < TERR_HEIGHT)
			{
				float temp_pos =
					y - pow((sqrt(pow(temp_x - x, 2) + pow(temp_z - z, 2)) / size), 1) * ((rand() % 4) * y /2);


				if (y > 0)
				{
					if (temp_pos > 0)
					{
						positions[temp_x][temp_z] += temp_pos;
					}
				}
				else
				{
					if (y < 0)
					{
						if (temp_pos < 0)
						{
							positions[temp_x][temp_z] += temp_pos;
						}
					}
				}

				updatePosBuffer(temp_x, temp_z);
			}
		}
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
// Update Data
//--------------------------------------------------------
void Terrain::updatePosBuffer(int x, int z)
{
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);

	glBufferSubData(GL_ARRAY_BUFFER,
		((x * TERR_HEIGHT) + (z)) * 4 * sizeof(GLfloat) + sizeof(GLfloat),
		sizeof(GLfloat),
		&positions[x][z]);
}
void Terrain::updateNormals(int x, int z)
{
	//cout << "Calculating Normals...\n";
	for (int i = 0; i <= 2 * size; i++)
	{
		for (int j = 0; j <= 2 * size; j++)
		{
			int temp_x = x - size + i;
			int temp_z = z - size + j;

			if (temp_x >= 0 && temp_x < TERR_WIDTH &&
				temp_z >= 0 && temp_z < TERR_HEIGHT)
			{
				vec3 new_normal = vec3(0, 0, 0);
				int num_normals = 0;

				if (temp_x - 1 >= 0 && temp_z - 1 >= 0)
				{
					new_normal += (glm::cross(
						vec3(temp_x - 1, positions[temp_x - 1][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x - 1, positions[temp_x - 1][temp_z], temp_z) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;

					new_normal += (glm::cross(
						vec3(temp_x, positions[temp_x][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x - 1, positions[temp_x - 1][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;
				}
				if (temp_x + 1 < TERR_WIDTH && temp_z - 1 >= 0)
				{
					new_normal += (glm::cross(
						vec3(temp_x + 1, positions[temp_x + 1][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x, positions[temp_x][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;

					new_normal += (glm::cross(
						vec3(temp_x + 1, positions[temp_x + 1][temp_z], temp_z) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x + 1, positions[temp_x + 1][temp_z - 1], temp_z - 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;
				}
				if (temp_x + 1 < TERR_WIDTH && temp_z + 1< TERR_HEIGHT)
				{
					new_normal += (glm::cross(
						vec3(temp_x + 1, positions[temp_x + 1][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x + 1, positions[temp_x + 1][temp_z], temp_z) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;

					new_normal += (glm::cross(
						vec3(temp_x, positions[temp_x][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x + 1, positions[temp_x + 1][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;
				}
				if (temp_x - 1 >= 0 && temp_z + 1 < TERR_HEIGHT)
				{
					new_normal += (glm::cross(
						vec3(temp_x - 1, positions[temp_x - 1][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x, positions[temp_x][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;

					new_normal += (glm::cross(
						vec3(temp_x - 1, positions[temp_x - 1][temp_z], temp_z) - vec3(temp_x, positions[temp_x][temp_z], temp_z),
						vec3(temp_x - 1, positions[temp_x - 1][temp_z + 1], temp_z + 1) - vec3(temp_x, positions[temp_x][temp_z], temp_z)));
					num_normals++;
				}
				//cout << num_normals << endl;
				//cout << "( " << new_normal.x << " , " << new_normal.y << " , " << new_normal.z << " )" << endl;
				normals[temp_x][temp_z] = glm::normalize(new_normal);
				//cout << "( " << normals[temp_x][temp_z].x << " , " << normals[temp_x][temp_z].y << " , " << normals[temp_x][temp_z].z << " )" << endl; 
				updateNormBuffer(temp_x, temp_z);
			}
		}
	}
}
void Terrain::updateNormBuffer(int x, int z)
{
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORM_BUFFER]);

	glBufferSubData(GL_ARRAY_BUFFER,
		((x * TERR_HEIGHT) + (z)) * sizeof(vec3),
		sizeof(vec3),
		&normals[x][z]);
}
//--------------------------------------------------------

//--------------------------------------------------------
// State Functions
//--------------------------------------------------------
void Terrain::setIsTextured(bool in_textured)
{
	isTextured = in_textured;
}
void Terrain::setIsTransformed(bool in_transformed)
{
	isTransformed = in_transformed;
}
void Terrain::setTileFactor(int in_tileFactor)
{
	tile_factor = in_tileFactor;
}
bool Terrain::getIsTextured()
{
	return isTextured;
}
bool Terrain::getIsTransformed()
{
	return isTransformed;
}
int Terrain::getWidth()
{
	return TERR_WIDTH;
}
int Terrain::getHeight()
{
	return TERR_HEIGHT;
}
//--------------------------------------------------------

void Terrain::setupUniforms(Shader in_shader)
{
	glUniform1i(in_shader.getUniformLocation("terrainProperties.width"), TERR_WIDTH);
	glUniform1i(in_shader.getUniformLocation("terrainProperties.height"), TERR_HEIGHT);	
	glUniform1i(in_shader.getUniformLocation("terrainProperties.tileFactor"), tile_factor);
}