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

	vec4 albedo;
	float metallic;
	float roughness;
};

uniform LightInfo[MAX_LIGHTS] lights;
uniform Material material;

float computeLight(LightInfo light, vec3 point, vec3 normal){
	
	float pow = lights[0].power / (distance(light.pos, point) * lights[0].attenuation);
	float sens = max(dot(normal, normalize(light.pos - point)), 0.0);

	return pow * (sens > 0 ? 1.0 : 0.0);
}

void main(){
	vec4 albedoM = texture(material.albedoMap, TexCoord);
	vec4 normalM = texture(material.normalMap, TexCoord);
	vec4 metallicM = texture(material.metallicMap, TexCoord);
	vec4 roughnessM = texture(material.roughnessMap, TexCoord);
	vec4 aoM = texture(material.aoMap, TexCoord);

	color =  albedoM * material.albedo;

	vec3 normal = Normal;
	if(normalM.w == 1){
		normal = normalM.xyz;
	}

	/*if(MAX_LIGHTS-1 > 0)
		color = color * computeLight(lights[0], PointCoord.xyz, normal);*/
	
}