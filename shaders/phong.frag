#version 330 core

// Interpolated values from the vertex shaders
in vec3 PositionWorldPass;
in vec3 NormalViewPass;
in vec3 EyeDirectionViewPass;
in vec3 LightDirectionViewPass;
in vec3 LightPositionViewPass;
in vec2 TexCoordPass;
// Ouput data
out vec3 outColor;

// Values that stay constant for the whole mesh.
uniform sampler2D gTextureSampler;
uniform vec4 gMaterialColor;
uniform mat4 gView;
uniform mat4 gModel;
uniform vec4 gLightPosition; // light (in world)
uniform vec4 gLightColor;

void main()
{
	vec3 LightColor = gLightColor.rgb;
	
	// Material properties
	vec3 materialDiffuseColor = texture2D(gTextureSampler, TexCoordPass).rgb * gMaterialColor.xyz;
	vec3 materialAmbientColor = vec3(0.1,0.1,0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(1.0,1.0,1.0);

	// Normal of the computed fragment, in camera space
	vec3 N = normalize(NormalViewPass);
	// Direction of the light (from the fragment to the light)
	vec3 L = normalize(LightDirectionViewPass);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(N, L), 0, 1);
	
	// Eye vector (towards the camera)
	vec3 V = normalize(EyeDirectionViewPass);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-L, N);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(V,R), 0, 1);

	float gLightAttenuation = 0.0005; //should be uniform
	vec3 temp = LightPositionViewPass - PositionWorldPass;
	//attenuation by distance of fragment from light:
	float attenuation = 1.0 / (1.0 + gLightAttenuation * pow(length(temp), 2));
	
	outColor = vec3(0, 0, 0);
	outColor += materialDiffuseColor * LightColor * cosTheta;			// Diffuse : "color" of the object
	outColor += materialSpecularColor * LightColor *pow(cosAlpha,5);	// Specular : reflective highlight, like a mirror
	outColor *= attenuation;
	outColor += materialAmbientColor;									// Ambient : simulates indirect lighting
		
}