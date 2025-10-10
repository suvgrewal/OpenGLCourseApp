#version 330

layout (location = 0) in vec3 pos;   // XYZ
layout (location = 1) in vec2 tex;   // UV

out vec4 vColor;
out vec2 TexCoord0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0);

    TexCoord0 = tex;
}