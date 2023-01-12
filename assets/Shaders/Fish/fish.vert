#version 430
#extension GL_OES_standard_derivatives : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aOffset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 Normal;
out vec2 TexCoord;
out vec4 PointCoord;

void main(){
	PointCoord = u_model * vec4(aPos + aOffset, 1.0f);
	Normal = mat3(u_model) * aNormal;
	TexCoord = aTexCoord;
	gl_Position =  u_projection * (u_view * PointCoord);
}