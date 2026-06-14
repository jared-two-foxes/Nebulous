#version 120

attribute vec4 a_vertex;
attribute vec2 a_texcoord;

varying vec2 vTexCoord;

void main()
{
  vTexCoord = a_texcoord;
  gl_Position = a_vertex;
}
