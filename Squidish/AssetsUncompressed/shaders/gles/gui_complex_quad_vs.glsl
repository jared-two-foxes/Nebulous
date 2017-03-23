
uniform mat4  projection;
uniform vec4  left;
uniform vec4  right;
uniform vec4  texcoord;
uniform float depth;

attribute vec2 a_vertex; // vertex position

varying vec2 v_texcoord;

void main(void)
{
  // Store the texture coordinates.
  v_texcoord  = texcoord.xy + a_vertex * (texcoord.zw - texcoord.xy);

  // Adjust vertex position size and offset.
  vec2 p;

  if( a_vertex.x < 0.5 )
  {
    if( a_vertex.y < 0.5 )
    {
      p = left.xy;
    }
    else
    {
      p = left.zw;
    }
  }
  else
  {
    if( a_vertex.y < 0.5 )
    {
      p = right.xy;
    }
    else
    {
      p = right.zw;
    } 
  }
  
  // Generate the vertex position
  gl_Position = vec4( p, -10, 1.0 ) * projection;
}