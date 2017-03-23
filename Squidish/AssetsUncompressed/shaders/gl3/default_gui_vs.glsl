
#define M_PI 3.1415926535897932384626433832795

uniform mat4 projection;
uniform vec2 size;
uniform vec2 offset;

uniform float angle;

attribute vec2 a_vertex; // vertex position

varying vec2 v_texcoord;

void main(void)
{ 
  // Store the texture coordinates.
  v_texcoord  = a_vertex;

  mat2  m  = mat2( cos(angle), -sin(angle), sin(angle), cos(angle) );
  vec2  p  = a_vertex*size;
  p       *= m;

  // Adjust vertex position size and offset.
  p += offset;
	
  // Generate the vertex position
  gl_Position = vec4( p, -10.0, 1.0 ) * projection;
}