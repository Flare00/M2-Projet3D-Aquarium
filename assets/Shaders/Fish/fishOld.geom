#version 430
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

in vec3 V_Normal[];
in vec2 V_TexCoord[];
in vec4 V_PointCoord[];

out vec3 Normal;
out vec2 TexCoord;
out vec4 PointCoord;

const float offset = 0.5f;
void main(){

    vec4 pos = gl_in[0].gl_Position;
    vec4 p1 = vec4(pos.x - offset, pos.y - offset, pos.z, 1.0f);
    vec4 p2 = vec4(pos.x - offset, pos.y + offset, pos.z, 1.0f);
    vec4 p3 = vec4(pos.x + offset, pos.y - offset, pos.z, 1.0f);

    //triangle 1
    Normal = V_Normal[0];
    TexCoord = V_TexCoord[0];
    PointCoord = p1;
    gl_Position = p1;
    EmitVertex();

    Normal = V_Normal[0];
    TexCoord = V_TexCoord[0];
    PointCoord = p2;
    gl_Position = p2;
    EmitVertex();

    Normal = V_Normal[0];
    TexCoord = V_TexCoord[0];
    PointCoord = p3;
    gl_Position = p3;
    EmitVertex();
    EndPrimitive();
}