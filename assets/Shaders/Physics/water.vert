#version 430

layout(location = 0) in vec3 aPos;
uniform vec2 halfSize;
out vec2 coord;

void main(){
	coord = (aPos.xy / halfSize);
	gl_Position =  vec4(aPos,1);
}