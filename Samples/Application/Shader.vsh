//
//  Shader.vsh
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

attribute vec4 a_vertex;
attribute vec3 a_normal;

varying vec3 v_modelViewVertex;
varying vec3 v_modelViewNormal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform vec4 diffuseColor;

void main()
{
  v_modelViewVertex = vec3(modelViewMatrix * a_vertex);
  v_modelViewNormal = vec3(normalMatrix * vec4(a_normal, 0.0));

  gl_Position       = modelViewProjectionMatrix * a_vertex;
}
