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
    //vec3 offset = offsets[gl_InstanceID];
    gl_Position = projection * view *  model * vec4(position, 1.0f);
    gl_ClipDistance[0] = dot(gl_Position, clippingPlane);
    normalColor = color;
}
