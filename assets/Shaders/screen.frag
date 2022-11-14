#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;
uniform sampler2D depthTexture;

void main()
{
    vec3 col = texture(renderTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 