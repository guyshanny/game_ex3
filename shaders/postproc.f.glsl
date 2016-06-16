#version 330

#define M_PI 3.1415926
#define KERNEL_SIZE 3

uniform int textureMode;

// convolutionEffect effect
uniform mat3 convMatrix;

// waveEffect effect
uniform float offset;

// Action based effect
uniform sampler2D fbo_texture;

in vec2 UV;
out vec3 color;

// Conv & glow
#define TEXTURE_POS (UV + 1.0/textureDims * vec2(i,j))

// Based on ImageProcessing course & https://en.wikipedia.org/wiki/Kernel_(image_processing)
vec3 convolutionEffect()
{
	vec3 retColor = vec3(0, 0, 0);
	
	// Retrieving texture's data
	vec3[KERNEL_SIZE*KERNEL_SIZE] textureData;
	vec2 textureDims = textureSize(fbo_texture, 0) / 2.0;
	int padding = KERNEL_SIZE % 2;
	
	for(int i = -padding; i < KERNEL_SIZE - padding; i++)
	{
		for(int j = -padding; j < KERNEL_SIZE - padding; j++)
		{
			textureData[(i+1)*KERNEL_SIZE + (j+1)] = texture(fbo_texture, TEXTURE_POS).xyz;
		}
	}
	
	// Applying convolutionEffect
	for(int i = 0; i < KERNEL_SIZE; i++)	
	{
		for(int j = 0; j < KERNEL_SIZE; j++)
		{
			retColor += textureData[i*KERNEL_SIZE + j] * convMatrix[i][j];
		}
	}
	
	return retColor;
}

// Based on https://en.wikibooks.org/wiki/OpenGL_Programming/Post-Processing
vec3 waveEffect()
{
	vec3 retColor = vec3(0, 0, 0);
	vec2 texcoord = UV;
	texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;
	return texture(fbo_texture, texcoord);
}

void main()
{
	switch (textureMode)
	{
		case 0:	
			color = convolutionEffect();
		break;
		
		case 1:
			color = waveEffect();
		break;
		
		default:
			color = vec3(0, 0, 0);
		break;
	}
}