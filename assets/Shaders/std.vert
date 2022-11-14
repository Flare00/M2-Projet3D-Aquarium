#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 Normal;
out vec2 TexCoord;

void main(){

	Normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);;
	TexCoord = aTexCoord;

	gl_Position =  u_projection * (u_view * (u_model* vec4(aPos,1.0f)));;
}