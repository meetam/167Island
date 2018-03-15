#ifndef TERRAIN_H
#define TERRAIN_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Terrain
{
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> indices;

	// shader program variables
	GLuint VAO, VBO_vertex, VBO_normal, EBO;

	unsigned int textureId[2];

public:
	Terrain();
	Terrain(int numVertex, int size, const char* heightMapPath);
	~Terrain();

	void readHeightMap(const char* heightMapPath);
	void calculateNormals(int numVertex);
	void loadTexture(const char* texturePath, int index);
	void draw(GLuint shaderProgram);
};

#endif