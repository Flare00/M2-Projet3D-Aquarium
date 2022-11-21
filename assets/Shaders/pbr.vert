#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform sampler2D m_heightmap;

out vec3 Normal;
out vec2 TexCoord;
out vec4 PointCoord;


void main(){
	vec4 p = (u_view * u_model) * vec4(aPos,1.0f);
	PointCoord = u_model * vec4(aPos,1.0f);
	Normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);
	TexCoord = aTexCoord;

	gl_Position =  u_projection * p;
}