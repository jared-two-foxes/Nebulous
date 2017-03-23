uniform mat4 world;         
uniform mat4 view;            
uniform mat4 projection;

uniform vec2 size;
uniform vec2 offset;
uniform vec2 min_uv;
uniform vec2 max_uv;

attribute vec2 a_vertex;        // vertex position

varying vec2 v_texcoord;

void main(void)
{ 
  // Store the texture coordinates.
  v_texcoord = min_uv + a_vertex * ( max_uv - min_uv );

  // Adjust vertex position size and offset.
  vec2 p = a_vertex*size + offset;
	
  // Make it a 4 value vector. 
  vec4 vert = vec4( p, 0.0, 1.0 );
  
  // Generate the projection view world matrix
  mat4 mvp = world * view * projection;
  
  // Generate the vertex position
  gl_Position = vert * mvp;
}