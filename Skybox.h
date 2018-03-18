#ifndef SKYBOX_H
#define SKYBOX_H

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

class Skybox
{
private:
	std::vector<GLfloat> vertices;

	GLuint VAO, VBO; // note to self: there is no need to make EBO with this vertex definition (because glDrawArrays handles it)
	unsigned int textureId;

public:
	Skybox();
	Skybox(std::vector<const char*> cubeLocations);
	~Skybox();

	void initializeVertices();
	void loadTexture(std::vector<const char*> cubeLocations);
	void draw(GLuint shaderProgram);
};

#endif