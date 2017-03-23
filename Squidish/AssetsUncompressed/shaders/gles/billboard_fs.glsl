
uniform sampler2D     diffuseTexture;
uniform mediump vec4  tintColour;
uniform mediump float tintFactor;

varying mediump vec2 v_texcoord;

void main(void)
{
  mediump vec4 diffuseColour = texture2D( diffuseTexture, v_texcoord );

  if( diffuseColour.a > 0.01 ) {
    gl_FragColor = (1.0 - tintFactor) * diffuseColour + tintFactor * tintColour;
  } else {
    gl_FragColor = diffuseColour;
  }
}