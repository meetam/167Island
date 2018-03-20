#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 outPosition;
out vec3 outNormal;

uniform mat4 view;			// from Window
uniform mat4 projection;	// from Window
uniform vec4 clippingPlane; // from Water

void main()
{
	outPosition = position;
    outNormal = normal;
	gl_Position = projection * view * vec4(position, 1.0f);
	gl_ClipDistance[0] = dot(vec4(position, 1.0f), clippingPlane);
}
