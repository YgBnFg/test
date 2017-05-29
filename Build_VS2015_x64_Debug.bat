@echo off
set "sourceDir=%~dp0"
set "currentDir=%cd%"
echo conan install
if not exist %sourceDir%\_build\vs2015\x64\Debug\ (
mkdir %sourceDir%"\_build\vs2015\x64\Debug\
)
cd %sourceDir%\_build\vs2015\x64\Debug\
conan install %sourceDir% -u -s arch="x86_64" -s build_type="Debug" -s compiler="Visual Studio" -s compiler.runtime="MDd" -s compiler.version="14"

echo cmake build
cmake %sourceDir% -G "Visual Studio 14 Win64"
cmake --build . --config Debug
echo done
pause