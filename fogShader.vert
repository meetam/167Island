#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoords;
out vec4 viewSpace;
out vec3 normalColor;

void main(){
    
    //Used for lighting models
    world_pos = (model * vec4(position, 1.0f)).xyz;
    world_normal = normalize(mat3(model) * normal);
    texcoords = texcoord;
    
    //Send to fragment shader
    viewSpace = view * model * vec4(position, 1.0f);
    gl_Position = projection * viewSpace;
    
    //Normal color
    //normalColor = normalize(normal) * 0.5 + 0.5;
    normalColor = color;
}
