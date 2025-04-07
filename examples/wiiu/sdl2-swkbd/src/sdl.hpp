/*
 * A minimalist C++ wrapper for SDL2.
 */

#ifndef SDL_HPP
#define SDL_HPP

#include <optional>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>


namespace sdl {

    inline
    namespace literals {

        inline
        namespace color_literals {


            constexpr
            SDL_Color
            operator ""_rgb(unsigned long long rgb)
                noexcept
            {
                return {
                    .r = static_cast<Uint8>(rgb >> 16u),
                    .g = static_cast<Uint8>(rgb >>  8u),
                    .b = static_cast<Uint8>(rgb >>  0u),
                    .a = 0xff
                };
            }

            constexpr
            SDL_Color
            operator ""_rgba(unsigned long long rgba)
                noexcept
            {
                return {
                    .r = static_cast<Uint8>(rgba >> 24u),
                    .g = static_cast<Uint8>(rgba >> 16u),
                    .b = static_cast<Uint8>(rgba >>  8u),
                    .a = static_cast<Uint8>(rgba >>  0u)
                };
            }

        } // namespace color_literals
    } // namespace literals


    struct vec2 : SDL_Point {

        float
        length()
            const noexcept;

        int
        length2()
            const noexcept;

    };

    vec2
    operator +(const vec2& a, const vec2& b)
        noexcept;

    vec2
    operator -(const vec2& a, const vec2& b)
        noexcept;

    vec2
    operator /(const vec2& a, int b)
        noexcept;



    struct rect : SDL_Rect {

        vec2
        top_left()
            const noexcept;

        vec2
        center()
            const noexcept;

    };



    // Convenience exception class that obtains description from `SDL_GetError()`.
    struct error : std::runtime_error{

        error(const std::string& msg);

    };


    // RAII class for SDL_Init()/SDL_Quit()
    struct init {

        bool initialized = false;

        init()
            noexcept;

        init(Uint32 flags);

        ~init()
            noexcept;

    };


    // RAII class for SDL_Window
    class window {

        SDL_Window* ptr = nullptr;

        void
        link_this()
            noexcept;

    public:

        window() noexcept;

        window(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags);

        // move constructor
        window(window&& other)
            noexcept;

        // move assignment
        window&
        operator =(window&& other)
            noexcept;

        ~window()
            noexcept;

        void
        create(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags);

        void
        destroy()
            noexcept;

        SDL_Window*
        data()
            noexcept;

        const SDL_Window*
        data()
            const noexcept;

        vec2
        size()
            const noexcept;

        int
        width()
            const noexcept;

        int
        height()
            const noexcept;

        Uint32
        get_flags()
            const noexcept;

        void
        focus()
            noexcept;

        void
        raise()
            noexcept;

        bool
        is_screen_keyboard_shown()
            noexcept;

        static
        window*
        get_wrapper(SDL_Window* win)
            noexcept;

    };


    // RAII class for SDL_Surface
    class surface {

        SDL_Surface* ptr = nullptr;

    public:

        surface()
            noexcept;

        explicit
        surface(SDL_Surface* raw_ptr)
            noexcept;

        // move constructor
        surface(surface&& other)
            noexcept;

        // move assignment
        surface&
        operator =(surface&& other)
            noexcept;

        ~surface()
            noexcept;

        void
        create(SDL_Surface* raw_ptr);

        void
        destroy()
            noexcept;

        bool
        is_valid()
            const noexcept;

        explicit
        operator bool()
            const noexcept;

        SDL_Surface*
        data()
            noexcept;

        const SDL_Surface*
        data()
            const noexcept;

        vec2
        size()
            const noexcept;

    };


    // RAII class for SDL_Texture
    class texture {

        SDL_Texture* ptr = nullptr;

    public:

        texture()
            noexcept;

        explicit
        texture(SDL_Texture* raw_ptr)
            noexcept;

        // move constructor
        texture(texture&& other)
            noexcept;

        // move assignment
        texture&
        operator =(texture&& other)
            noexcept;

        ~texture()
            noexcept;

        void
        destroy()
            noexcept;

        bool
        is_valid()
            const noexcept;

        explicit
        operator bool()
            const noexcept;

        SDL_Texture*
        data()
            noexcept;

        const SDL_Texture*
        data()
            const noexcept;

        vec2
        size()
            const;

        int
        width()
            const;

        int
        height()
            const;

    };


    // RAII class for SDL_Renderer
    class renderer {

        SDL_Renderer* ptr = nullptr;

    public:

        renderer()
            noexcept;

        renderer(window& win,
                 int index,
                 Uint32 flags);

        ~renderer()
            noexcept;

        void
        create(window& win,
               int index,
               Uint32 flags);

        void
        destroy()
            noexcept;

        window*
        get_window();

        SDL_Renderer*
        data()
            noexcept;


        const SDL_Renderer*
        data()
            const noexcept;

        void
        clear();

        void
        set_blend_mode(SDL_BlendMode mode);

        void
        set_color(Uint8 r,
                  Uint8 g,
                  Uint8 b,
                  Uint8 a = 0xff);

        void
        set_color(SDL_Color c);

        void
        set_logical_size(const vec2& size);

        void
        set_logical_size(int width, int height);

        void
        present();

        texture
        create_texture(const surface& surf);

        void
        draw_point(int x, int y);

        void
        draw_rect(const SDL_Rect& rect);

        void
        draw_rect(int x, int y, int w, int h);

        void
        fill_rect(const SDL_Rect& rect);

        void
        fill_rect(int x, int y, int w, int h);

        void
        copy(const texture& tex,
             const std::optional<SDL_Rect>& src_rect = {});

        void
        copy(const texture& tex,
             const std::optional<SDL_Rect>& src_rect,
             const SDL_Rect& dst_rect);

        void
        copy(const texture& tex,
             const std::optional<SDL_Rect>& src_rect,
             const SDL_FRect& dst_rect);

    };

} // namespace sdl

#endif
