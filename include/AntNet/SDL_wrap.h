//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_SDL_WRAP_H
#define ANTNET_SDL_WRAP_H

#include <utility>
#include <system_error>
#include <memory>
#include <SDL_video.h>

namespace sdl2 {

    template<typename Creator, typename Destructor, typename... Arguments>
    auto make_resource(Creator c, Destructor d, Arguments&&... args)
    {
        auto r = c(std::forward<Arguments>(args)...);
        if (!r) { throw std::system_error(errno, std::generic_category()); }
        return std::unique_ptr<std::decay_t<decltype(*r)>, decltype(d)>(r, d);
    }

    using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

    inline window_ptr_t make_window(const char* title,
                                    int x, int y, int w, int h, Uint32 flags) {
        return make_resource(SDL_CreateWindow, SDL_DestroyWindow,
                             title, x, y, w, h, flags);
    }



}

#endif //ANTNET_SDL_WRAP_H