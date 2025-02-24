/*
 * application.cc - Copyright (c) 2024-2025 - Olivier Poncet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <cmath>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "application.h"

// ---------------------------------------------------------------------------
// <anonymous>::utilities
// ---------------------------------------------------------------------------

namespace {

inline auto clampf(float val, float min, float max) -> float
{
    if(val < min) {
        val = min;
    }
    if(val > max) {
        val = max;
    }
    return val;
}

}

// ---------------------------------------------------------------------------
// Application
// ---------------------------------------------------------------------------

Application::Application(const std::string& title)
    : _title(title)
    , _ptime(0)
    , _ctime(0)
    , _dtime(0.0f)
    , _quit(false)
{
    constexpr uint32_t flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

    if(::SDL_Init(flags) != 0) {
        throw std::runtime_error("SDL_Init() has failed");
    }
    else {
        _ptime  = ::SDL_GetTicks();
        _ctime  = ::SDL_GetTicks();
        _dtime  = ::clampf((float(_ctime - _ptime) / 1000.0f), 0.0f, 0.1f);
    }
}

Application::~Application()
{
    ::SDL_Quit();
}

void Application::main()
{
    auto get_ticks = [&]() -> bool
    {
        const uint32_t ptime = _ctime;
        const uint32_t ctime = ::SDL_GetTicks();
        const bool     valid = (ctime != ptime);

        if(valid != false) {
            _ptime = ptime;
            _ctime = ctime;
            _dtime = ::clampf((float(_ctime - _ptime) / 1000.0f), 0.0f, 0.1f);
        }
        else {
            _dtime = 0.0f;
        }
        return valid;
    };

    auto poll_events = [&]() -> bool
    {
        EventType event;
        while(::SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    on_quit(event.quit);
                    break;
                case SDL_WINDOWEVENT:
                    on_window(event.window);
                    break;
                case SDL_KEYDOWN:
                    on_key_press(event.key);
                    break;
                case SDL_KEYUP:
                    on_key_release(event.key);
                    break;
                case SDL_MOUSEMOTION:
                    on_mouse_motion(event.motion);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    on_mouse_button_press(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    on_mouse_button_release(event.button);
                    break;
                case SDL_MOUSEWHEEL:
                    on_mouse_wheel(event.wheel);
                    break;
                default:
                    break;
            }
        }
        return running();
    };

#ifdef __EMSCRIPTEN__
    auto do_main = [&]() -> void
    {
        if(poll_events()) {
            if(get_ticks()) {
                update();
                render();
            }
        }
    };
#else
    auto do_main = [&]() -> void
    {
        while(poll_events()) {
            if(get_ticks()) {
                update();
                render();
            }
        }
    };
#endif

    return do_main();
}

void Application::quit()
{
    if(_quit == false) {
        _quit = true;
        shutdown();
    }
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
