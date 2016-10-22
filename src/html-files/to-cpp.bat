@echo off

set BIN2CPP=%~dp0..\..\tools\bin2cpp\build-msvc\bin\bin2cpp.exe
if not exist %BIN2CPP% (
    echo bin2cpp.exe is missing!
    exit /b 1
)

REM generate C++ files in ../generated/ folder
set OUTDIR=%~dp0..\generated
mkdir %OUTDIR% 2> nul

echo Generating cpp files in %OUTDIR%

%BIN2CPP% -ns static_files -d %OUTDIR% -o staticfiles css js || exit /b 1

echo Success!
exit /b 0
