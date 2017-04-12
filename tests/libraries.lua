-- Format: { projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {
  gmock = {                  
    nil,                                                               
    'googletest/googlemock/include',
    'googletest/build/googlemock/$(Configuration)',  
    'gmock.lib',
    'gmock.lib',
  },

  gtest = {                  
    nil,                                                               
    'googletest/googletest/include',              
    'googletest/build/googlemock/gtest/$(Configuration)',  
    'gtest.lib',
    'gtest.lib',
  },  

}