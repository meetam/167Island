#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "shader.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Water.h"
#include "Plant.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P;	// projection matrix
	static glm::mat4 V;	// view matrix

	// graphics-related variables
	static Skybox* skybox;
	static Terrain* terrain;
	static Water* water;
	static glm::vec3 pointLightPosition;
	static glm::vec3 pointLightColor;

	// mouse-related variables
	static bool isMousePressed;
	static int mouseButtonPressed;
	static double xPrevious;
	static double yPrevious;

	// initializer functions
	static void initialize_objects();
    static void initialize_plants();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	
	// callback functions
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void mouse_button_callback(GLFWwindow* window, int key, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);

	// draw functions
	static void drawSkybox();
	static void drawTerrain(int renderType);
	static void drawWater();
	static void drawPlants(int renderType);

	// helper function for the cursor position callback
	static glm::vec3 trackBallMapping(double x, double y);
};

#endif
