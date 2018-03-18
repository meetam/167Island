#include "window.h"
#include "Plant.h"

const char* window_title = "Assignment 4";
GLint plantShader;
GLint skyboxShader;
GLint terrainShader;
GLint waterShader;

// On some systems you need to change this to the absolute path
#define PLANT_VERTEX_PATH "../plantShader.vert"
#define PLANT_FRAGMENT_PATH "../plantShader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxShader.frag"
#define TERRAIN_VERTEX_SHADER_PATH "../terrainShader.vert"
#define TERRAIN_FRAGMENT_SHADER_PATH "../terrainShader.frag"
#define WATER_VERTEX_SHADER_PATH "../waterShader.vert"
#define WATER_FRAGMENT_SHADER_PATH "../waterShader.frag"

// resource files
const char* heightMap = "../res/height_map.png";
const char* sandTexture = "../res/texture_sand.jpg";
const char* groundTexture = "../res/texture_ground.jpg";
std::vector<const char*> skyFiles = {
	"../res/skybox_sky/ss_ft.tga",
	"../res/skybox_sky/ss_bk.tga",
	"../res/skybox_sky/ss_up.tga",
	"../res/skybox_sky/ss_dn.tga",
	"../res/skybox_sky/ss_rt.tga",
	"../res/skybox_sky/ss_lf.tga"
};


// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, -20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
glm::mat4 Window::P;
glm::mat4 Window::V;

Skybox* Window::skybox;
Terrain* Window::terrain;
Water* Window::water;
Plant* fernPlant;
Plant* bushPlant;
Plant* vinePlant;

const double MOUSE_ROTATION_FACTOR = 0.25;
const double MOUSE_TRANSLATION_FACTOR = 0.04;
bool Window::isMousePressed;
int Window::mouseButtonPressed;
double Window::xPrevious;
double Window::yPrevious;

void Window::initialize_objects()
{
	// loads the shader programs
	plantShader = LoadShaders(PLANT_VERTEX_PATH, PLANT_FRAGMENT_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	terrainShader = LoadShaders(TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH);
	waterShader = LoadShaders(WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH);

	// creates the plants
	// Parameters: type, shader, position, color, start angle, angle delta, draw size, iterations
	fernPlant = new Plant("fern", plantShader, glm::vec3(450.0f, -300.0f, 1000.0f), glm::vec3(0.42f, 0.557f, 0.137f), 60.0f, 25.0f, 20.0f, 5);
	bushPlant = new Plant("bush", plantShader, glm::vec3(0.0f, -300.0f, 500.0f), glm::vec3(0.133f, 0.545f, 0.133f), 95.0f, 22.5f, 5.0f, 4);
	vinePlant = new Plant("vine", plantShader, glm::vec3(100.0f, -300.0f, 500.0f), glm::vec3(0.133f, 0.7f, 0.133f), 90.0f, 25.7f, 5.0f, 5);

	// creates the skybox
	// Parameters: vector of file paths
	skybox = new Skybox(skyFiles);

	// creates the terrain
	// Parameters: number of vectors (size of height map), size of terrain, height map file path
	float terrainSize = 800.f;
	float waterHeight = 125.0f;
	terrain = new Terrain(256, terrainSize, heightMap);
	terrain->loadTexture(sandTexture, 0);
	terrain->loadTexture(groundTexture, 1);

	// creates the water
	water = new Water(terrainSize, waterHeight);
	
	// configure initial settings
	isMousePressed = false;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(fernPlant);
	delete(bushPlant);
	delete(vinePlant);
	delete(skybox);
	delete(terrain);
	delete(water);

	glDeleteProgram(plantShader);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(terrainShader);
	glDeleteProgram(waterShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draws the plants
	glUseProgram(plantShader);
	//fernPlant->draw();
	//bushPlant->draw();
	//vinePlant->draw();

	// draws the terrain
	glUseProgram(terrainShader);
	GLuint uView = glGetUniformLocation(terrainShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &V[0][0]);
	GLuint uProjection = glGetUniformLocation(terrainShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);
	terrain->draw(terrainShader);

	// draws the water
	glUseProgram(waterShader);
	uView = glGetUniformLocation(waterShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &V[0][0]);
	uProjection = glGetUniformLocation(waterShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);
	water->draw(waterShader);

	// draws the skybox
	glUseProgram(skyboxShader);
	glm::mat4 skyboxView = glm::mat4(glm::mat3(V));
	uView = glGetUniformLocation(skyboxShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &skyboxView[0][0]);
	uProjection = glGetUniformLocation(skyboxShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);
	skybox->draw(skyboxShader);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	}
}

void Window::mouse_button_callback(GLFWwindow* window, int key, int action, int mods) {
	if (action == GLFW_PRESS) {
		isMousePressed = true;
		mouseButtonPressed = key;
	}

	else if (action == GLFW_RELEASE) {
		isMousePressed = false;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
	if (isMousePressed) {
		switch (mouseButtonPressed) {
			case GLFW_MOUSE_BUTTON_LEFT: {
				glm::vec3 previousPosition = trackBallMapping(xPrevious, yPrevious);
				glm::vec3 currentPosition = trackBallMapping(x, y);

				float dotProduct = glm::dot(previousPosition, currentPosition);
				if (dotProduct < 0.001f) dotProduct = 0.001f;
				if (dotProduct > 0.999f) dotProduct = 0.999f;

				float rotationAngle = acos(dotProduct) * MOUSE_ROTATION_FACTOR;
				glm::vec3 rotationAxis = glm::cross(previousPosition, currentPosition);
				
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);
				cam_pos = glm::vec3(rotationMatrix * glm::vec4(cam_pos, 1.0f));
				cam_up = glm::inverse(glm::transpose(glm::mat3(rotationMatrix))) * cam_up;
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			}
		}
	}

	// updates the previous cursor positions
	xPrevious = x;
	yPrevious = y;
}

void Window::scroll_callback(GLFWwindow* window, double x, double y) {
	// uses negative y value since the positive z direction is outward
	// scrolling up has positive values but has to go in the negative z direction
	cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), glm::normalize(-cam_pos) * float(y)) * glm::vec4(cam_pos, 1.0f));
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

glm::vec3 Window::trackBallMapping(double x, double y) {
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse

	v.x = (2.0 * x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (height - 2.0 * y) / height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below

	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line

	v.z = sqrtf(1.001 - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.

	return v;  // return the mouse location on the surface of the trackball
}

