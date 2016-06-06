#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform vec3 gUp;

out vec2 TexCoordPass;

void main() {
	float size = 5.f;
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(gCameraPos - pos);
	vec3 up = gUp;
    vec3 right = normalize(cross(gUp, toCamera));
    
	// bottom left corner
    pos -= (right * 0.5) * size;
    gl_Position = gVP * vec4(pos, 1.0);
    TexCoordPass = vec2(0.0, 1.0);
    EmitVertex();

	// top left corner
    pos.y += 1.0 * size;
    gl_Position = gVP * vec4(pos, 1.0);
    TexCoordPass = vec2(0.0, 0.0);
    EmitVertex();
	
	// bottom right corner
    pos.y -= 1.0 * size;
    pos += right * size;
    gl_Position = gVP * vec4(pos, 1.0);
    TexCoordPass = vec2(1.0, 1.0);
    EmitVertex();
	
	// top right corner
	pos.y += 1.0 * size;
    gl_Position = gVP * vec4(pos, 1.0);
    TexCoordPass = vec2(1.0, 0.0);
    EmitVertex();
	
    EndPrimitive();
}
