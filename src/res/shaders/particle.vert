#version 460 core

layout (location = 0) in vec4 aPos;
//layout (location = 1) in vec2 aTexCoord;
layout (location = 1) in vec3 aInstancedPos;
layout (location = 2) in vec4 aColor;

layout (location = 0) out vec4 ourColor;
layout (location = 1) out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

float amount = 0.005f;

void main()
{
    ourColor = aColor;
    TexCoord = aPos.zw;
    vec4 position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    vec3 worldPos = vec3(model * position);
    worldPos -= cameraPos;
    worldPos = vec3(view * vec4(worldPos, 1.0));
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec3(0, ypos, 0);
    gl_Position =  projection * vec4(worldPos, 1.0);
}