#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

uniform mat4 gVP;

out vec3 vColor;
out float vSize;

void main()
{
	vSize = position.w;
    gl_Position = vec4(position.xyz, 1.0);
	vColor = color;
}
