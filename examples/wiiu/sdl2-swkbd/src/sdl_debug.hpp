/*
 * Print out all SDL2 events.
 */

#ifndef SDL_DEBUG_HPP
#define SDL_DEBUG_HPP

#include <SDL2/SDL_events.h>


namespace sdl::debug {

    void print(const SDL_Event& e);

} // namespace sdl::debug

#endif
