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
    vec3 lightDir = -vec3(constants.lightPos) + pos;
    float d = max(0.0,dot(normalize(lightDir),normalize(normal)));
    //vec3 debugColor = normalize(normal) * 0.5 + 0.5;

    //outColor=vec4(debugColor,1.0);
    outColor=d*vec4(fragColor,1.0)*constants.lightColor;
}