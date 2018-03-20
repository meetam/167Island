#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

uniform float size;				// size/dimensions of water (from Water)
uniform mat4 view;				// view matrix (from Window)
uniform mat4 projection;		// projection matrix (from Window)
uniform vec3 cameraPosition;	// position of camera in world space (from Window)
uniform vec3 lightPosition;		// position of point light in world space (from Window)

out vec4 projectionCoordinates;
out vec2 distortionCoordinates;
out vec3 toCameraVector;
out vec3 toLightVector;

void main()
{
	projectionCoordinates = projection * view * vec4(position, 1.0f);
	distortionCoordinates = (position.xz) / size + 0.5f;
	toCameraVector = cameraPosition - position;
	toLightVector = lightPosition - position;

	gl_Position = projectionCoordinates;

}
