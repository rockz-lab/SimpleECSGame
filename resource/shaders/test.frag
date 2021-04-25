#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform float spriteColor[3];

void main()
{    
    color = vec4(spriteColor[0], spriteColor[1], spriteColor[2], 1.0)*0.001 +  1000*texture(image, texCoords);
//    color = vec4(10, 0, 0, 1);
}  