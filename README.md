## Spelunky® remake for the Sony PSP.

[![`Build status`](https://travis-ci.org/dbeef/spelunky-psp.svg?branch=master)](https://travis-ci.org/dbeef/spelunky-psp)

### How?

Original Spelunky classic was written using GameMaker, properietary tool for creating games. License, under
Derek Yu, its autor, released sources says clearly than one can use them, combined with assets freely as long
as it is not for one's profit.

This is fantastic, but in the same time limiting, as an eager programmer can't build its favorite game untill 
GameMaker declares it will now support some retro console (which is very unlikely to happen).
  
But, as there are good people providing and maintaining free, homebrew toolchains 
(i.e [devkitPro](https://github.com/devkitPro) organization or [pspdev group](https://github.com/pspdev)) it is still
possible to reuse assets and use provided sources as a point of reference, when rewritting original sources to C++,
using platform-specific rendering API.  
  
And this is what this project is about.
  
#### See my other Spelunky-related projects:
 
* [Spelunky-DS](https://github.com/dbeef/spelunky-ds) 
* [Spelunky live wallpaper for Android](https://github.com/dbeef/SpelunkyLiveWallpaper) 

---

### State of the project

This is my second Spelunky project, as I started with Spelunky-NDS. 
Unfortunately, as Nintendo DS rendering relies on a very platform-specific code 
(i.e writing to certain memory addresses via DMA to update sprites), I could not reuse it.   
As it introduces a problem, rewriting it for PSP creates new, fascinating possibilities. You see, PSP supports
very early, primitive subset of a commonly adopted rendering API - OpenGL.

Once renderer is rewritten to utilize OpenGL-ES 1.0, Spelunky can be ran pretty much everywhere.
Not only PSP, but PS Vita, Nintendo Switch and ordinary PC or Mac. Also mobiles - Android and iOS.

Just like Doom is ported to all those weird devices - calculators, printers, digital cameras, ATM's,
smart watches and so on, Spelunky classic would be playable everywhere, where rudimentary subset of OpenGL settled.

I split creating rendering context into platform-specific directories (under `/src/video`). 
There, depending on platform, I use glad for obtaining OpenGL 1.3 context on desktops or OpenGL-ES 1.0 on PSP. 
Those are 1:1 subsets of OpenGL in terms of functions they provide, so code written and run on desktop should 
compile, link and run successfuly on a PSP. 

Using [glad](https://github.com/Dav1dde/glad) as an abstraction layer for OpenGL is deliberate. 
This way I am not depending on system headers that would provide me OpenGL functions - glad headers are used everywhere
where OpenGL is called, so I only need to make sure that glad loads pointers to subsequent functions successfuly.

On both platforms, glad is feeded pointers to OpenGL functions, in runtime, by SDL. 

### Building

Currently, build scripts that are placed under `/scripts` support building on Linux and PSP. 
Linux one produces ordinary `elf` executable, PSP one produces `.EBP` file that can be placed on SD card and ran on a 
real device or passed to an emulator like PPSSPP.

##### Linux

The only dependency is SDL 1.2.15.

##### PSP

Building requires free PSP toolchain, that too needs to be build first:
https://github.com/pspdev/psptoolchain 

After building core toolchain, its scripts will build and install a few libraries including SDL 1.2.15, 
hence no additional action is required. 

After toolchain's build scripts set up environment variables (make sure `echo $PSPDEV` returns your PSP toolchain's 
root directory), you are ready to cross compile to PSP via `/scripts/config-psp.sh` and `/scripts/build-psp.sh`.

#### Windows

The only dependency is SDL 1.2.15. Download the [Visual Studio development files](https://www.libsdl.org/release/SDL-devel-1.2.15-VC.zip) to start your journey.
Alternatively, from project's root directory run
```powershell
.\scripts\dependencies-install.ps1 [-DepsDirectory <DIR>] # deps is default
.\scripts\config-windows.ps1 [-DepsDirectory <DIR>] # deps is default
```
This will configure the required SDL dependency and effectively set you up.


### Contributing

##### To-be-done list

* Establish CI, doing:
    * license scanning
* Provide issue template on Github
* Create resource-compiler subproject, outputting executable that would convert given asset (i.e image) to header file. Add automatic invoking resource-compiler to CMake every time assets were touched.
* Extract level-generating code from Spelunky DS, making it as renderer-agnostic as possible, for potential reuse between Spelunky DS / Spelunky PSP.
* Write tile renderer in OpenGL 1.0 / OpenGL ES 1.3
* Multi-platform stdout/stderr logger. I was considering spdlog, however because of stdlib restrictions using psp-g++, it fails to compile. Main requirement is for logged messages to be visible in PPSSPP's (emulator) console output.

##### Before you contribute

* Make sure you use Unix line endings (LF) in your editor.
* Use spaces, not tabs.
* When posting a pull request, check if your change does not introduce any regression - test your feature on 
platforms that are supported.
* When creating an issue, use provided template.

### Copyrights

Full license text can be accessed under this link:

https://github.com/dbeef/spelunky-psp/blob/master/LICENSE

Briefly, as for my code, I distribute it under GNU General Public License, as for Spelunky game assets, 
I use them under Spelunky® User License v1.1b.

PSP is a trademark of Sony and is used purely for descriptive purposes. This game is not affiliated with Sony.
