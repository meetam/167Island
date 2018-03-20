#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

uniform mat4 view;				// from Window
uniform mat4 projection;		// from Window
uniform vec3 cameraPosition;	// from Window
uniform vec3 lightPosition;		// from Window
uniform float waterSize;

out vec4 projectionCoordinates;
out vec2 distortionCoordinates;
out vec3 toCameraVector;
out vec3 toLightVector;

void main()
{
	projectionCoordinates = projection * view * vec4(position, 1.0f);
	distortionCoordinates = (position.xz) / waterSize + 0.5f;
	toCameraVector = cameraPosition - position;
	toLightVector = lightPosition - position;
	gl_Position = projectionCoordinates;

}
