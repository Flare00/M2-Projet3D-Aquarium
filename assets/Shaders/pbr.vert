#version 430
#extension GL_OES_standard_derivatives : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform sampler2D m_heightmap;

uniform sampler2D p_data_physics;
uniform int u_is_data_physics;

out mat4 Proj;
out vec3 Normal;
out vec2 TexCoord;
out vec2 ScreenTexCoord;
out vec4 PointCoord;
out vec3 PhysicsNorm;
out flat int IsDataPhysics;
out float fogDistance;

const float displacementFactor = 0.1f;
const float displacementBias = 0.0f;

void main(){
	IsDataPhysics = u_is_data_physics;
	float height = texture(m_heightmap, aTexCoord).r;;

	if(u_is_data_physics == 1 && aNormal.y == 1){
		vec4 d = texture(p_data_physics, aTexCoord);
		height += d.r;
		PhysicsNorm = vec3(d.b, sqrt(1.0 - dot(d.ba, d.ba)), d.a);
	}

	PointCoord = u_model * vec4(aPos.x, aPos.y + height, aPos.z, 1.0f);
	Normal = mat3(u_model) * aNormal;
	TexCoord = aTexCoord;
	ScreenTexCoord = aPos.xy;
	gl_Position =  u_projection * (u_view * PointCoord);
	Proj = u_projection;
	fogDistance = length(gl_Position.xyz);
}