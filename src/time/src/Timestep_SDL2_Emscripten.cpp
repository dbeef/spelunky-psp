#include "time/Timestep.hpp"
#include <SDL_timer.h>

Timestamp Timestep::delay() const
{
  const auto delta_ms = (_end - _start);

  if (delta_ms < _min_delay_ms)
  {
    // Note - No real sleep done here - emscripten already handles
    //        running game loop in 60fps.
    return _min_delay_ms;
  }
  return delta_ms;
}

void Timestep::mark_start()
{
  _start = SDL_GetTicks();
}

void Timestep::mark_end()
{
  _end = SDL_GetTicks();
}
