#version 330 core

in vec4 projectionCoordinates;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	vec2 textureCoordinates = (projectionCoordinates.xy / projectionCoordinates.w) / 2.0f + 0.5f;

	vec2 reflectionCoordinates = vec2(textureCoordinates.x, -textureCoordinates.y);
	vec2 refractionCoordinates = textureCoordinates;

	vec4 reflectionColor = texture(reflectionTexture, reflectionCoordinates);
	vec4 refractionColor = texture(refractionTexture, refractionCoordinates);

	color = reflectionColor;
}