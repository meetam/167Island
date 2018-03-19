#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

uniform mat4 view;			// from Window
uniform mat4 projection;	// from Window

out vec4 projectionCoordinates;

void main()
{
	projectionCoordinates = projection * view * vec4(position, 1.0f);
	gl_Position = projectionCoordinates;
}
