@echo off
setlocal
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build\windows64_vs14

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%" || goto error
pushd "%BUILD_DIR%" || goto error

:: Configure cmake.
call cmake -G "Visual Studio 14 2015 Win64" "%PROJECT_DIR%" || goto error

:: Build the application.
call cmake --build . --config Debug || goto error
call cmake --build . --config Release || goto error

popd
goto end

:error
echo Build failed!
exit /b 1

:end
