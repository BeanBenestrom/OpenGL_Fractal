#version 430 core

layout(binding = 0) uniform sampler2D tex;

out vec4 FragColor;
in vec2 texCoord;

void main()
{
    vec3 texCol = texture(tex, texCoord).rgb;
    FragColor = vec4(texCol, 1);
} 