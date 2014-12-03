#include "Terrain.h"


Terrain::Terrain()
{
	
}

Terrain::~Terrain()
{
}

void Terrain::draw(Shader in_shader)
{
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);

	for (int i = 0; i < TERR_WIDTH - 1; i++)
	{
		for (int j = 0; j < TERR_HEIGHT - 1; j++)
		{
			// Solid:
			//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 6 * ((i*(TERR_WIDTH - 1)) + j)));

			// Mesh:
			glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint) * 6 * ((i*(TERR_WIDTH - 1)) + j)));
		}
	}
	
}

void Terrain::init()
{
	for (int i = 0; i < TERR_WIDTH; i++)
	{
		for (int j = 0; j < TERR_HEIGHT; j++)
		{
			positions[i][j] = 0.0;
		}
	}

	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);
	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(positions), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < TERR_WIDTH; i++)
	{
		for (int j = 0; j < TERR_HEIGHT; j++)
		{
			GLfloat temp[4] = { -(.5 * TERR_WIDTH) + i, positions[i][j], (.5 * TERR_HEIGHT) - j, 1.0 };
			glBufferSubData(GL_ARRAY_BUFFER, ((i * TERR_WIDTH) + j) * 4 * sizeof(GLfloat), 4 * sizeof(GLfloat), temp);
		}
	}

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint) * (TERR_WIDTH - 1) * (TERR_HEIGHT - 1), NULL, GL_DYNAMIC_DRAW);

	int processed = 0;
	for (int i = 0; i < TERR_WIDTH - 1; i++)
	{
		for (int j = 0; j < TERR_HEIGHT - 1; j++)
		{
			/*
			GLuint temp[4] = { 0, (TERR_WIDTH - 1) * (TERR_HEIGHT - 1) - (TERR_HEIGHT - 1), (TERR_WIDTH - 1) * (TERR_HEIGHT - 1), TERR_WIDTH - 1,
			};*/

			GLuint temp[6] = {
				(i * TERR_WIDTH) + j,
				(i * TERR_WIDTH) + j + 1,
				((i + 1) * TERR_WIDTH) + j + 1,
				(i * TERR_WIDTH) + j,
				((i + 1) * TERR_WIDTH) + j + 1,
				((i + 1) * TERR_WIDTH) + j
			};

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,processed * sizeof(temp),sizeof(temp),temp);

			processed++;
		}
	}
}