#version 330

layout (location = 0) in vec4 position;

uniform mat4 gVP;

out float vSize;

void main()
{
	vSize = position.w;
    gl_Position = vec4(position.xyz, 1.0);
}
