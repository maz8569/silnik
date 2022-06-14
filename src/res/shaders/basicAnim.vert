#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout(location = 5) in ivec4 aBoneIds; 
layout(location = 6) in vec4 aWeights;

layout (location = 0) out vec4 ourColor;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 Normal;
layout (location = 3) out vec3 FragPos;
layout (location = 4) out vec4 FragPosLightSpace;
layout (location = 5) out vec4 screenPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 aColor;
uniform vec3 cameraPos;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

float amount = 0.005f;

void main()
{
    ourColor = aColor;
    TexCoord = aTexCoord;

    vec4 totalPosition = vec4(0.0f);
    /*
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(aBoneIds[i] == -1) 
            continue;
        if(aBoneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[aBoneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * aWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[aBoneIds[i]]) * Normal;
    }
    */

    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    worldPos -= cameraPos;
    FragPos = worldPos;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    worldPos = vec3(view * vec4(worldPos, 1.0));
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec3(0, ypos, 0);
    screenPosition = projection * vec4(worldPos, 1.0);
    gl_Position = screenPosition;
}