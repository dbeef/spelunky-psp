@echo off
setlocal
set PROJECT_DIR=%~dp0\..
set PLATFORM_DIR=%PROJECT_DIR%\platforms\android

:: USAGE:
:: "build_android" -> builds android apk
:: "build_android clean" -> Detele previously built object files


set BUILD_TARGETS=assemble
::set BUILD_TARGETS=assembleDebug
::set BUILD_TARGETS=assembleRelease

if /I "%1" == "clean" goto clean

pushd %PLATFORM_DIR%
call gradlew.bat %BUILD_TARGETS% || goto error
popd
goto end

:clean
pushd %PLATFORM_DIR%
call gradlew.bat clean || goto error
popd
goto end

:error
echo Build failed!
exit /b 1

:end
