#version 330 core

in vec3 outPosition;
in vec3 outNormal;

uniform sampler2D texture1;
uniform sampler2D texture2;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	float height = outPosition.y / 255.0f;
	color = mix(texture(texture1, outPosition.xz), texture(texture2, outPosition.xz), height * height);

	if (outPosition.y < 125.0f) {
		//color = mix(texture(texture1, outPosition.xz), texture(texture2, outPosition.xz), 0.1);
	}
}