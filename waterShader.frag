#version 330 core

in vec4 projectionCoordinates;
in vec2 distortionCoordinates;
in vec3 toCameraVector;
in vec3 toLightVector;

uniform vec3 waterColor;
uniform vec3 lightColor;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;
uniform float distortionOffset;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

const float distortionStrength = 0.01;

vec3 calculatePointLightSpecular(vec3 normal);

void main()
{
	vec2 textureCoordinates = (projectionCoordinates.xy / projectionCoordinates.w) / 2.0f + 0.5f;

	vec2 reflectionCoordinates = vec2(textureCoordinates.x, -textureCoordinates.y);
	vec2 refractionCoordinates = textureCoordinates;

	vec2 distortionCoordinates1 = texture(dudvTexture, vec2(distortionCoordinates.x + distortionOffset, distortionCoordinates.y)).rg * 0.1;
	distortionCoordinates1 = distortionCoordinates + vec2(distortionCoordinates1.x, distortionCoordinates1.y + distortionOffset);
	vec2 totalDistortion = (texture(dudvTexture, distortionCoordinates1).rg * 2.0 - 1.0) * distortionStrength;

	reflectionCoordinates.x = clamp(reflectionCoordinates.x + totalDistortion.x, 0.001, 0.999);
	reflectionCoordinates.y = clamp(reflectionCoordinates.y + totalDistortion.y, -0.999, -0.001);
	refractionCoordinates = clamp(refractionCoordinates + totalDistortion, 0.001, 0.999);

	vec4 reflectionColor = texture(reflectionTexture, reflectionCoordinates);
	vec4 refractionColor = texture(refractionTexture, refractionCoordinates);

	float refractiveFactor = dot(normalize(toCameraVector), vec3(0.0f, 1.0f, 0.0f));

	vec4 normalColor = texture(normalTexture, distortionCoordinates1);
	vec3 normal = normalize(vec3(normalColor.r * 2.0f - 1.0f, normalColor.b, normalColor.g * 2.0f - 1.0f));
	vec3 specularHighlight = calculatePointLightSpecular(normal);

	color = mix(reflectionColor, refractionColor, refractiveFactor);
	color = mix(color, vec4(waterColor, 1.0f), 0.3) + vec4(specularHighlight, 1.0f);
}

vec3 calculatePointLightSpecular(vec3 normal) {
	vec3 reflectedLight = reflect(-normalize(toLightVector), normal);
	float specular = pow(max(dot(toCameraVector, reflectedLight), 0.0f), 0.6);
	vec3 specularColor = lightColor * (specular * 0.01f);

	return specularColor;
}