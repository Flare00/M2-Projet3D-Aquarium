#version 430

const float PI = 3.141592;
out vec4 color;

in vec2 coord;

uniform sampler2D tex;

uniform vec2 center;
uniform float radius;
uniform float strength;

void main(){
    vec4 data = texture(tex, coord);
    
    //Add the drop.
    float d = max(0.0 , 1.0 - length(center - coord) / radius);
    d = 0.5 - (0.5 * cos(d * PI));
    if(d > 0.01){
        data.r = d * strength;
    }
    color = data;
}

