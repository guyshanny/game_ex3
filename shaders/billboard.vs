#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in float type;

uniform mat4 gVP;

out float vSize;
out float vType;

void main()
{
	vType = type;
	vSize = position.w;
    gl_Position = vec4(position.xyz, 1.0);
}
