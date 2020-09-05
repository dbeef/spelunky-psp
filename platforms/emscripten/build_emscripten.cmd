@echo off
setlocal
set PROJECT_DIR=%~dp0..\..
set BUILD_DIR=%PROJECT_DIR%\build\emscripten

:: Find emscripten
if exist "%EMSCRIPTEN_HOME%\emsdk_env.bat" goto emscripten_ok
if exist "%EMSDK%\emsdk_env.bat" set EMSCRIPTEN_HOME="%EMSDK%" && goto emscripten_ok
echo Emscripten not found or location not defined (EMSDK="%EMSDK%")
goto error
:emscripten_ok
call "%EMSCRIPTEN_HOME%\emsdk_env.bat"


:: Build debug version
::if not exist "%BUILD_DIR%_debug" mkdir "%BUILD_DIR%_debug" || goto error
::pushd "%BUILD_DIR%_debug" || goto error
::call emcmake cmake -D CMAKE_BUILD_TYPE:STRING=Debug "%PROJECT_DIR%" || goto error
::call cmake --build . || goto error
::popd

:: Build release version
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%" || goto error
pushd "%BUILD_DIR%" || goto error
call emcmake cmake -D CMAKE_BUILD_TYPE:STRING=Release "%PROJECT_DIR%" || goto error
call cmake --build . || goto error
popd


goto end

:error
echo Build failed!
exit /b 1

:end
