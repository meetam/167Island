#version 330 core

in vec2 texturePosition;
in float height;

uniform float waterHeight;
uniform float maxHeight;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 color;

void main()
{
	float tilingConst = 5.0f;			// higher = more texture tiles
	float texture1Cutoff = 0.0f;		// higher = higher level for terrain to appear
	float mixTexture1Strength = 5.0f;	// higher = more aggressive mixing
	float texture2Cutoff = 0.2f;
	float mixTexture2Strength = 7.0f;

	vec2 textureCoordinates = texturePosition * tilingConst; 
	float heightAboveWaterPercentage = (height - waterHeight) / (maxHeight - waterHeight);
		
	color = texture(texture0, textureCoordinates);

	float texture1MixPercentage = clamp(((clamp(heightAboveWaterPercentage, texture1Cutoff, 1.0f) - texture1Cutoff) * mixTexture1Strength), 0.0f, 1.0f);
	color = mix(color, texture(texture1, textureCoordinates), texture1MixPercentage);

	float texture2MixPercentage = clamp(((clamp(heightAboveWaterPercentage, texture2Cutoff, 1.0f) - texture2Cutoff) * mixTexture2Strength), 0.0f, 1.0f);
	color = mix(color, texture(texture2, textureCoordinates), texture2MixPercentage);
}