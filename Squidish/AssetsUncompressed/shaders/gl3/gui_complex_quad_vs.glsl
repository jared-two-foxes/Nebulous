
uniform mat4  projection;
uniform vec4  texcoord;

attribute vec2 a_vertex; // vertex position

varying vec2 v_texcoord;

void main(void)
{ 
  // Store the texture coordinates.
  v_texcoord  = texcoord.xy + a_vertex * (texcoord.zw - texcoord.xy);

  // Generate the vertex position
  gl_Position = vec4( a_vertex, -10.0, 1.0 ) * projection;
}