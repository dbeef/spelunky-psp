@echo off
setlocal
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build\windows_mingw

:: Debug version.
if not exist "%BUILD_DIR%_debug" mkdir "%BUILD_DIR%_debug" || goto error
pushd "%BUILD_DIR%_debug" || goto error
call cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug VERBOSE=0 "%PROJECT_DIR%" || goto error
call cmake --build . || goto error
popd

:: Release version.
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%" || goto error
pushd "%BUILD_DIR%" || goto error
call cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING=MinSizeRel VERBOSE=0 "%PROJECT_DIR%" || goto error
call cmake --build . || goto error
popd

goto end

:error
echo Build failed!
exit /b 1

:end
