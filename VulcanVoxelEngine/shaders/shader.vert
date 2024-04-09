#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in mat4 matrix;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout (location = 2) out vec3 outUVW;

void main() {
    outUVW = inPosition;

    vec4 worldPos = matrix * vec4(inPosition, 1.0);

    vec4 viewPos = ubo.view * worldPos;
    vec4 projectionPos = ubo.proj * viewPos;
    gl_Position = projectionPos;
    
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}