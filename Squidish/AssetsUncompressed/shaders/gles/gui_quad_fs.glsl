
uniform sampler2D    diffuseTexture;
uniform mediump vec4 colour; 

varying mediump vec2 v_texcoord;

void main (void)
{
  mediump vec4 diffuse = texture2D( diffuseTexture, v_texcoord );
  
  gl_FragColor = diffuse * colour;
}