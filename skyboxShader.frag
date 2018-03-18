#version 330 core

in vec3 textureCoordinates;

uniform samplerCube skybox;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    color = texture(skybox, textureCoordinates);
}
