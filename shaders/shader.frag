#version 460 core
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 normal;


layout(push_constant) uniform PushConstants {
    mat4 render_matrix; 
    vec3 lightPos;
    vec3 lightColor;
    vec3 camPos;
} constants;

layout(location = 0) out vec4 outColor;
void main() {

    float x = dot( normal, constants.lightPos);
    vec4 baseColor = vec4(1.0f)*0.1f+x*0.9f;
    
    outColor = baseColor; 
}