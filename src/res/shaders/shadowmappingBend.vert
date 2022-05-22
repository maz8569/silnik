#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform mat4 model;

float amount = 0.005f;

void main()
{
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));

    worldPos = vec3(view * vec4(worldPos, 1.0));
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec3(0, ypos, 0);

    gl_Position =  projection * vec4(worldPos, 1.0);
}