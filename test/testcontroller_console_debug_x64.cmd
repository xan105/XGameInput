@echo off
setlocal

REM Add the API DLL directory to the process's DLL search path
set "PATH=%~dp0..\build\XGameInput\output\x64\Debug;%PATH%"

REM Launch the test executable.
"%~dp0testcontroller\build\testcontroller_console\output\x64\Debug\testcontroller_console.exe"

endlocal