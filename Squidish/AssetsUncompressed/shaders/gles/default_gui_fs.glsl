
uniform sampler2D diffuseTexture;

varying mediump vec2 v_texcoord;

void main (void)
{
  mediump vec4 color = texture2D( diffuseTexture, v_texcoord );
  
  gl_FragColor = color;
}