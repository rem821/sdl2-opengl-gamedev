#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;


layout (location = 0) out vec4 outColor;

struct PointLight {
    vec3 position;
    vec4 color;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 invViewMatrix;
    vec4 ambientLightColor;
    PointLight pointLights[10];
    int numLights;
} ubo;


layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    outColor = vec4(fragColor, 1.0);
}