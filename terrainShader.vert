#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform float size;			// size of terrain (from Terrain)
uniform mat4 view;			// view matrix (from Window)
uniform mat4 projection;	// projection matrix (from Window)
uniform vec4 clippingPlane; // clipping plane for reflection/refraction purposes (from Water)

out vec2 texturePosition;
out float height;

void main()
{
	texturePosition = position.xz / size + 0.5f;
	height = position.y;

	gl_Position = projection * view * vec4(position, 1.0f);
	gl_ClipDistance[0] = dot(vec4(position, 1.0f), clippingPlane);
}
