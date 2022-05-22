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
uniform sampler2D foamTexture;
 
uniform vec3 viewPos; 
uniform vec3 color; 
uniform float shininess;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform float uTime;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//float shadow = 0;

float near_plane = 0.1f;
float far_plane = 100.0f;

vec3 foamcolor = vec3(1, 1, 1);

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
    //if(pointLight.ambient != vec3(0, 0, 0))
    //    result += CalcPointLight(pointLight, norm, FragPos, viewDir);    
    // phase 3: spot light
    FragColor =  vec4(result, 0.5);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //vec3 lightDir = normalize(light.direction + FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if      (diff >= 0.8) { diff = 1.0; }
    else if (diff >= 0.6) { diff = 0.6; }
    else if (diff >= 0.3) { diff = 0.3; }
    else                              { diff = 0.0; }
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        spec = step(0.98, spec);

    // combine results

    vec3 textureee = vec3(texture(ourTexture, TexCoord  * 10));

  vec3 color2 = texture2D( ourTexture,
  TexCoord * 10.0 +
  0.8*vec2(
    cos(uTime*0.001*0.1),
    sin(uTime*0.001*0.1)
  ) +
  0.01*vec2(
    cos(1.7 + uTime*0.0012+3.2*100.0*TexCoord.x),
    sin(1.7 + uTime*0.001+3.0*100.0*TexCoord.y)
  )
).rgb;

    vec3 finalColor = mix(color * clamp(1.0 - color2, 0.9, 1.0), foamcolor, textureee );

    float depthValue = (texture(foamTexture, TexCoord).r) / far_plane;   

    vec3 scolor = vec3(LinearizeDepth(depthValue) );

    vec3 ambient = light.ambient * finalColor * vec3(1.0, 1.0, 1.2);
    vec3 diffuse = light.diffuse * diff * finalColor;
    vec3 specular = light.specular * spec*0.5;
    //return (ambient + (diffuse + specular));

    return (ambient + (diffuse + specular));
}