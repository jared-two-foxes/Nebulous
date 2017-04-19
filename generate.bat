
:: Make the solution file by running premake.
::premake5 %* --clean --build_dependencies --plugins --samples --tests vs2015
premake5 %* --clean --plugins --build_dependencies --samples vs2015
