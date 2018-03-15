#include "Terrain.h"
#include "Window.h"
#include "stb_image.h"

Terrain::Terrain() { }

Terrain::Terrain(int numVertex, int size, const char* heightMapPath) {
	// calculates the vertex positions
	int i = 0;
	for (int z = -numVertex / 2; z < numVertex / 2; z++) {
		for (int x = -numVertex / 2; x < numVertex / 2; x++) {
			float vertexX = (x * 1.0 / numVertex) * size;
			float vertexY = 0.0f;
			float vertexZ = (z * 1.0 / numVertex) * size;

			vertices.push_back((GLfloat) vertexX);
			vertices.push_back((GLfloat) vertexY);
			vertices.push_back((GLfloat) vertexZ);
		}
	}

	// calculates the vertex indices (for drawing)
	for (int z = 0; z < numVertex - 1; z++) {
		for (int x = 0; x < numVertex - 1; x++) {
			int topLeftIndex = z * numVertex + x;
			int topRightIndex = topLeftIndex + 1;
			int bottomLeftIndex = topLeftIndex + numVertex;
			int bottomRightIndex = bottomLeftIndex + 1;

			indices.push_back((GLuint) topLeftIndex);
			indices.push_back((GLuint) bottomLeftIndex);
			indices.push_back((GLuint) topRightIndex);

			indices.push_back((GLuint) topRightIndex);
			indices.push_back((GLuint) bottomLeftIndex);
			indices.push_back((GLuint) bottomRightIndex);
		}
	}

	readHeightMap(heightMapPath);
	calculateNormals(numVertex);

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
		// iterates with an offset of 1 (for y index)
		for (int i = 1; i < vertices.size(); i += 3) {
			vertices[i] = heightMap[i];
		}
	}

	stbi_image_free(heightMap);
}

void Terrain::calculateNormals(int numVertex) {
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
	int w, h, numColorChannels;

	glGenTextures(1, &textureId[index]);
	glBindTexture(GL_TEXTURE_2D, textureId[index]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char *data = stbi_load(texturePath, &w, &h, &numColorChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	else {
		std::cout << "Terrain texture failed to load at path: " << texturePath << std::endl;
	}

	stbi_image_free(data);
}

void Terrain::draw(GLuint shaderProgram)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureId[1]);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}