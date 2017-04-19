-- Format: { projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {
  gmock = {   
    nil,               
    nil,                                                               
    'googletest/googlemock/include',
    'googletest/build/googlemock/$(Configuration)',  
    nil,               
    'gmock.lib',
    'gmock.lib',
  },

  gtest = {     
    nil,                            
    nil,                                                               
    'googletest/googletest/include',              
    'googletest/build/googlemock/gtest/$(Configuration)',  
    nil,               
    'gtest.lib',
    'gtest.lib',
  },  

}