#version 120

uniform sampler2D s_texture;

varying vec2 vTexCoord;

void main()
{
  gl_FragColor = texture2D(s_texture, vTexCoord);
}
