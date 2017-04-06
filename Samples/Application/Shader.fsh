//
//  Shader.fsh
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

varying vec4 colorVarying;

void main()
{
  gl_FragColor = colorVarying;
}
