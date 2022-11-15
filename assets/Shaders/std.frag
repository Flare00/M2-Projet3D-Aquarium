#version 430
#define MAX_LIGHTS 1

out vec4 color;

in vec3 Normal;
in vec2 TexCoord;

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
	vec3 color;
};

uniform LightInfo[MAX_LIGHTS] lights;
uniform Material material;

void main(){
	
	color = vec4(material.color, 1.0);
	/*if(MAX_LIGHTS-1 > 0){
		color = vec4(lights[0].color, 1);
	}*/
	// color = vec4(vec3(gl_FragCoord.z), 1.0);
}