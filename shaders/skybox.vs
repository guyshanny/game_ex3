#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
                                                                                    
uniform mat4 gWVP;                                                                  
                                                                                    
out vec3 TexCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 WVP_Pos = gWVP * vec4(position, 1.0);                                      
    gl_Position = WVP_Pos.xyww;                                                     
    TexCoord0   = position;                                                         
}
