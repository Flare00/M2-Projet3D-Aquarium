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
} 