#version 450
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 0) out vec3 fragColor;

// Remove push constants for a second
void main() {
    // FORCE the triangle to the center of the screen
    // This ignores your camera, rotation, and model data.
    // If you see NOTHING now, your Vertex Buffer is empty or broken.
    gl_Position = vec4(inPosition.x * 0.01, inPosition.y * 0.01, 0.5, 1.0);
    
    fragColor = vec3(1.0, 1.0, 1.0); // Force White
}