#version 430
#define MAX_LIGHTS 1

out vec4 color;

in vec3 Normal;
in vec2 TexCoord;
in vec4 PointCoord;

struct LightInfo
{
	vec3 pos;
	vec3 dir;
	vec3 color;
	float power;
	float attenuation;
	bool directional;
};

struct Material
{
	sampler2D albedoMap;
	sampler2D normalMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D aoMap;

	vec3 albedo;
	float metallic;
	float roughness;
};

uniform LightInfo[MAX_LIGHTS] lights;
uniform Material material;

float computeLight(LightInfo light, vec3 point){
	
	float pow = lights[0].power / (distance(light.pos, point) * lights[0].attenuation);
	float sens = max(dot(Normal, normalize(light.pos - point)), 0.0);

	return pow * (sens > 0 ? 1.0 : 0.0);
}

void main(){
	
	color = vec4(material.color, 1.0);
	if(MAX_LIGHTS-1 > 0){
		//color = vec4(lights[0].color, 1);

		color = color * computeLight(lights[0], PointCoord.xyz);
	}
	// color = vec4(vec3(gl_FragCoord.z), 1.0);
}