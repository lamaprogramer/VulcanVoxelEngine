#version 450

layout (set = 1, binding = 0) uniform samplerCube cubeMapTexture;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout (location = 2) in vec3 inUVW;

layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(fragTexCoord, 0.0, 1.0);
    outColor = texture(cubeMapTexture, inUVW);
}