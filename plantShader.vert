#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
uniform vec4 clippingPlane;

out vec3 normalColor;

void main()
{
	vec4 worldCoordinates = model * vec4(position, 1.0f);
    gl_Position = projection * view * worldCoordinates;
    gl_ClipDistance[0] = dot(worldCoordinates, clippingPlane);
    normalColor = color;
}
