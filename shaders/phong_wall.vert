#version 330 core

#define MY_PI 3.1415926
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 norm;
layout(location = 2) in vec4 texCoord;

// Output data will be interpolated for each fragment.
out vec3 PositionWorldPass;
out vec3 NormalViewPass;
out vec3 EyeDirectionViewPass;
out vec3 LightDirectionViewPass;
out vec3 LightPositionViewPass;
out vec2 TexCoordPass;

// Values that stay constant for the whole mesh.
uniform mat4 gModel, gView, gProjection;
uniform vec4 gLightPosition; // light (in world)
uniform vec3 gEyePosition; // camera (in world)

void main()
{
	mat4 MVP = gProjection * gView * gModel;
	
	// Output position of the vertex, in clip space : gMVP * position
	gl_Position =  MVP * position;

	// Position of the vertex, in worldspace : gWorld * position
	PositionWorldPass = (gModel * position).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 PositionView = (gView * gModel * position).xyz;
	EyeDirectionViewPass = gEyePosition - PositionView;
	
	// Vector that goes from the vertex to the light, in camera space.
	LightPositionViewPass = (gView * gModel * gLightPosition).xyz;
	LightDirectionViewPass = LightPositionViewPass + EyeDirectionViewPass;
	
	// Normal of the the vertex, in camera space
	NormalViewPass = (gView * gModel * norm).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	TexCoordPass = texCoord.xy;
}