#version 450 core

out vec4 color;

void main(){

	//color = vec4(1.0,0,0,1.0);
	color = vec4(vec3(gl_FragCoord.z), 1.0);
}