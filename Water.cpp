#include "Water.h"
#include "stb_image.h"

Water::Water() { }

Water::Water(float size, float height, float speed, glm::vec3 color) {
	this->size = size;
	this->height = height;
	this->speed = speed;
	this->waterColor = color;
	this->distortionOffset = 0.0f;
	initializeVertices(size, height);

	initializeVertexBuffer();
	initializeFrameBuffer(&FBO_reflection, &reflectionColorTexture, &reflectionDepthTexture, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	initializeFrameBuffer(&FBO_refraction, &refractionColorTexture, &refractionDepthTexture, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

Water::~Water() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteFramebuffers(1, &FBO_reflection);
	glDeleteFramebuffers(1, &FBO_refraction);

	glDeleteTextures(1, &reflectionColorTexture);
	glDeleteTextures(1, &reflectionDepthTexture);
	glDeleteTextures(1, &refractionColorTexture);
	glDeleteTextures(1, &refractionDepthTexture);
	glDeleteTextures(1, &dudvMapTexture);
	glDeleteTextures(1, &normalMapTexture);
}

float Water::getHeight() {
	return this->height;
}

void Water::initializeVertices(float size, float height) {
	float half = size / 2.0;

	vertices = {
		-half, height, -half,
		-half, height,  half,
		half, height, -half,

		half, height, -half,
		-half, height,  half,
		half, height,  half
	};
}

void Water::initializeVertexBuffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Water::initializeFrameBuffer(GLuint* FBO, unsigned int* colorTextureId, unsigned int* depthTextureId, int width, int height) {
	// creates the frame buffer
	glGenFramebuffers(1, FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	// creates the color texture and binds it to the FBO
	createColorTexture(colorTextureId, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *colorTextureId, 0);

	// creates the depth texture and binds it to the FBO
	createDepthTexture(depthTextureId, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *depthTextureId, 0);

	// tells opengl to draw on the color texture
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// checks that the frame buffer was set up correctly
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Water framebuffer was not initialized correctly." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::createColorTexture(unsigned int* textureId, int width, int height) {
	glGenTextures(1, textureId);
	glBindTexture(GL_TEXTURE_2D, *textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Water::createDepthTexture(unsigned int* textureId, int width, int height) {
	glGenTextures(1, textureId);
	glBindTexture(GL_TEXTURE_2D, *textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Water::readDudvMap(const char* dudvMapPath) {
	int width, height, numColorChannels;
	unsigned char *dudvMap = stbi_load(dudvMapPath, &width, &height, &numColorChannels, 0);

	if (!dudvMap) {
		std::cout << "DuDv map failed to load" << std::endl;
	}

	else {
		// std::cout << width << " " << height << " " << numColorChannels << " " << sizeof(dudvMap) << std::endl;
	}

	if (dudvMap) {
		glGenTextures(1, &dudvMapTexture);
		glBindTexture(GL_TEXTURE_2D, dudvMapTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dudvMap);
	}

	stbi_image_free(dudvMap);
}

void Water::readNormalMap(const char* normalMapPath) {
	int width, height, numColorChannels;
	unsigned char *normalMap = stbi_load(normalMapPath, &width, &height, &numColorChannels, 0);

	if (!normalMap) {
		std::cout << "Normal map failed to load" << std::endl;
	}

	else {
		// std::cout << width << " " << height << " " << numColorChannels << " " << sizeof(normalMap) << std::endl;
	}

	if (normalMap) {
		glGenTextures(1, &normalMapTexture);
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, normalMap);
	}

	stbi_image_free(normalMap);
}

void Water::update() {
	distortionOffset += speed;
	if (distortionOffset > 1.0f) distortionOffset -= 1.0f;
}

void Water::bindFrameBuffer(int type) {
	// makes sure there is no bound texture
	glBindTexture(GL_TEXTURE_2D, 0);

	if (type == REFLECTION) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_reflection);
		glViewport(0, 0, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	}

	else if (type == REFRACTION) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_refraction);
		glViewport(0, 0, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	}
}

void Water::unbindFrameBuffer(int windowWidth, int windowHeight) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
}

void Water::loadClippingPlane(GLuint shaderProgram, int type) {
	if (type == REFLECTION) {
		glm::vec4 clippingPlane = glm::vec4(0.0f, 1.0f, 0.0f, -height + 1.0f);
		GLuint uClippingPlane = glGetUniformLocation(shaderProgram, "clippingPlane");
		glUniform4fv(uClippingPlane, 1, &clippingPlane[0]);
	}

	else if (type == REFRACTION) {
		glm::vec4 clippingPlane = glm::vec4(0.0f, -1.0f, 0.0f, height + 20.0f);
		GLuint uClippingPlane = glGetUniformLocation(shaderProgram, "clippingPlane");
		glUniform4fv(uClippingPlane, 1, &clippingPlane[0]);
	}

	// case for rendering to the screen
	else {
		glm::vec4 clippingPlane = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		GLuint uClippingPlane = glGetUniformLocation(shaderProgram, "clippingPlane");
		glUniform4fv(uClippingPlane, 1, &clippingPlane[0]);
	}
}

void Water::loadHeight(GLuint shaderProgram) {
	glUniform1f(glGetUniformLocation(shaderProgram, "waterHeight"), height);
}

void Water::draw(GLuint shaderProgram)
{
	GLuint uWaterColor = glGetUniformLocation(shaderProgram, "waterColor");
	glUniform3fv(uWaterColor, 1, &waterColor[0]);

	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudvTexture"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "normalTexture"), 3);
	glUniform1f(glGetUniformLocation(shaderProgram, "distortionOffset"), distortionOffset);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionColorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionColorTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvMapTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}