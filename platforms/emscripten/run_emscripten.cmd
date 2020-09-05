@echo off
setlocal
set PROJECT_DIR=%~dp0..\..
set BIN_DIR=%PROJECT_DIR%\bin\emscripten

:: Find emscripten
if exist "%EMSCRIPTEN_HOME%\emsdk.bat" goto emscripten_ok
echo Emscripten not found or location not defined (EMSCRIPTEN_HOME="%EMSCRIPTEN_HOME%")
goto error
:emscripten_ok
call "%EMSCRIPTEN_HOME%\emsdk.bat" activate latest

:: Serve and run html
call emrun --no_emrun_detect "%BIN_DIR%\SimpleExample.html" || goto error

goto end

:error
echo failed!
exit /b 1

:end
