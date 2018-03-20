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

	// terrain information variables
	float numVertex;	// number of vertices on each side
	float size;			// size of terrain in world coordinates (from -size/2 to size/2)

	// shader program variables
	GLuint VAO, VBO_vertex, VBO_normal, EBO;

	// texture variables
	unsigned int textureId[3];

public:
	Terrain();
	Terrain(float size, const char* heightMapPath);
	~Terrain();

	void readHeightMap(const char* heightMapPath);
	void calculateNormals();
	void loadTexture(const char* texturePath, int index);
	void draw(GLuint shaderProgram);
};

#endif
