//
//  Shader.vsh
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

attribute vec4 a_vertex;
attribute vec3 a_normal;

varying vec4 colorVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;                //modelViewMatrix
uniform vec4 diffuseColor;

void main()
{
  vec3  lightPosition = vec3(0.0, 0.0, 1.0);
  vec3  modelViewVertex = vec3(normalMatrix * a_vertex);
  vec3  modelViewNormal = vec3(normalMatrix * vec4(a_normal, 0.0));
  
  float distance    = length( lightPosition - modelViewVertex );
  vec3  lightVector = normalize( lightPosition - modelViewVertex );
  float diffuse     = max( dot(modelViewNormal, lightVector), 0.2 ); //< note that we are clamping to 0.2
  diffuse          *= (1.0 / (1.0, + (0.25 * distance * distance)));

  colorVarying      = diffuse * diffuseColor;

  gl_Position       = modelViewProjectionMatrix * a_vertex;
}
