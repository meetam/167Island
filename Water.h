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

#include <iostream>
#include <vector>

class Water
{
protected:
	// reflection and refraction variables
	static const int REFLECTION_WIDTH = 320;	// half of window resolution (see main.cpp)
	static const int REFLECTION_HEIGHT = 240;
	static const int REFRACTION_WIDTH = 640;	// full window resolution
	static const int REFRACTION_HEIGHT = 480;

	float height;
	std::vector<GLfloat> vertices;

	// shader program variables
	GLuint VAO, VBO;
	GLuint FBO_reflection, FBO_refraction;
	unsigned int reflectionColorTexture, reflectionDepthTexture;
	unsigned int refractionColorTexture, refractionDepthTexture;

public:
	static const int NONE = -1;
	static const int REFLECTION = 0;
	static const int REFRACTION = 1;

	Water();
	Water(float size, float height);
	~Water();

	float getHeight();

	// helper methods for initialization
	void initializeVertices(float size, float height);
	void initializeVertexBuffer();
	void initializeFrameBuffer(GLuint* FBO, unsigned int* colorTextureId, unsigned int* depthTextureId, int width, int height);
	void createColorTexture(unsigned int* textureId, int width, int height);
	void createDepthTexture(unsigned int* textureId, int width, int height);

	void bindFrameBuffer(int type);
	void unbindFrameBuffer(int windowWidth, int windowHeight);
	void loadClippingPlane(GLuint shaderProgram, int type);
	void draw(GLuint shaderProgram);

};

#endif