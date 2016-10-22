@echo off
REM Simple script to build bin2c with VC++ 

:configure_v140
echo Configuring VC++ 2015...
if not exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" GOTO:EOF
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x86_amd64 || GOTO:EOF
goto:build_src

:build_src
echo.
echo Building bin2c...
set OUTDIR=%~dp0bin\win32
mkdir %OUTDIR% || goto:eof
pushd %OUTDIR%
cl /nologo /O1 /MT %~dp0/src/bin2c.c
del *.obj 2> nul
popd

echo.

if not exist %OUTDIR%\bin2c.exe (
    echo Failure!
    pause
    goto:eof
)

echo Success!
pause
