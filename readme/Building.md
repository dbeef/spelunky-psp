# Building

### Notes

* All assets are compiled inside the final artifact (ELF/EXE/PBP), so it can be moved around the filesystem and ran as it is.

### Linux

To install dependencies:

```bash
sudo apt install libsdl1.2-dev
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
