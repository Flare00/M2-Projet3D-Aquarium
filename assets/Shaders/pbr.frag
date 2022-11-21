#version 430
#define MAX_LIGHTS 1

out vec4 color;

in vec3 Normal;
in vec2 TexCoord;
in vec4 PointCoord;

const float PI = 3.14159265359;

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
uniform vec3 cameraPos;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float gamma = 2.2f;

float computeLight(LightInfo light, vec3 point, vec3 normal){
	vec3 lightDir = normalize(light.pos - point);
	
	float dist = distance(light.pos, point);
	float diff = max(dot(lightDir, normal), 0.0);
	float power = light.power / (pow(dist,gamma)  * light.attenuation);

	return power * diff;
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

	normal = normalize((normal * 2.0) - 1.0);

	if(MAX_LIGHTS-1 > 0)
		color = color* computeLight(lights[0], PointCoord.xyz, normal);
	//color = vec4(normal,1.0);
}