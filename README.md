## Spelunky® Classic remake for the Sony PSP [![`Build status`](https://github.com/dbeef/spelunky-psp/actions/workflows/build.yml/badge.svg)](https://github.com/dbeef/spelunky-psp/actions/workflows/build.yml)

*Rewriting the game from scratch in C++17 and OpenGL 1.3 / ES 1.0, using CMake as a build system and SDL as a part of a platform abstraction layer, making it a truly multiplatform experience.*

![`Playing on PSP`](readme/PSP.png)

### How?

Original Spelunky classic was written using GameMaker, properietary tool for creating games. License, under
Derek Yu, its autor, released sources says clearly than one can use them, combined with assets freely as long
as it is not for one's profit.

This is fantastic, but in the same time limiting, as an eager programmer can't build its favorite game untill 
GameMaker declares it will now support some retro console (which is very unlikely to happen).
  
But, as there are good people providing and maintaining free, homebrew toolchains 
(i.e [devkitPro](https://github.com/devkitPro) organization or [pspdev group](https://github.com/pspdev)) it is still
possible to reuse assets and use provided sources as a point of reference, when rewriting original sources to C++,
using commonly adopted, multi-platform rendering API - OpenGL.
  
Currently supported platforms are:

* PSP
* Android
* Linux
* Windows
* macOS

With successful attempts to run it on such devices as Raspberry PI 3, Orange PI One, Samsung Note III.

### See the development videos I put on Youtube:

* [Spelunky-PSP devlog #1](https://www.youtube.com/watch?v=2v40JiyYs-M)
* [Spelunky-PSP devlog #2](https://www.youtube.com/watch?v=UDkkU8J8Ejk)
* [Spelunky-PSP devlog #3](https://www.youtube.com/watch?v=mp09ItJAMJo)
* [Software-implemented OpenGL - SpelunkyPSP on PocketGo case study](https://www.youtube.com/watch?v=NbYRNmjxoR8)
* [Spelunky-PSP devlog #4](https://www.youtube.com/watch?v=GRuf4z4yyHU)
* [PSP HDC 2021 - Spelunky PSP - First year of development](https://www.youtube.com/watch?v=45JVr3RiMQE)
* [Spelunky-PSP - 3D accelerometer experiment on Android](https://www.youtube.com/watch?v=wVxGq6nK9GA)
* [Spelunky-PSP devlog #5](https://www.youtube.com/watch?v=iJq1OIfMNVw)
  
### See my other Spelunky-related projects:
 
* [Spelunky-DS](https://github.com/dbeef/spelunky-ds) 
* [Spelunky live wallpaper for Android](https://github.com/dbeef/SpelunkyLiveWallpaper) 

### Documentation

* [Introduction](readme/Introduction.md)
* [Dependency matrix](readme/Dependencies.md)
* [Building](readme/Building.md) 

### Copyrights

Full license text can be accessed under this link:

https://github.com/dbeef/spelunky-psp/blob/master/LICENSE

Briefly, as for my (Daniel 'dbeef' Zalega) code, I distribute it under GNU General Public License, as for Spelunky
Classic game assets, I use and distribute them under Spelunky® User License v1.1b.

PSP is a trademark of Sony and is used purely for descriptive purposes. This game is not affiliated with Sony.
