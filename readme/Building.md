# Building

### Notes

* All assets are compiled inside the final artifact (ELF/EXE/PBP), so it can be moved around the filesystem and ran as it is.
* There's a Dockerfile in the root of the repository defining build environment for Linux/PSP/Android. You can use it for reference.

### Linux

To install dependencies:

```bash
sudo apt install libsdl1.2-dev libsdl-mixer1.2-dev
```

To build:

```bash
./scripts/config-linux.sh
./scripts/build-linux.sh
```

The artifact (ordinary ELF executable) will reside under:

`./tmp/install-linux/Release/bin/Spelunky_PSP`

### PSP

Building requires a free PSP toolchain, which has to be built first:
https://github.com/pspdev/psptoolchain 

The toolchain includes a few libraries on its own, including SDL 1.2.15, hence 
no additional action is needed.

After building and installing the toolchain, restart the computer so to make sure that
the environment variables are set, and call:

```bash
./scripts/config-psp.sh
./scripts/build-psp.sh
```

The artifact (.PBP file) will reside under:

`./tmp/build-psp/EBOOT.PBP`

To optionally run the artifact on the [PPSSPP](https://github.com/hrydgard/ppsspp) emulator (must be present in the system),
call:

```bash
./scripts/run-psp.sh
```

#### Windows

In your powershell console:

```powershell
.\scripts\dependencies-install.ps1
.\scripts\config-windows.ps1
.\scripts\build-windows.ps1
```

#### Android

Android is a very special case in the build system. The official channel of distribution (through .APK files) requires
each application to be wrapped around at least one Java `Activity` instance, which represents a running process.

Because of that, Spelunky_PSP target is built as a shared library instead of an executable. When invoking gradle,
it will in turn run CMake and then bundle artifact `libSpelunky_PSP.so` to the .APK file. 

SDL ships a few Java files, including an activity that will bootstrap the application and then retrieve pointer to the main
function from `libSpelunky_PSP.so` and call it through JNI. These files are compiled and included in the .APK file, aswell
as other Android-related boilerplate (AndroidManifest.xml, strings.xml, icons).
From the moment of calling main function through JNI until return, the code will be executed in ordinary way as on other platforms.   

Building requires Android NDK r21e, Android SDK and SDL 2.0.14 being crosscompiled in 2 architectures (armeabi-v7a/arm64-v8a).
To set-up building environment locally, you can reffer to the Dockerfile in the root of the repository. When set-up, call `scripts/build-android.sh`.

Alternatively, you can build the image:

```bash
sudo docker build . -t test/spelunky-psp:latest
```

...and invoke `scripts/build-android.sh` through docker.

Minimum supported Android API version is 15 (Android 4.0.3) and built .APK file is multi-arch, meaning that native artifacts for 
more than one architecture are bundled (armeabi-v7a/arm64-v8a).
