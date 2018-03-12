#include "Window.h"
#include "Skybox.h"
#include "Curve.h"
#include "Plant.h"

const char * window_title = "Assignment 4";
Skybox * box;
Plant * fernPlant;

GLint plantShader, skyboxShader;
bool scaleSet = false;
glm::vec3 offsets[25];
GLuint quadVAO;
int curveIndex = 0;

// On some systems you need to change this to the absolute path
#define PLANT_VERTEX_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/plantShader.vert"
#define PLANT_FRAGMENT_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/plantShader.frag"
#define BOX_VERTEX_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/boxShader.vert"
#define BOX_FRAGMENT_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/boxShader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, -20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
    // Load the shader program. Make sure you have the correct filepath up top
    plantShader = LoadShaders(PLANT_VERTEX_PATH, PLANT_FRAGMENT_PATH);
    skyboxShader = LoadShaders(BOX_VERTEX_SHADER_PATH, BOX_FRAGMENT_SHADER_PATH);
    
    box = new Skybox(skyboxShader, width, height);
    
    //Parameters: type, shader, position, color, start angle, angle delta, draw size, iterations
    fernPlant = new Plant("fern", plantShader, glm::vec3(600.0f, -300.0f, 1000.0f), glm::vec3(0.42f, 0.557f, 0.137f), 60.0f, 25.0f, 10.0f, 10);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(plantShader);
    glDeleteProgram(skyboxShader);
    delete box;
    delete fernPlant;
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
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 2000.0f);
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

	// Use the shader of programID
	glUseProgram(skyboxShader);
    box->draw();
    
    glUseProgram(plantShader);
    
    fernPlant->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

bool rDown = false;
bool lDown = false;

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
    
    // Check for control button
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if (action == GLFW_PRESS)
            rDown = true;
        if (action == GLFW_RELEASE)
            rDown = false;
    }
}

double xPos1, xPos2;
double yPos1, yPos2;
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            lDown = true;
            glfwGetCursorPos(window, &xPos1, &yPos1);
        }
        if (action == GLFW_RELEASE)
        {
            lDown = false;
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    glm::vec4 cam (cam_pos, 1.0f);
    if (yoffset < 0)
    {
        glm::mat4 positiveZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f));
        cam = positiveZ * cam;
    }
    
    else if (yoffset > 0)
    {
        glm::mat4 negativeZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        cam = negativeZ * cam;
    }
    
    cam_pos = glm::vec3(cam.x, cam.y, cam.z);
    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    xPos2 = xpos;
    yPos2 = ypos;
    
    if (rDown)
    {
    }
    
    if (lDown)
    {
        glm::vec3 v1 = trackballCalculation(xPos1, yPos1);
        glm::vec3 v2 = trackballCalculation(xPos2, xPos2);
        glm::vec3 begin (v1.x, v1.y, v1.z);
        glm::vec3 end (v2.x, v2.y, v2.z);
        glm::vec3 axis = glm::cross(v2, v1);
        float angle = glm::dot(v1, v2) / (v1.length() * v2.length()); /// 180.0f * glm::pi<float>();
        angle = acos(angle);
        glm::vec4 cameraPos (cam_pos, 0);
        cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * cameraPos;
        cam_pos = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z);
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    }
}

glm::vec3 Window::trackballCalculation(double x, double y)
{
    glm::vec3 v;
    v.x = (2.0f * (float)x - width) / width;
    v.y = (height - 2.0f * (float)y) / height;
    v.z = 0.0f;
    float d = glm::length(v); //v.length();
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f - d*d);
    v = glm::normalize(v);
    return v;
}
