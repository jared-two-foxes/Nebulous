
uniform sampler2D    diffuseTexture;

varying mediump vec2 v_texcoord;
varying mediump vec4 v_colour;

void main (void)
{ 
  gl_FragColor = v_colour * texture2D( diffuseTexture, v_texcoord );
}