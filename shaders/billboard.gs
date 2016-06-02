#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
//layout(triangle_strip) out;                                                         
layout(points) out;

layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 gVP;                                                                   
uniform vec3 gCameraPos;                                                            
                                                                                    
out vec2 TexCoordPass;                                                                  
                                                                                    
void main()                                                                         
{              

                                                                     
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(gCameraPos - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
    
	// bottom left corner 
    pos -= (right * 0.5); 
    gl_Position = gVP * vec4(pos, 1.0); 
    TexCoordPass = vec2(0.0, 0.0); 
    EmitVertex(); 
            
    EndPrimitive();
	
	/*        
	// top left corner 
    pos.y += 1.0; 
    gl_Position = gVP * vec4(pos, 1.0); 
    TexCoordPass = vec2(0.0, 1.0); 
    EmitVertex(); 
     
	// bottom right corner 
    pos.y -= 1.0; 
    pos += right; 
    gl_Position = gVP * vec4(pos, 1.0); 
    TexCoordPass = vec2(1.0, 0.0); 
    EmitVertex(); 
	 
	// top right corner
	pos.y += 1.0; 
    gl_Position = gVP * vec4(pos, 1.0); 
    TexCoordPass = vec2(1.0, 1.0); 
    EmitVertex(); 
	 
    EndPrimitive();*/ 
}                                                                                   
