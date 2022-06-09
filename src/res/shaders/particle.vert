#version 460 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
layout (location = 1) in vec4 aInstancedPos;
layout (location = 2) in vec4 aColor;

layout (location = 0) out vec4 ourColor;
layout (location = 1) out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

float amount = 0.005f;

void main()
{
    
    ourColor = aColor;
    TexCoord = aPos.xy + vec2(0.5, 0.5);

    float particleSize = aInstancedPos.w;
    vec3 particleCenter = aInstancedPos.xyz;
    vec3 worldPos = particleCenter;
    /*
    vec3 worldPos = worldPos - cameraPos;
    worldPos = vec3(view * vec4(worldPos, 1.0));
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec3(0, ypos, 0);
    */

    vec3 position = worldPos 
    + CameraRight_worldspace * aPos.x * particleSize
    + CameraUp_worldspace * aPos.y * particleSize;


    gl_Position =  projection * view * vec4(position, 1.0);
    //gl_Position =  projection * vec4(position, 1.0);
    //gl_Position =  projection * vec4(worldPos, 1.0);
}