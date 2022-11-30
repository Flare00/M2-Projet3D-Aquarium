#version 430

#define PI 3.141592653589793;

struct Drop{
    vec2 center;
    float radius;
    float strength;
    int isDrop;
};

in vec2 coord;

uniform sampler2D texture;

uniform float deltaTime;
uniform vec2 deltaMove;

uniform Drop drop;

void main(){
    vec4 data = texture(texture, coord);

    //Add drop
    if(drop.isDrop == 1){

        float d = max(0.0 , 1.0 - length(drop.center - coord) / drop.radius);

        d = 0.5 - (cos(d * PI) * 0.5);
        data.r += d * drop.strength;
    }

    vec2 dx = vec2(deltaMove.x, 0.0);
    vec2 dy = vec2(0.0, deltaMove.y);

    float average = (
        texture2D(texture, coord - dx).r +
        texture2D(texture, coord - dy).r +
        texture2D(texture, coord + dx).r +
        texture2D(texture, coord + dy).r
    ) * 0.25;

    // change velocity to average
    data.g += (average - data.r) * 2.0;
    
    // attenuate the velocity a little so waves do not last forever 
    data.g *= 0.995;
    
    // move the vertex along the velocity 
    data.r += data.g;

    gl_FragColor = data;
}

