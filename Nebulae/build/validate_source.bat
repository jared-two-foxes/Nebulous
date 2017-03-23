"%~dp0../tools/cppcheck/cppcheck.exe" -j 4 --template=vs --enable=style,warning,performance,portability --suppress=cstyleCast --inline-suppr -I "%~dp0../Nebulae" "%~dp0../Nebulae"
