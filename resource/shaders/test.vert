#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat3 projection;
uniform mat3 model;

out vec2 texCoords;

void main()
{
    vec3 transformed = projection * model * vec3(position, 1.0);
//    vec3 transformed = projection * vec3(position, 1.0);
    texCoords = texCoord;
    gl_Position = vec4( transformed.xy, 0.0, 1.0);
}