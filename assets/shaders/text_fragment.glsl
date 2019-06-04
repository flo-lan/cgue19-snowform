// Fragment Shader
#version 430

const float smoothing = 1.0 / 16.0;

layout(location = 2) uniform sampler2D atlasTexture;
layout(location = 3) uniform vec4 textColor;

in vec2 fragUV;

out vec4 colorOut;

void main()
{
    // Retrieve distance from texture
    float distance = texture(atlasTexture, fragUV).a;
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    colorOut = vec4(1.0, 1.0, 1.0, alpha) * textColor;
}
