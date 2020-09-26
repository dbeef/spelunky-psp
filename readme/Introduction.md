# Introduction

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
There, depending on platform, I use glad for obtaining OpenGL 1.3 context on desktops or OpenGL-ES 1.0 on PSP and Android. 
Those are (almost) 1:1 subsets of OpenGL in terms of functions they provide, so code written and run on desktop should 
compile, link and run successfuly on a PSP. 

Using [glad](https://github.com/Dav1dde/glad) as an abstraction layer for OpenGL is deliberate. 
This way I am not depending on system headers that would provide me OpenGL functions - glad headers are used everywhere
where OpenGL is called, so I only need to make sure that glad loads pointers to subsequent functions successfuly.

On all platforms, glad is feeded pointers to OpenGL functions, in runtime, by SDL. 
