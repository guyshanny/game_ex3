#version 330

layout(location = 0) in vec2 v_coord;

out vec2 UV;
out vec3 PositionWorldPass;
 
void main(void) {
  gl_Position = vec4(v_coord, 0, 1);
  UV = (v_coord + 1.0) / 2.0;
}
