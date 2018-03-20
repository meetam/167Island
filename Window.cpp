#include "Window.h"
#include "Skybox.h"
#include "Plant.h"

bool scaleSet = false;
glm::vec3 offsets[25];
GLuint quadVAO;
int curveIndex = 0;

const char* window_title = "Assignment 4";
GLint plantShader;
GLint skyboxShader;
GLint terrainShader;
GLint waterShader;
GLint fogShader;

// On some systems you need to change this to the absolute path
/*#define PLANT_VERTEX_PATH "../plantShader.vert"
#define PLANT_FRAGMENT_PATH "../plantShader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxShader.frag"
#define TERRAIN_VERTEX_SHADER_PATH "../terrainShader.vert"
#define TERRAIN_FRAGMENT_SHADER_PATH "../terrainShader.frag"
#define WATER_VERTEX_SHADER_PATH "../waterShader.vert"
#define WATER_FRAGMENT_SHADER_PATH "../waterShader.frag"
#define FOG_VERTEX_SHADER_PATH "../fogShader.vert"
#define FOG_FRAGMENT_SHADER_PATH "../fogShader.frag"

// resource files
const char* heightMap = "../res/height_map.png";
const char* sandTexture = "../res/texture_sand.jpg";
const char* groundTexture = "../res/texture_ground.jpg";
const char* dudvMap = "../res/dudv.png";
const char* normalMap = "../res/normal.png";
std::vector<const char*> skyFiles = {
	"../res/skybox_sky/ss_ft.tga",
	"../res/skybox_sky/ss_bk.tga",
	"../res/skybox_sky/ss_up.tga",
	"../res/skybox_sky/ss_dn.tga",
	"../res/skybox_sky/ss_rt.tga",
	"../res/skybox_sky/ss_lf.tga"
};*/

// On some systems you need to change this to the absolute path
#define PLANT_VERTEX_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/plantShader.vert"
#define PLANT_FRAGMENT_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/plantShader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/skyboxShader.frag"
#define TERRAIN_VERTEX_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/terrainShader.vert"
#define TERRAIN_FRAGMENT_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/terrainShader.frag"
#define WATER_VERTEX_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/waterShader.vert"
#define WATER_FRAGMENT_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/waterShader.frag"
#define FOG_VERTEX_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/fogShader.vert"
#define FOG_FRAGMENT_SHADER_PATH "/Users/Meeta/Desktop/CSE 167/HW4/HW4/fogShader.frag"

// resource files
const char* heightMap = "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/height_map.png";
const char* sandTexture = "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/texture_sand.jpg";
const char* groundTexture = "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/texture_ground.jpg";
const char* dudvMap = "/Users/Meeta/Desktop/CSE 167/HW4/HW4//res/dudv.png";
const char* normalMap = "/Users/Meeta/Desktop/CSE 167/HW4/HW4//res/normal.png";
std::vector<const char*> skyFiles = {
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_ft.tga",
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_bk.tga",
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_up.tga",
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_dn.tga",
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_rt.tga",
    "/Users/Meeta/Desktop/CSE 167/HW4/HW4/res/skybox_sky/ss_lf.tga"
};

// Default camera parameters
glm::vec3 cam_pos(0.0f, 175.0f, -400.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
glm::mat4 Window::P;
glm::mat4 Window::V;

Skybox* Window::skybox;
Terrain* Window::terrain;
Water* Window::water;
glm::vec3 Window::pointLightPosition;
glm::vec3 Window::pointLightColor;
std::vector<Plant*> fernPlants;
std::vector<Plant*> bushPlants;
std::vector<Plant*> vinePlants;

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
    fogShader = LoadShaders(FOG_VERTEX_SHADER_PATH, FOG_FRAGMENT_SHADER_PATH);

    // creates the plants
    initialize_plants();

	// creates the skybox
	skybox = new Skybox(skyFiles);

	// creates the terrain
	float terrainSize = 800.f;
	terrain = new Terrain(256, terrainSize, heightMap);
	terrain->loadTexture(sandTexture, 0);
	terrain->loadTexture(groundTexture, 1);

	// creates the water
	float waterHeight = 125.0f;
	glm::vec3 beachWaterColor = glm::vec3(0.03f, 0.53f, 0.58f);
	water = new Water(terrainSize, waterHeight, beachWaterColor);
	water->readDudvMap(dudvMap);
	water->readNormalMap(normalMap);

	// configures the point light
	pointLightPosition = glm::vec3(0.0f, 300.0f, 0.0f);
	pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	
	// configures initial settings
	isMousePressed = false;
}

void Window::initialize_plants()
{
    /**
     * POSITIONS GUIDE:
     *     left - positive x
     *     right - negative x
     *     back - positive z
     *     forward - negative z
     */
    
    // creates the fern plants
    glm::vec3 fernPos[] = {
        // cluster 1
        glm::vec3(260.0f, 150.0f, -240.0f),
        glm::vec3(270.0f, 150.0f, -260.0f),
        glm::vec3(250.0f, 150.0f, -280.0f),
        glm::vec3(230.0f, 160.0f, -280.0f),
        glm::vec3(230.0f, 160.0f, -260.0f),
        // cluster 2
        glm::vec3(230.0f, 150.0f, 100.0f),
        glm::vec3(230.0f, 160.0f, 50.0f),
        glm::vec3(210.0f, 160.0f, 30.0f),
        glm::vec3(160.0f, 160.0f, 30.0f),
        // cluster 3
        glm::vec3(0.0f, 140.0f, 150.0f),
        glm::vec3(-40.0f, 140.0f, 150.0f),
        glm::vec3(-50.0f, 140.0f, 170.0f),
        glm::vec3(-50.0f, 140.0f, 180.0f),
        glm::vec3(-80.0f, 140.0f, 180.0f),
        // cluster 4
        glm::vec3(-210.0f, 150.0f, -200.0f),
        glm::vec3(-210.0f, 140.0f, -320.0f),
        glm::vec3(-240.0f, 150.0f, -320.0f),
        glm::vec3(-250.0f, 140.0f, -360.0f),
        glm::vec3(-210.0f, 140.0f, -370.0f)
    };
    float drawSize = 1.0f;
    float startAngle = 60.0f;
    for (int i = 0; i < 19; i++)
    {
        // Parameters: type, shader, position, color, start angle, angle delta, draw size, iterations
        Plant * fernPlant = new Plant("fern", fogShader, fernPos[i], glm::vec3(0.42f, 0.557f, 0.137f), startAngle, 25.0f, drawSize, 4);
        fernPlants.push_back(fernPlant);
        if (i % 2 == 0)
            startAngle += 20.0f;
        else
            startAngle -= 20.0f;
    }
    
    // creates the bush plants
    glm::vec3 bushPos[] = {
        glm::vec3(240.0f, 140.0f, -240.0f),
        glm::vec3(200.0f, 120.0f, 100.0f),
        glm::vec3(0.0f, 150.0f, 200.0f),
        glm::vec3(-200.0f, 120.0f, 0.0f)
    };
    drawSize = 2.0f;
    int iterations = 3;
    for (int i = 0; i < 4; i++)
    {
        // Parameters: type, shader, position, color, start angle, angle delta, draw size, iterations
        Plant * bushPlant = new Plant("bush", fogShader, bushPos[i], glm::vec3(0.133f, 0.545f, 0.133f), 95.0f, 22.5f, drawSize, iterations);
        bushPlants.push_back(bushPlant);
        if (i % 2 == 0)
            drawSize -= 1.0f;
        else
            drawSize += 1.0f;
        if (i == 1)
            iterations = 4;
    }
    
    
    // creates the vine plants
    glm::vec3 vinePos[] = {
        glm::vec3(390.0f, 120.0f, -390.0f),
        glm::vec3(240.0f, 120.0f, -390.0f),
        glm::vec3(90.0f, 120.0f, -390.0f),
        glm::vec3(-60.0f, 120.0f, -390.0f),
        glm::vec3(-110.0f, 120.0f, -390.0f),
        glm::vec3(-120.0f, 120.0f, -290.0f),
        glm::vec3(-170.0f, 150.0f, -200.0f),
        glm::vec3(-240.0f, 120.0f, -150.0f),
        glm::vec3(-250.0f, 120.0f, -90.0f),
        glm::vec3(-250.0f, 120.0f, 60.0f),
        glm::vec3(-300.0f, 120.0f, 200.0f),
        glm::vec3(-390.0f, 120.0f, 390.0f),
        glm::vec3(-320.0f, 120.0f, 390.0f),
        glm::vec3(-220.0f, 120.0f, 380.0f),
        glm::vec3(-120.0f, 120.0f, 370.0f),
        glm::vec3(0.0f, 120.0f, 370.0f),
        glm::vec3(0.0f, 120.0f, 250.0f),
        glm::vec3(200.0f, 120.0f, 370.0f),
        glm::vec3(390.0f, 120.0f, 370.0f),
        glm::vec3(390.0f, 120.0f, 200.0f)
    };
    drawSize = 0.1f;
    for (int i = 0; i < 20; i++)
    {
        // Parameters: type, shader, position, color, start angle, angle delta, draw size, iterations
        Plant * vinePlant = new Plant("vine", fogShader, vinePos[i], glm::vec3(0.133f, 0.7f, 0.133f), 90.0f, 25.7f, drawSize, 5);
        vinePlants.push_back(vinePlant);
        
        if (i % 3 == 0)
        {
            drawSize += 0.1f;
        }
        else if (i % 3 == 1)
        {
            drawSize -= 0.1f;
        }
    }
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    for (Plant* fernPlant : fernPlants)
    {
        delete(fernPlant);
    }
    for (Plant* bushPlant : bushPlants)
    {
        delete(bushPlant);
    }
    for (Plant * vinePlant : vinePlants)
    {
        delete(vinePlant);
    }
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
	water->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// sets up the water reflection texture
	float cameraToWaterDistance = cam_pos.y - water->getHeight();
	float lookAtToWaterDistance = cam_look_at.y - water->getHeight();
	cam_pos.y -= 2 * cameraToWaterDistance;
	cam_look_at.y -= 2 * lookAtToWaterDistance;
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	water->bindFrameBuffer(Water::REFLECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTerrain(Water::REFLECTION);
	drawPlants(Water::REFLECTION);
	drawSkybox();
	cam_pos.y += 2 * cameraToWaterDistance;
	cam_look_at.y += 2 * lookAtToWaterDistance;
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	
	// sets up the water refraction texture
	water->bindFrameBuffer(Water::REFRACTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTerrain(Water::REFRACTION);
	drawPlants(Water::REFRACTION);
	drawSkybox();

	// draws the actual scene
	water->unbindFrameBuffer(width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTerrain(Water::NONE);
	drawWater();
	drawPlants(Water::NONE);
	drawSkybox();

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

		// case when lower case a is pressed: move left
		else if (key == GLFW_KEY_A) {
			cam_pos.x += 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// case when lower case d is pressed: move right
		else if (key == GLFW_KEY_D) {
			cam_pos.x -= 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// case when lower case w is pressed: move front
		else if (key == GLFW_KEY_W) {
			cam_pos.z += 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// case when lower case s is pressed: move back
		else if (key == GLFW_KEY_S) {
			cam_pos.z -= 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// case when lower case w is pressed: move up
		else if (key == GLFW_KEY_O) {
			cam_pos.y += 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// case when lower case s is pressed: move down
		else if (key == GLFW_KEY_L) {
			cam_pos.y -= 10.0f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
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
				//cam_up = glm::inverse(glm::transpose(glm::mat3(rotationMatrix))) * cam_up;
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

void Window::drawSkybox() {
	glUseProgram(skyboxShader);

	glm::mat4 skyboxView = glm::mat4(glm::mat3(V));
	GLuint uView = glGetUniformLocation(skyboxShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &skyboxView[0][0]);

	GLuint uProjection = glGetUniformLocation(skyboxShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);

	skybox->draw(skyboxShader);
}

void Window::drawTerrain(int renderType) {
	glUseProgram(terrainShader);

	water->loadClippingPlane(terrainShader, renderType);

	GLuint uView = glGetUniformLocation(terrainShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &V[0][0]);

	GLuint uProjection = glGetUniformLocation(terrainShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);

	terrain->draw(terrainShader);
}

void Window::drawWater() {
	glUseProgram(waterShader);

	GLuint uView = glGetUniformLocation(waterShader, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, &V[0][0]);

	GLuint uProjection = glGetUniformLocation(waterShader, "projection");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);

	GLuint uCameraPosition = glGetUniformLocation(waterShader, "cameraPosition");
	glUniform3fv(uCameraPosition, 1, &cam_pos[0]);

	GLuint uLightPosition = glGetUniformLocation(waterShader, "lightPosition");
	glUniform3fv(uLightPosition, 1, &pointLightPosition[0]);

	GLuint uLightColor = glGetUniformLocation(waterShader, "lightColor");
	glUniform3fv(uLightColor, 1, &pointLightColor[0]);

	water->draw(waterShader);
}

void Window::drawPlants(int renderType) {
	glUseProgram(fogShader);

	water->loadClippingPlane(plantShader, renderType);

	for (Plant* vPlant : vinePlants)
	{
		vPlant->draw();
	}
	for (Plant* bPlant : bushPlants)
	{
		bPlant->draw();
	}
	for (Plant* fPlant : fernPlants)
	{
		fPlant->draw();
	}
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

