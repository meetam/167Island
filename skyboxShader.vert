#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

out vec3 textureCoordinates;

uniform mat4 view;			// from Window
uniform mat4 projection;	// from Window

void main()
{
	textureCoordinates = position;
	vec4 newPosition = projection * view * vec4(position, 1.0f);
	gl_Position = newPosition.xyww;
}
