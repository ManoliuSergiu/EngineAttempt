#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

// A "Push Constanad" is a small chunk of data we send directly to the GPU
layout(push_constant) uniform PushConstants {
    mat4 render_matrix;
} constants;

void main() {
    // Apply the matrix to the position
    gl_Position = constants.render_matrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}