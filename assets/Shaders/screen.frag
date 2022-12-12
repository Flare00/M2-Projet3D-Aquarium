#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;
uniform sampler2D depthTexture;
//uniform sampler2D stencilTexture;

void main()
{
    vec4 col = texture(renderTexture, TexCoords).rgba;
    FragColor = col;
    if(col.r > 1.0){
        FragColor = vec4(0,1,0,1);
    } else if (col.r > 2.0){
        FragColor = vec4(0,0,1,1);
    }
} 