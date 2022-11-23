#version 430
#define MAX_LIGHTS 1

// --- IN / OUT ---

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec4 PointCoord;

// --- Structs ---

struct LightInfo
{
	vec3 pos;
	vec3 dir;
	vec3 color;
	float power;
	bool directional;
};

struct Material
{
	sampler2D albedoMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D normalMap;
	sampler2D aoMap;

	vec4 albedo;
	float metallic;
	float roughness;
};

// --- Global Variables ---

uniform LightInfo[MAX_LIGHTS] lights;
uniform Material material;
uniform vec3 u_cameraPos;

const int u_show_mode = 0; // 0 = PBR, 1 = Albedo, 2 = Albedo Map, 3 = Normal, 4 = Metallic, 5 = Roughness, 6 = Ao

const float PI = 3.14159265359;
const float gamma = 2.2;



// --- PBR Functions --- 
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normalMap, TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(PointCoord.xyz);
    vec3 Q2  = dFdy(PointCoord.xyz);
    vec2 st1 = dFdx(TexCoord);
    vec2 st2 = dFdy(TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
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
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


// --- Main ---

void main(){
	// Get textures infos
	vec4 albedoM = texture(material.albedoMap, TexCoord);
	vec4 metallicM = texture(material.metallicMap, TexCoord);
	vec4 roughnessM = texture(material.roughnessMap, TexCoord);
	vec4 aoM = texture(material.aoMap, TexCoord);

	// Set variables
	vec4 Albedo = albedoM * material.albedo;
	vec3 Norm = getNormalFromMap();
	float Metal = (metallicM.a == 0) ? material.metallic : metallicM.r;
	float Roughness = (roughnessM.a == 0) ? material.roughness : roughnessM.r;
	float Ao = (aoM.a == 0) ? 1.0f : aoM.r;

	FragColor = material.albedo;


	vec3 V = normalize(u_cameraPos - PointCoord.xyz);
	// Reflectance at normal incidence, 0.04 for plastic, else albedo
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Albedo.xyz, Metal);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for( int i = 0 ; i < MAX_LIGHTS - 1; i++){
		//per light radiance
		vec3 L = normalize(lights[0].pos - PointCoord.xyz);
		vec3 H = normalize(V + L);
		float dist = length(lights[0].pos - PointCoord.xyz);
		float attenuation = lights[0].power /  (dist);
		vec3 radiance = lights[0].color * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(Norm,H, Roughness);
		float Geom	= GeometrySmith(Norm, V, L, Roughness);
		vec3 Fresnel = FresnelSchlick(max(dot(H, V), 0.0), F0);

		float NdotL = max(dot(Norm, L), 0.0);

		vec3 numerator = NDF * Geom * Fresnel;
		float denominator = 4.0 * max(dot(Norm, V), 0.0) * NdotL + 0.0001;
		vec3 spec = numerator/denominator;

		vec3 kD = vec3(1.0) - Fresnel;
		kD *= 1.0 - Metal;
		
		Lo += ((kD * Albedo.rgb / PI) + spec) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	vec3 ambient = vec3(0.001) * Albedo.rgb; //* Ao;

	vec3 c = ambient + Lo;

	c = c / (c+vec3(1.0));
	c = pow(c, vec3(1.0/gamma));
	FragColor = vec4(c,1.0f);
	
		
	//FragColor = material.albedo;
	
}