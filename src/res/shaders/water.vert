#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout (location = 0) out vec4 ourColor;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 Normal;
layout (location = 3) out vec3 FragPos;
layout (location = 4) out vec4 FragPosLightSpace;
layout (location = 5) out vec4 screenPosition;
layout (location = 6) out vec3 WorldPosition;
layout(location = 7) out float aTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 aColor;
uniform vec3 cameraPos;
uniform mat4 lightSpaceMatrix;
uniform float uTime;

uniform vec4 uScreenSize;

uniform sampler2D noiseTexture;

uniform float amount;

void main()
{
    ourColor = aColor;
    vec3 pos = aPos;

    vec2 texcoor = aTexCoord;

    float distortion = 0;

    distortion = cos(pos.z * 5.0 + uTime) * 0.1 * sin(pos.x * 5.0 + uTime);

    //texcoor.x -= uTime * 0.001;
    //texcoor.y -= uTime * 0.001;

    //distortion = 0.1  * texture2D(noiseTexture, texcoor * 100).r;
    aTime = uTime;

    pos.y +=  distortion;

    TexCoord = aTexCoord + vec2(distortion * 0.1, distortion * 0.1);

    vec3 worldPos = vec3(model * vec4(pos, 1.0));

    worldPos -= cameraPos;
    FragPos = worldPos;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    worldPos = vec3(view * vec4(worldPos, 1.0));
    float ypos = (pow(worldPos.z, 2) * 1.5 + pow(worldPos.x, 2) )* (-1) * amount ;
    worldPos += vec3(0, ypos, 0);
    WorldPosition = worldPos;
    screenPosition = projection * vec4(worldPos, 1.0);
    gl_Position = screenPosition;
}