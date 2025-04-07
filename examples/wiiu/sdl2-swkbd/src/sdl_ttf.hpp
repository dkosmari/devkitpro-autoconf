/*
 * A minimalist C++ wrapper for SDL2_ttf.
 */

#ifndef SDL_TTF_HPP
#define SDL_TTF_HPP

#include <SDL2/SDL_ttf.h>

#include "sdl.hpp"


namespace sdl::ttf {

    using sdl::literals::operator""_rgb;
    using sdl::literals::operator""_rgba;


    // RAII class for TTF_Init()/TTF_Quit()
    struct init {

        init();

        ~init()
            noexcept;

    };


    // RAII class for TTF_Font
    struct font {

        TTF_Font* ptr = nullptr;

        font()
            noexcept;

        font(SDL_RWops* src,
             bool take_src,
             int pt_size);

        ~font()
            noexcept;

        void
        create(SDL_RWops* src,
               bool take_src,
               int pt_size);

        void
        destroy()
            noexcept;

        bool
        is_valid()
            const noexcept;

        explicit
        operator bool()
            const noexcept;

        int
        height()
            const;

        surface
        render_blended(const char* text,
                       SDL_Color fg = 0xffffff_rgb);

        surface
        render_blended_wrapped(const char* text,
                               SDL_Color fg = 0xffffff_rgb,
                               Uint32 wrap_length = 0);


    };


} // namespace sdl::ttf

#endif
