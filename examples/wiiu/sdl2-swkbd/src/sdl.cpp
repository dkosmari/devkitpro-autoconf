/*
 * A minimalist C++ wrapper for SDL2.
 */

#include <cmath>

#include <SDL2/SDL.h>

#include "sdl.hpp"


using namespace std::literals;


namespace sdl {

    float
    vec2::length()
        const noexcept
    {
        return std::sqrt(length2());
    }

    int
    vec2::length2()
        const noexcept
    {
        return x * x + y * y;
    }

    vec2
    operator +(const vec2& a,
               const vec2& b)
        noexcept
    {
        return {a.x + b.x, a.y + b.y};
    }

    vec2
    operator -(const vec2& a,
               const vec2& b)
        noexcept
    {
        return {a.x - b.x, a.y - b.y};
    }

    vec2
    operator /(const vec2& a,
               int b)
        noexcept
    {
        return {a.x / b, a.y / b};
    }



    vec2
    rect::top_left()
        const noexcept
    {
        return {x, y};
    }

    vec2
    rect::center()
        const noexcept
    {
        return { x + w / 2, y + h / 2 };
    }



    error::error(const std::string& msg) :
        std::runtime_error{msg + ": "s + SDL_GetError()}
    {}


    init::init()
        noexcept = default;

    init::init(Uint32 flags)
    {
        if (SDL_Init(flags))
            throw error{"SDL_Init() failed"};
        initialized = true;
    }

    init::~init()
        noexcept
    {
        if (initialized)
            SDL_Quit();
    }


    void
    window::link_this()
        noexcept
    {
        if (ptr)
            SDL_SetWindowData(ptr, "wrapper", this);
    }

    window::window()
        noexcept = default;

    window::window(const char* title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags)
    {
        create(title, x, y, w, h, flags);
    }

    window::window(window&& other)
        noexcept :
        ptr{other.ptr}
    {
        if (other.ptr) {
            other.ptr = nullptr;
            link_this();
        }
    }

    window&
    window::operator =(window&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
            link_this();
        }
        return *this;
    }

    window::~window()
        noexcept
    {
        destroy();
    }

    void
    window::create(const char* title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags)
    {
        destroy();
        ptr = SDL_CreateWindow(title, x, y, w, h, flags);
        if (!ptr)
            throw error{"SDL_CreateWindow() failed"};
        link_this();
    }

    void
    window::destroy()
        noexcept
    {
        if (ptr) {
            SDL_DestroyWindow(ptr);
            ptr = nullptr;
        }
    }

    SDL_Window*
    window::data()
        noexcept
    {
        return ptr;
    }

    const SDL_Window*
    window::data()
        const noexcept
    {
        return ptr;
    }

    vec2
    window::size()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowSize(ptr, &result.x, &result.y);
        return result;
    }

    int
    window::width()
        const noexcept
    {
        return size().x;
    }

    int
    window::height()
        const noexcept
    {
        return size().y;
    }

    Uint32
    window::get_flags()
        const noexcept
    {
        return SDL_GetWindowFlags(const_cast<SDL_Window*>(ptr));
    }

    void
    window::focus()
        noexcept
    {
        SDL_SetWindowInputFocus(ptr);
    }

    void
    window::raise()
        noexcept
    {
        SDL_RaiseWindow(ptr);
    }

    bool
    window::is_screen_keyboard_shown()
        noexcept
    {
        return SDL_IsScreenKeyboardShown(ptr);
    }

    window*
    window::get_wrapper(SDL_Window* win)
        noexcept
    {
        if (!win)
            return nullptr;
        return reinterpret_cast<window*>(SDL_GetWindowData(win, "wrapper"));
    }



    surface::surface()
        noexcept = default;

    surface::surface(SDL_Surface* raw_ptr)
        noexcept :
        ptr{raw_ptr}
    {}

    surface::surface(surface&& other)
        noexcept
    {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    surface&
    surface::operator =(surface&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    surface::~surface()
        noexcept
    {
        destroy();
    }

    void
    surface::create(SDL_Surface* raw_ptr)
    {
        destroy();
        ptr = raw_ptr;
    }

    void
    surface::destroy()
        noexcept
    {
        if (ptr) {
            SDL_FreeSurface(ptr);
            ptr = nullptr;
        }
    }

    bool
    surface::is_valid()
        const noexcept
    {
        return ptr;
    }

    surface::operator bool()
        const noexcept
    {
        return is_valid();
    }

    SDL_Surface*
    surface::data()
        noexcept
    {
        return ptr;
    }


    const SDL_Surface*
    surface::data()
        const noexcept
    {
        return ptr;
    }

    vec2
    surface::size()
        const noexcept
    {
        if (!ptr)
            return {0, 0};
        return {ptr->w, ptr->h};
    }



    texture::texture()
        noexcept = default;

    texture::texture(SDL_Texture* raw_ptr)
        noexcept :
        ptr{raw_ptr}
    {}

    // move constructor
    texture::texture(texture&& other)
        noexcept
    {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    // move assignment
    texture&
    texture::operator =(texture&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    texture::~texture()
        noexcept
    {
        destroy();
    }

    void
    texture::destroy()
        noexcept
    {
        if (ptr) {
            SDL_DestroyTexture(ptr);
            ptr = nullptr;
        }
    }

    bool
    texture::is_valid()
        const noexcept
    {
        return ptr;
    }

    texture::operator bool()
        const noexcept
    {
        return is_valid();
    }

    SDL_Texture*
    texture::data()
        noexcept
    {
        return ptr;
    }

    const SDL_Texture*
    texture::data()
        const noexcept
    {
        return ptr;
    }

    vec2
    texture::size()
        const
    {
        vec2 result;
        if (SDL_QueryTexture(ptr, nullptr, nullptr, &result.x, &result.y))
            throw error{"SDL_QueryTexture() failed"};
        return result;
    }

    int
    texture::width()
        const
    {
        return size().x;
    }

    int
    texture::height()
        const
    {
        return size().y;
    }


    renderer::renderer()
        noexcept = default;

    renderer::renderer(window& win,
                       int index,
                       Uint32 flags)
    {
        create(win, index, flags);
    }

    renderer::~renderer()
        noexcept
    {
        destroy();
    }

    void
    renderer::create(window& win,
                     int index,
                     Uint32 flags)
    {
        destroy();
        ptr = SDL_CreateRenderer(win.data(), index, flags);
        if (!ptr)
            throw error{"SDL_CreateRenderer() failed"};
    }

    void
    renderer::destroy()
        noexcept
    {
        if (ptr) {
            SDL_DestroyRenderer(ptr);
            ptr = nullptr;
        }
    }

    window*
    renderer::get_window()
    {
        auto raw_window = SDL_RenderGetWindow(ptr);
        if (!raw_window)
            throw error{"SDL_RenderGetWindow() failed"};
        return window::get_wrapper(raw_window);
    }

    SDL_Renderer*
    renderer::data()
        noexcept
    {
        return ptr;
    }

    const SDL_Renderer*
    renderer::data()
        const noexcept
    {
        return ptr;
    }

    void
    renderer::clear()
    {
        if (SDL_RenderClear(ptr))
            throw error{"SDL_RenderClear() failed"};
    }

    void
    renderer::set_blend_mode(SDL_BlendMode mode)
    {
        if (SDL_SetRenderDrawBlendMode(ptr, mode))
            throw error{"SDL_SetRenderDrawBlendMode() failed"};
    }

    void
    renderer::set_color(Uint8 r,
                        Uint8 g,
                        Uint8 b,
                        Uint8 a)
    {
        if (SDL_SetRenderDrawColor(ptr, r, g, b, a))
            throw error{"SDL_SetRenderDrawColor() failed"};
    }

    void
    renderer::set_color(SDL_Color c)
    {
        set_color(c.r, c.g, c.b, c.a);
    }

    void
    renderer::set_logical_size(const vec2& size)
    {
        if (SDL_RenderSetLogicalSize(ptr, size.x, size.y))
            throw error{"SDL_RenderSetLogicalSize() failed"};
    }

    void
    renderer::set_logical_size(int width,
                               int height)
    {
        set_logical_size({width, height});
    }

    void
    renderer::present()
    {
        SDL_RenderPresent(ptr);
    }

    texture
    renderer::create_texture(const surface& surf)
    {
        auto raw_tex = SDL_CreateTextureFromSurface(ptr, const_cast<SDL_Surface*>(surf.data()));
        if (!raw_tex)
            throw error{"SDL_CreateTextureFromSurface() failed"};
        return texture{raw_tex};
    }

    void
    renderer::draw_point(int x, int y)
    {
        if (SDL_RenderDrawPoint(ptr, x, y))
            throw error{"SDL_RenderDrawPoint() failed"};
    }

    void
    renderer::draw_rect(const SDL_Rect& rect)
    {
        if (SDL_RenderDrawRect(ptr, &rect))
            throw error{"SDL_RenderDrawRect() failed"};
    }

    void
    renderer::draw_rect(int x, int y, int w, int h)
    {
        draw_rect({x, y, w, h});
    }

    void
    renderer::fill_rect(const SDL_Rect& rect)
    {
        if (SDL_RenderFillRect(ptr, &rect))
            throw error{"SDL_RenderFillRect() failed"};
    }

    void
    renderer::fill_rect(int x, int y, int w, int h)
    {
        fill_rect({x, y, w, h});
    }

    void
    renderer::copy(const texture& tex,
                   const std::optional<SDL_Rect>& src_rect)
    {
        if (SDL_RenderCopy(ptr,
                           const_cast<SDL_Texture*>(tex.data()),
                           src_rect ? &*src_rect : nullptr,
                           nullptr))
            throw error{"SDL_RenderCopy() failed"};
    }

    void
    renderer::copy(const texture& tex,
                   const std::optional<SDL_Rect>& src_rect,
                   const SDL_Rect& dst_rect)
    {
        if (SDL_RenderCopy(ptr,
                           const_cast<SDL_Texture*>(tex.data()),
                           src_rect ? &*src_rect : nullptr,
                           &dst_rect))
            throw error{"SDL_RenderCopy() failed"};
    }

    void
    renderer::copy(const texture& tex,
                   const std::optional<SDL_Rect>& src_rect,
                   const SDL_FRect& dst_rect)
    {
        if (SDL_RenderCopyF(ptr, const_cast<SDL_Texture*>(tex.data()),
                            src_rect ? &*src_rect : nullptr,
                            &dst_rect))
            throw error{"SDL_RenderCopyF() failed"};
    }

} // namespace sdl
