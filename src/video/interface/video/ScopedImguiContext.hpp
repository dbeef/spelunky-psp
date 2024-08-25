#pragma once

struct SDL_Window;

class ScopedImguiContext {
public:
    ScopedImguiContext(SDL_Window *window, void *gl_context);
    ~ScopedImguiContext();
};