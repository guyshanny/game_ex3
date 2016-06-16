#version 330
                                               
//uniform sampler2D gTextureSampler;
//uniform sampler2D gTextureSampler[8];
uniform sampler2D gTextureSampler0;
uniform sampler2D gTextureSampler1;
uniform sampler2D gTextureSampler2;
uniform sampler2D gTextureSampler3;
uniform sampler2D gTextureSampler4;
uniform sampler2D gTextureSampler5;
uniform sampler2D gTextureSampler6;
uniform sampler2D gTextureSampler7;

in vec2 TexCoordPass;
in float typePass;
out vec4 FragColor;

void setColor(int type)
{
    switch(type)
    {
        case 0:
            FragColor = texture2D(gTextureSampler0, TexCoordPass);
            break;
        case 1:
            FragColor = texture2D(gTextureSampler1, TexCoordPass);
            break;
        case 2:
            FragColor = texture2D(gTextureSampler2, TexCoordPass);
            break;
        case 3:
            FragColor = texture2D(gTextureSampler3, TexCoordPass);
            break;
        case 4:
            FragColor = texture2D(gTextureSampler4, TexCoordPass);
            break;
        case 5:
            FragColor = texture2D(gTextureSampler5, TexCoordPass);
            break;
        case 6:
            FragColor = texture2D(gTextureSampler6, TexCoordPass);
            break;
        case 7:
            FragColor = texture2D(gTextureSampler7, TexCoordPass);
            break;
        default:
            FragColor = texture2D(gTextureSampler0, TexCoordPass);
            break;
    }
}

void main()
{
    setColor(int(typePass));
	//FragColor = texture2D(gTextureSampler, TexCoordPass);
    //FragColor = texture2D(, TexCoordPass);
    
    if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0) {
		discard;
	}
}