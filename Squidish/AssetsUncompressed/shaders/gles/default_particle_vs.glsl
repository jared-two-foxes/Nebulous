
uniform mat4 world;         
uniform mat4 view;            
uniform mat4 projection;
uniform vec2 size;
uniform vec2 offset;

attribute vec2 a_vertex;  // vertex position

varying vec2 v_texcoord;

void main(void)
{ 
  // Store the texture coordinates.
  v_texcoord = a_vertex;

  // Determine renderspace location
  vec2 p    = a_vertex*size + offset;
  vec4 vert = vec4( p, 1.0, 1.0 ); // z & w components set to 1
  mat4 mvp  = world * view * projection;
  
  gl_Position = mvp * vert;
}