//
//  Shader.fsh
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

varying vec3 v_modelViewVertex;
varying vec3 v_modelViewNormal;

uniform vec4 diffuseColor;

void main()
{
  vec3 lightPosition = vec3(0.0, 0.0, 1.0);

  vec3  modelViewVertex = v_modelViewVertex;
  vec3  modelViewNormal = normalize(v_modelViewNormal);

  float distance    = length( lightPosition - modelViewVertex );
  vec3  lightVector = normalize( lightPosition - modelViewVertex );
  float diffuse     = max( dot(modelViewNormal, lightVector), 0.2 );
  diffuse          *= (1.0 / (1.0 + (0.25 * distance * distance)));

  gl_FragColor      = diffuse * diffuseColor;
}
