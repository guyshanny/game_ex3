#version 330
                                               
uniform sampler2D gTextureSampler;

in vec2 TexCoordPass;
out vec4 FragColor;

void main()
{
	FragColor = texture2D(gTextureSampler, TexCoordPass);
    
    if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0) {
		discard;
	}
}