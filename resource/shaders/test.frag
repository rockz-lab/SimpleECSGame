#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform float spriteColor[3];

void main()
{    
    color = (vec4(spriteColor[0], spriteColor[1], spriteColor[2], 0.0) * 0.000001) + texture(image, texCoords);
//    color = vec4(10, 0, 0, 1);
}  