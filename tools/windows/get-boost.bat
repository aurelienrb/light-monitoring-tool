@echo off

set BUILD_DIR=%~dp0..\..\build
mkdir %BUILD_DIR% 2> nul

where conan 2> nul || goto:setup_path
goto:get_boost

:setup_path
set PATH=%~dp0Conan-0.14.1\conan;%PATH%

:get_boost
set CONAN_USER_HOME=%BUILD_DIR%
copy %~dp0conanfile.txt %BUILD_DIR%
set COMMON_OPTIONS=-s compiler="Visual Studio" -s compiler.version=14 -s arch=x86_64

pushd %BUILD_DIR%

conan install %COMMON_OPTIONS% -s compiler.runtime=MDd -s build_type=Debug || goto:failed
ren conanbuildinfo.cmake conanbuildinfo-debug.cmake > nul || goto:failed

conan install %COMMON_OPTIONS% -s compiler.runtime=MD -s build_type=Release || goto:failed
ren conanbuildinfo.cmake conanbuildinfo-release.cmake > nul || goto:failed

popd

goto:eof

:failed
echo FAILED!
pause
