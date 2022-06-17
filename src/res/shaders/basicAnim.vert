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

uniform float amount;

void main()
{
    ourColor = aColor;
    TexCoord = aTexCoord;

    const vec4 pos = vec4(aPos, 1.0f);
    const vec4 norm = vec4(aNormal, 0.0f);

    vec4 posSkinned = vec4(0.0f);
    vec4 normSkinned = vec4(0.0f);
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(aBoneIds[i] >= 0)
        {
            const mat4 bone = finalBonesMatrices[aBoneIds[i]];
            const float weight = aWeights[i];

            posSkinned += (bone * pos) * weight;
            normSkinned += (bone * norm) * weight;

        }
    }
    
    posSkinned.w = 1.0f;

    vec4 worldPos = model * posSkinned;
    //vec4 worldPos = model * pos;
    worldPos -= vec4(cameraPos, 0.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(model))) * vec3(normSkinned);
    //Normal = mat3(transpose(inverse(model))) * vec3(aNormal);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    worldPos = view * worldPos;
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec4(0, ypos, 0, 0);
    screenPosition = projection * worldPos;
    gl_Position = screenPosition;
}