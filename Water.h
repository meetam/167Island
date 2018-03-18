#ifndef WATER_H
#define WATER_H

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

class Water
{
protected:
	std::vector<GLfloat> vertices;

	// shader program variables
	GLuint VAO, VBO;

public:
	Water();
	Water(float size, float height);
	~Water();

	void draw(GLuint shaderProgram);
};

#endif