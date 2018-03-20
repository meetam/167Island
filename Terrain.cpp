#include "Terrain.h"
#include "Window.h"
#include "stb_image.h"

Terrain::Terrain() { }

Terrain::Terrain(float size, const char* heightMapPath) {
	this->size = size;

	readHeightMap(heightMapPath);
	calculateNormals();

	// creates the buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vertex);
	glGenBuffers(1, &VBO_normal);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// binds the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// binds the normal buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// binds the vertex index buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	// unbinds the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_vertex);
	glDeleteBuffers(1, &VBO_normal);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(2, &textureId[0]);
}

void Terrain::readHeightMap(const char* heightMapPath) {
	int width, height, numColorChannels;
	unsigned char *heightMap = stbi_load(heightMapPath, &width, &height, &numColorChannels, 0);

	if (!heightMap) {
		std::cout << "Height map failed to load" << std::endl;
	}

	else {
		// std::cout << width << " " << height << " " << numColorChannels << " " << sizeof(heightMap) << std::endl;
	}

	if (heightMap) {
		this->numVertex = width;

		// calculates the vertex positions
		int i = 1;
		for (int z = -numVertex / 2; z < numVertex / 2; z++) {
			for (int x = -numVertex / 2; x < numVertex / 2; x++) {
				float vertexX = (x * 1.0 / numVertex) * size;
				float vertexY = heightMap[i];
				float vertexZ = (z * 1.0 / numVertex) * size;

				vertices.push_back((GLfloat)vertexX);
				vertices.push_back((GLfloat)vertexY);
				vertices.push_back((GLfloat)vertexZ);

				i += 3;
			}
		}

		// calculates the vertex indices (for drawing)
		for (int z = 0; z < numVertex - 1; z++) {
			for (int x = 0; x < numVertex - 1; x++) {
				int topLeftIndex = z * numVertex + x;
				int topRightIndex = topLeftIndex + 1;
				int bottomLeftIndex = topLeftIndex + numVertex;
				int bottomRightIndex = bottomLeftIndex + 1;

				indices.push_back((GLuint)topLeftIndex);
				indices.push_back((GLuint)bottomLeftIndex);
				indices.push_back((GLuint)topRightIndex);

				indices.push_back((GLuint)topRightIndex);
				indices.push_back((GLuint)bottomLeftIndex);
				indices.push_back((GLuint)bottomRightIndex);
			}
		}
	}

	stbi_image_free(heightMap);
}

void Terrain::calculateNormals() {
	// interpolates the vertex normals from the surrounding heights
	for (int z = 0; z < numVertex; z++) {
		for (int x = 0; x < numVertex; x++) {
			float heightXLeft, heightXRight, heightZUp, heightZDown;

			if (x == 0) {
				heightXLeft = vertices[(z * numVertex * 3) + 1];
			}

			else {
				heightXLeft = vertices[((z * numVertex + x - 1) * 3) + 1];
			}

			if (x == numVertex - 1) {
				heightXRight = vertices[((z * numVertex + x) * 3) + 1];
			}

			else {
				heightXRight = vertices[((z * numVertex + x + 1) * 3) + 1];
			}

			if (z == 0) {
				heightZUp = vertices[(x * 3) + 1];
			}

			else {
				heightZUp = vertices[(((z - 1) * numVertex + x) * 3) + 1];
			}

			if (x == numVertex - 1) {
				heightZDown = vertices[((z * numVertex + x) * 3) + 1];
			}

			else {
				heightZDown = vertices[(((z + 1) * numVertex + x) * 3) + 1];
			}

			glm::vec3 normal = glm::vec3(heightXLeft - heightXRight, 2.0f, heightZDown - heightZUp);
			normal = glm::normalize(normal);

			normals.push_back((GLfloat)normal.x);
			normals.push_back((GLfloat)normal.y);
			normals.push_back((GLfloat)normal.z);
		}
	}

}

void Terrain::loadTexture(const char* texturePath, int index) {
	int width, height, numColorChannels;
	unsigned char *texture = stbi_load(texturePath, &width, &height, &numColorChannels, 0);

	if (!texture) {
		std::cout << "Texture map #" << index << " failed to load" << std::endl;
	}

	else {
		// std::cout << width << " " << height << " " << numColorChannels << " " << sizeof(texture) << std::endl;
	}

	if (texture) {
		glGenTextures(1, &textureId[index]);
		glBindTexture(GL_TEXTURE_2D, textureId[index]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	}

	stbi_image_free(texture);
}

void Terrain::draw(GLuint shaderProgram)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureId[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureId[2]);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 2);
	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform1f(glGetUniformLocation(shaderProgram, "maxHeight"), 255.0f);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}