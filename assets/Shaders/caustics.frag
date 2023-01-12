#version 430

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec4 gNormal;
layout (location = 3) out vec4 gMetalnness;

uniform sampler2D waterData;
uniform vec3 lightDirection;

in vec2 coord;

void main(){
    vec4 d = texture(waterData, coord);
    vec3 normal = vec3(d.b, sqrt(1.0 - dot(d.ba, d.ba)), d.a);
    vec3 reflection = reflect(lightDirection, normal);
    float caustic = pow(clamp(dot(reflection, vec3(0, 0, 1)), 0.0, 1.0), 2.0);

    gAlbedo = vec4(vec3(1,1,1) * caustic, 1.0f);
}