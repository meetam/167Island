#version 330 core

in vec3 outPosition;
in vec3 outNormal;

uniform sampler2D texture1;
uniform sampler2D texture2;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	//color = vec4(outNormal, 1.0f);
	color = mix(texture(texture1, outPosition.xz), texture(texture2, outPosition.xz), 0.4);
	//color = texture(texture1, outPosition.xz);
	
	if (outPosition.y <= 125) {
		color = color * vec4(0.25f, 0.64f, 0.99f, 0.1f);
	}
}