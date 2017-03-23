
uniform mat4 projection;
uniform vec2 size;
uniform vec2 offset;
uniform vec2 texCoord;

attribute vec2 a_vertex;

varying vec2 v_texcoord;

void main(void)
{ 
  // Determine the texture coordinates
  v_texcoord = a_vertex;
  v_texcoord *= texCoord;

  // Adjust vertex position using the size and offset.
  vec2 p = a_vertex*size*texCoord + offset;
	
  // Make it a 4 value vector. 
  vec4 vert = vec4( p, -1.0, 1.0 );
  
  // Generate the vertex position
  gl_Position = vert * projection;
}