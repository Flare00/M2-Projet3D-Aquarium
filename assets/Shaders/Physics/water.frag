#version 430

const float PI = 3.141592;

out vec4 color;

in vec2 coord;

uniform sampler2D tex;

uniform float deltaTime;
uniform vec2 deltaMove;

void main(){
    vec4 data = texture(tex, coord);


    vec2 dx = vec2(deltaMove.x, 0.0);
    vec2 dy = vec2(0.0, deltaMove.y);


    float average = (
        texture(tex, coord - dx).r +
        texture(tex, coord - dy).r +
        texture(tex, coord + dx).r +
        texture(tex, coord + dy).r
    ) * 0.25;

    // change velocity to average
    data.g += (average - data.r) * 2.0;
    
    // attenuate the velocity a little so waves do not last forever 
    data.g *= 1.0 - (deltaTime * 0.1);
    
    // move the vertex along the velocity 
    data.r += data.g;

    vec3 ndx = vec3(deltaMove.x, texture(tex, vec2(coord.x + deltaMove.x, coord.y)).r - data.r, 0.0);
    vec3 ndy = vec3(0.0, texture(tex, vec2(coord.x, coord.y + deltaMove.y)).r - data.r, deltaMove.y);
    data.ba = normalize(cross(ndy, ndx)).xz;

    color = data;
}

