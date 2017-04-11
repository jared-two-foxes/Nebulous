
uniform sampler2D diffuseTexture;
uniform vec4      colour; 

varying vec2      v_texcoord;

void main (void)
{
  vec4 diffuse = texture2D( diffuseTexture, v_texcoord );
  
  gl_FragColor = diffuse * colour;
}