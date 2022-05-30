#version 460 core

layout (location = 0) out vec4 FragColor;
  
layout (location = 0) in vec4 ourColor;
layout (location = 1) in vec2 TexCoord; 

uniform sampler2D ourTexture;
 
uniform vec3 viewPos; 

void main()
{
    vec4 result = texture(ourTexture, TexCoord);

    if(result.a < 0.4)
    {
        discard;
    }

    result *= ourColor;

    FragColor =  result; 
}