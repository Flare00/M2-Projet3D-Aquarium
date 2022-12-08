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

out vec3 Normal;
out vec2 TexCoord;
out vec4 PointCoord;
out vec3 PhysicsNorm;
out flat int IsDataPhysics;

const float displacementFactor = 0.1f;
const float displacementBias = 0.0f;

void main(){
	IsDataPhysics = u_is_data_physics;
	float height = texture(m_heightmap, aTexCoord).r;;

	if(u_is_data_physics == 1 && aNormal.y == 1){
		vec4 d = texture(p_data_physics, aTexCoord);
		height += -d.r;

		//Compute Normals
		vec3 dxN = vec3(d.b, texture(p_data_physics, vec2(aTexCoord.x + d.b, aTexCoord.y)).r - d.r, 0.0);
		vec3 dyN = vec3(0.0, texture(p_data_physics, vec2(aTexCoord.x, aTexCoord.y + d.b)).r - d.r, d.b);
		PhysicsNorm = normalize(cross(dyN, dxN));
	}

	PointCoord = u_model * vec4(aPos.x, aPos.y + height, aPos.z, 1.0f);
	Normal = mat3(u_model) * aNormal;
	TexCoord = aTexCoord;
	gl_Position =  u_projection * (u_view * PointCoord);
}