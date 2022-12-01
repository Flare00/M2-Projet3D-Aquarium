#version 430

layout(location = 0) in vec3 aPos;
uniform vec2 halfSize;
out vec2 coord;

void main(){
	coord = (aPos.xz+vec2(1)) / 2.0;
	gl_Position =  vec4(aPos.x, aPos.z, aPos.y, 1.0); 
}