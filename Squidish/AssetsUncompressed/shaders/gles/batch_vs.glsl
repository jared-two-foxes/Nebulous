
uniform mat4  projection;

attribute vec4 a_vertex;    // vertex position
attribute vec4 a_diffuse;   // diffuse
attribute vec4 a_texcoord;  // texcoords

varying vec2 v_texcoord;
varying vec4 v_colour;

void main(void)
{ 
  // Store the texture coordinates.
  v_texcoord  = a_texcoord.xy;
  v_colour    = a_diffuse;

  // Generate the vertex position
  gl_Position = a_vertex * projection;
}