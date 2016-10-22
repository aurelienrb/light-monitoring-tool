@echo off

set BUILD_DIR=%~dp0\build
mkdir %BUILD_DIR% 2> nul

where cmake.exe 1>nul 2>&1 && goto:cmake_ok
REM add cmake to PATH
set PATH=%~dp0tools\windows\cmake-3.6.2-win32-x86\bin;%PATH%
:cmake_ok

:run_conan
echo =================
echo Checking for Boost library
call tools\windows\get-boost.bat || exit /b 1

:run_bin2cpp
echo =================
echo Embedding static html files
call src\html-files\generate-cpp.bat || exit /b 1

:run_cmake
echo =================
echo Calling CMAKE
pushd %BUILD_DIR%
cmake.exe -G "Visual Studio 14 Win64" -DCMAKE_BUILD_TYPE=DEBUG ../src || exit /b 1
popd

echo.
echo Done!