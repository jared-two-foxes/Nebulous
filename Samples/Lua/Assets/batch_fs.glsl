
uniform sampler2D    diffuseTexture;

varying vec2 v_texcoord;
varying vec4 v_colour;

void main (void)
{ 
  gl_FragColor = v_colour * texture2D( diffuseTexture, v_texcoord );
}