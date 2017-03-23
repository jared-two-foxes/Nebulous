
uniform sampler2D diffuseTexture;

varying vec2 v_texcoord;

void main (void)
{
  vec4 color   = texture2D( diffuseTexture, v_texcoord );
  
  gl_FragColor = color;
}