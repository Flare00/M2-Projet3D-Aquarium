#version 450 core
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

uniform LightInfo[MAX_LIGHTS] lights;


void main(){
	
	color = vec4(1.0,0,0,1.0);
	/*if(MAX_LIGHTS-1 > 0){
		color = vec4(lights[0].color, 1);
	}*/
	// color = vec4(vec3(gl_FragCoord.z), 1.0);
}