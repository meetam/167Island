#version 330 core

in vec3 outPosition;
in vec3 outNormal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	color = vec4(0.25f, 0.64f, 0.99f, 0.1f);
}