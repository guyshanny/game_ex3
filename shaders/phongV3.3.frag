#version 330
#define MY_PI 3.1415926
#define LIGHT_MAX_ANGLE (10.0f)

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
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gModel;
uniform vec4 gLightPosition; // light (in world)
uniform vec4 gLightDirection;
uniform vec4 gLightColor;
uniform float lightFlicker;

void main()
{	
//	if (1.0 == lightFlicker)
//	{
//		outColor = vec3(0, 0, 0);
//		return;
//	}

	vec3 diffusiveColor;
	vec3 specularColor;
	float attenuation;

	vec3 lightDirection = (gView * gLightDirection).xyz;
	vec3 lightColor = gLightColor.rgb;

	// Material properties
	vec3 materialDiffuseCoefficient = texture2D(gTextureSampler, TexCoordPass).rgb * gMaterialColor.rgb;
	vec3 materialSpecularCoefficient = vec3(1.0,1.0,1.0);

	// Ambient color
	vec3 ambientColor = vec3(0.1,0.1,0.1) * materialDiffuseCoefficient;

	// Vectors calculations
	// Normal of the computed fragment, in camera space
	vec3 N = normalize(NormalViewPass);
	// Direction of the light (from the fragment to the light)
	vec3 L = normalize(gLightDirection.xyz);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-L, N);
	
	// Calculate diffusive color
	{
		// Cosine of the angle between the normal and the light direction, 
		/* clamped above 0
		  - light is at the vertical of the triangle -> 1
		  - light is perpendicular to the triangle -> 0
		  - light is behind the triangle -> 0 */
		float cosTheta = clamp(dot(N, L), 0, 1);
		
		diffusiveColor = materialDiffuseCoefficient * lightColor * cosTheta;
	}

	// Calculate specular color
	{
		// Eye vector (towards the camera)
		vec3 V = normalize(EyeDirectionViewPass);
		// Cosine of the angle between the Eye vector and the Reflect vector,
		/* clamped to 0
		  - Looking into the reflection -> 1
		  - Looking elsewhere -> < 1 */
		float cosAlpha = clamp(dot(V,R), 0, 1);
		
		specularColor = materialSpecularCoefficient * lightColor * pow(cosAlpha, 400);
	}

	// Calculate attenuation
	{
		float gLightAttenuation = 0.0005; //should be uniform
		vec3 temp = LightPositionViewPass - PositionWorldPass;
		//attenuation by distance of fragment from light:
		attenuation = 1.0 / (1.0 + gLightAttenuation * pow(length(temp), 2));

		float lightToFragmentAngle = degrees(acos(dot(normalize(temp), R)));
		float circleFactor = 1 - lightToFragmentAngle / 10.0;
		attenuation *= circleFactor;
	}

	outColor = vec3(0, 0, 0);
	outColor += diffusiveColor;
	outColor += specularColor;
	outColor *= attenuation;
	outColor += ambientColor;
}