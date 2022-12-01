#version 430

const float PI = 3.141592;

struct Drop{
    vec2 center;
    float radius;
    float strength;
};

in vec2 coord;

uniform sampler2D tex;

uniform float deltaTime;
uniform vec2 deltaMove;

uniform Drop drop;
uniform int is_drop;

void main(){
    vec4 data = texture(tex, coord);

    data.a = 1.0f;
    data.r *= 1.0 - (deltaTime/2.0);
    //Add drop
    
    if (is_drop == 0){
        data.b = 1.0f;
    } else {
        float d = max(0.0 , 1.0 - length(drop.center - coord) / drop.radius);
        d = 0.5 - (0.5 * cos(d * PI));
        data.r = d * drop.strength;
    } 


    gl_FragColor = data;
    /*

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
    //data.g *= 0.995;
    
    // move the vertex along the velocity 
    data.r += data.g;*/

}

