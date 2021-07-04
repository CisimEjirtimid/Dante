#pragma once
#include <SDL2/SDL_error.h>
#include <exception>

namespace dante::sdl
{
    class SDLException : public std::exception
    {
    public:
        const char* what() const override
        {
            return SDL_GetError();
        }
    };

    // Helper utility converts SDL API failures into exceptions.
    inline void ThrowIfFailed(SDL_bool value)
    {
        if (value == SDL_FALSE)
            throw SDLException{};
    }
}