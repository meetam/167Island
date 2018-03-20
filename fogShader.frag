/* Tutorial Used: http://in2gpu.com/2014/07/22/create-fog-shader/ */
#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;

uniform sampler2D texture1;

const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor = vec3(0.2, 0.2, 0.2);

//from vertex shader
in vec3 world_pos;
in vec3 world_normal;
in vec4 viewSpace;
in vec2 texcoords;
in vec3 normalColor;

const vec3 fogColor = vec3(0.5, 0.5,0.5);
const float FogDensity = 0.05;

void main(){
    
    vec3 tex1 = texture(texture1, texcoords).rgb;
    
    //get light an view directions
    vec3 L = normalize(light_position - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    
    //diffuse lighting
    vec3 diffuse = DiffuseLight * max(0, dot(L,world_normal));
    
    //rim lighting
    float rim = 1 - max(dot(V, world_normal), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    vec3 finalRim = RimColor * vec3(rim, rim, rim);
    //get all lights and texture
    vec3 lightColor = normalColor; //finalRim + diffuse + tex1 + normalColor;
    
    vec3 finalColor = vec3(0, 0, 0);
    
    //distance
    float dist = 0;
    float fogFactor = 0;
    
    //plane based distance
    dist = abs(viewSpace.z);

    //range based distance
    //dist = length(viewSpace);
    
    float fogStart = 100;
    float fogEnd = 1000;
    fogFactor = (fogEnd - dist)/(fogEnd - fogStart);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
    finalColor = mix(fogColor, lightColor, fogFactor);
    out_color = vec4(finalColor, 1);
}
