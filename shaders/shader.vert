#version 460 core

// 1. Inputs from C++ (must match your Vertex struct)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

// 2. Output to Fragment Shader
layout(location = 0) out vec3 fragColor;

// 3. Push Constants (The "Global" 4x4 Matrix you send every frame)
layout(push_constant) uniform PushConstants {
    mat4 render_matrix; 
} constants;

void main() {
    // Pass color through to the next stage
    fragColor = inColor;

    // Calculate final position: Matrix * Position
    // The "1.0" is required to turn a 3D point (x,y,z) into a 4D homogeneous vector (x,y,z,w)
    gl_Position = constants.render_matrix * vec4(inPosition, 1.0);
}