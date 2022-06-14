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
layout (location = 5) in vec4 screenPosition;
layout (location = 6) in vec3 WorldPosition;

uniform sampler2D ourTexture;
uniform sampler2D foamTexture;
uniform sampler2D refractTexture;

uniform sampler2D noiseTexture;
 
uniform vec3 viewPos; 
uniform vec3 color; 
uniform float shininess;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform float uTime;
uniform vec4 camera_params;
uniform vec4 uScreenSize;
uniform mat4 view;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//float shadow = 0;

float near_plane = 0.1f;
float far_plane = 100.0f;

vec3 foamcolor = vec3(1, 1, 1);
float dinstance = 1;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}
float linearizeDepth(float z) {
        z = z * 2.0 - 1.0;
        return 1.0 / (camera_params.z * z + camera_params.w);
}

float getLinearDepth(vec3 pos) {
    return -(view * vec4(pos, 1.0)).z;
}

float getLinearScreenDepth(vec2 uv) {
    return LinearizeDepth(texture2D(foamTexture, uv).r) * camera_params.y;
}

float getLinearScreenDepth() {
    vec2 uv = gl_FragCoord.xy * uScreenSize.zw;
    return getLinearScreenDepth(uv);
}

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result = CalcDirLight(dirLight, norm, viewDir);
    //if(pointLight.ambient != vec3(0, 0, 0))
    //    result += CalcPointLight(pointLight, norm, FragPos, viewDir);    
    // phase 3: spot light
    //vec4 ccc = vec4(screenPosition.x, screenPosition.y, screenPosition.z, 1.0);
    //FragColor = screenPosition;
    //result.rgb = pow(result.rgb, vec3(1.0 / 1.5));
    FragColor =  result;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //vec3 lightDir = normalize(light.direction + FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if      (diff >= 0.8) { diff = 1.0; }
    else if (diff >= 0.6) { diff = 0.6; }
    else if (diff >= 0.3) { diff = 0.3; }
    else                  { diff = 0.0; }
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        spec = step(0.98, spec);

    // combine results

    vec2 foamTexturecoords = TexCoord;

    vec3 textureee = vec3(texture(ourTexture, foamTexturecoords * 10));

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

    vec3 finalColor = mix(color * clamp(1.0 - color2, 0.85, 1.0), foamcolor, textureee );


    vec2 ndc = (screenPosition.xy/screenPosition.w)/2.0 + 0.5;
    vec2 texcoordss = vec2(ndc.x, ndc.y);

    vec2 texxtcoords = vec2(TexCoord.x, 1-TexCoord.y);

    float depthValue = texture(foamTexture, texcoordss).r;   
    depthValue = 60* LinearizeDepth(depthValue) / far_plane;

    //texcoordss.y +=0.03;

    float refValue = texture(foamTexture, texcoordss).r;
    refValue = 60* LinearizeDepth(refValue) / far_plane;

    float worldDepth = getLinearDepth(WorldPosition);
    float screenDepth = getLinearScreenDepth();

    vec3 ambient = light.ambient * finalColor * vec3(1.0, 1.0, 1.2);
    vec3 diffuse = light.diffuse * diff * finalColor;
    vec3 specular = light.specular * spec*0.5;

    vec2 pos = texcoordss;

    float X = 3*(pos.x*15.+uTime*0.5);
    float Y = 3*( pos.y*15.+uTime*0.5);
    pos.y += cos(X+Y)*0.01*cos(Y);
    pos.x += sin(X-Y)*0.01*sin(Y);

    vec4 refrColor = texture(refractTexture, pos);
    vec4 jColor = texture(refractTexture, texcoordss);

    vec4 gg = vec4(ambient + (diffuse + specular), 1.0);

    float foamLine = clamp( (-screenPosition.w/2 + depthValue)/10, 0, 1);
    float refrline = clamp( (-screenPosition.w/2 + refValue)/20, 0, 1);
    vec3 scolor = vec3(refrline);

    if (refrline < 0.5)
    {
        gg = mix(gg, refrColor , 0.5);

    }
    else{
        gg = mix(gg, jColor , 0.5);// * vec4(0.8, 0.8, 1, 1);

    }

    if(foamLine < 0.3){
        gg.rgb = vec3(0.8, 0.8, 0.75);
        //gg.a += 0.1;
    }

    //return (ambient + (diffuse + specular));
    vec4 ggcolor = vec4(scolor,1.0);

    //return (ambient + (diffuse + specular));
    //return ggcolor;
    return gg;
}