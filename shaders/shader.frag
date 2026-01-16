#version 460 core
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 pos;

layout(push_constant) uniform PushConstants {
    mat4 render_matrix; 
    mat4 model;
    vec4 lightPos;
    vec4 lightColor;
    vec4 lightIntensity;
} constants;

layout(location = 0) out vec4 outColor;
void main() {
    vec3 normalized_normal = normalize(normal);
    vec3 lp = vec3(-constants.lightPos);
    float distance = length(lp +pos);
    float attenuation =  constants.lightIntensity.x / (distance*distance );
    float x = dot( normalized_normal, lp+pos);
    vec4 baseColor = vec4(1.0f)*0.1f+constants.lightColor*0.9f;
    
    outColor = baseColor; 
}