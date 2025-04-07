/*
 * A minimalist C++ wrapper for SDL2_ttf.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "sdl_ttf.hpp"


namespace sdl::ttf {

    init::init()
    {
        if (TTF_Init())
            throw error{"TTF_Init() failed"};
    }

    init::~init()
        noexcept
    {
        TTF_Quit();
    }


    font::font()
        noexcept = default;

    font::font(SDL_RWops* src,
               bool take_src,
               int pt_size)
    {
        create(src, take_src, pt_size);
    }

    font::~font()
        noexcept
    {
        destroy();
    }

    void
    font::create(SDL_RWops* src,
                 bool take_src,
                 int pt_size)
    {
        destroy();
        ptr = TTF_OpenFontRW(src, take_src, pt_size);
        if (!ptr)
            throw error{"TTF_OpenFontRW() failed"};
    }

    void
    font::destroy()
        noexcept
    {
        if (ptr) {
            TTF_CloseFont(ptr);
            ptr = nullptr;
        }
    }

    bool
    font::is_valid()
        const noexcept
    {
        return ptr;
    }

    font::operator bool()
        const noexcept
    {
        return is_valid();
    }

    int
    font::height()
        const
    {
        return TTF_FontHeight(ptr);
    }

    surface
    font::render_blended(const char* text,
                         SDL_Color fg)
    {
        auto s = TTF_RenderUTF8_Blended(ptr, text, fg);
        if (!s)
            throw error{"TTF_RenderUTF8_Blended() failed"};
        return surface{s};
    }

    surface
    font::render_blended_wrapped(const char* text,
                                 SDL_Color fg,
                                 Uint32 wrap_length)
    {
        auto s = TTF_RenderUTF8_Blended_Wrapped(ptr, text, fg, wrap_length);
        if (!s)
            throw error{"TTF_RenderUTF8_Blended_Wrapped() failed"};
        return surface{s};
    }

} // namespace sdl::ttf
