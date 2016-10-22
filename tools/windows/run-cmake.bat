@echo off

set BUILD_DIR=%~dp0..\..\build

where cmake.exe 2> nul || goto:setup_path
goto:run_cmake

:setup_path
set PATH=%~dp0cmake-3.6.2-win32-x86\bin;%PATH%

:run_cmake
pushd %BUILD_DIR%

cmake.exe -G "Visual Studio 14 Win64" -DCMAKE_BUILD_TYPE=DEBUG ../src

popd
