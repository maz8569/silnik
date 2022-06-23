#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;
uniform float uTime;
uniform float gamma;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    vec2 pos = TexCoords;

    //int   size       = 5;
    //float separation = 3;
    //float threshold  = 0.8;
    //float amount     = 0.8;

    /*
    vec2 texSize = textureSize(depthMap, 0).xy;

    float value = 0.0;
    float count = 0.0;

    vec4 result = vec4(0);
    vec4 Scolor  = vec4(0);
    
    for (int i = -size; i <= size; ++i) {
        for (int j = -size; j <= size; ++j) {
               Scolor =
            texture
            ( depthMap
            ,   ( gl_FragCoord.xy
                 + (vec2(i, j) * separation)
                 )
                / texSize
            );

        value = max(Scolor.r, max(Scolor.g, Scolor.b));
         if (value < threshold) { Scolor = vec4(0); }

        result += Scolor;
        count  += 1.0;
        }
    }
    result /= count;
   */
    /*
    float X = 3*(pos.x*15.+uTime*0.5);
    float Y = 3*( pos.y*15.+uTime*0.5);
    pos.y += cos(X+Y)*0.01*cos(Y);
    pos.x += sin(X-Y)*0.01*sin(Y);
   */
   vec3 color = vec3( texture(depthMap, pos));

   color = pow(color, vec3(1.0/ 1.5));
   //vec4 acolor = mix(vec4(0), result, amount);
  // color += acolor.rgb;
    //float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(color, 1.0);
    //FragColor = acolor;
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}