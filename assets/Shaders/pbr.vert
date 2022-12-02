#version 430
#extension GL_OES_standard_derivatives : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform sampler2D m_heightmap;
uniform int u_is_heightmap;

out vec3 Normal;
out vec2 TexCoord;
out vec4 PointCoord;

const float displacementFactor = 0.1f;
const float displacementBias = 0.0f;

void main(){
	float height = 0.0f;
	if(u_is_heightmap == 1){
		height = -texture(m_heightmap, aTexCoord).r;
	}
	PointCoord = u_model * vec4(aPos.x, aPos.y + height, aPos.z, 1.0f);
	Normal = mat3(u_model) * aNormal;
	TexCoord = aTexCoord;
	gl_Position =  u_projection * (u_view * PointCoord);
}