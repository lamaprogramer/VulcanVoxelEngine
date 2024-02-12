#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout( push_constant ) uniform ModelMatrixObject {
    mat4 modelMatrix;
} mmo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    vec4 worldPos = mmo.modelMatrix * vec4(inPosition, 1.0);
    vec4 viewPos = ubo.view * worldPos;
    vec4 projectionPos = ubo.proj * viewPos;
    gl_Position = projectionPos;
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
}