#version 460 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_SPOT_LIGHTS 2

layout (location = 0) out vec4 FragColor;
  
layout (location = 0) in vec4 ourColor;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 FragPos;
layout (location = 4) in vec4 FragPosLightSpace;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform sampler2D cameraDepthMap;
 
uniform vec3 viewPos; 
uniform vec3 color; 
uniform float shininess;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float ShadowCalculation(vec3 lightDir, vec3 normal, vec4 fragPosLightSpace);

float near_plane = 0.1f;
float far_plane = 100.0f;

int size = 1;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    result *= color; 
    FragColor =  vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{

    float depthValue = texture(cameraDepthMap, TexCoord).r;   

    depthValue = LinearizeDepth(depthValue) / far_plane;

    vec3 color = vec3(depthValue);

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if      (diff >= 0.8) { diff = 1.0; }
    else if (diff >= 0.6) { diff = 0.6; }
    else if (diff >= 0.3) { diff = 0.3; }
    else                              { diff = 0.0; }
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        spec = step(0.98, spec);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(ourTexture, TexCoord)) * vec3(1.0, 1.0, 1.2);
    vec3 diffuse = light.diffuse * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular = light.specular * spec*0.5;
    //return (ambient + (diffuse + specular));
    float shadow = ShadowCalculation(lightDir, normal, FragPosLightSpace);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculation(vec3 lightDir, vec3 normal, vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0; 
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}