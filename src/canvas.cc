/*
 * canvas.cc - Copyright (c) 2024-2025 - Olivier Poncet
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
#include "canvas.h"

// ---------------------------------------------------------------------------
// Canvas
// ---------------------------------------------------------------------------

Canvas::Canvas(const std::string& title, int width, int height)
    : _title(title)
    , _drawable(nullptr)
    , _renderer(nullptr)
    , _underlay(nullptr)
    , _overlay(nullptr)
    , _show_underlay(true)
    , _show_overlay(false)
{
    create(width, height);
}

auto Canvas::create(int width, int height) -> void
{
    auto create_drawable = [&]() -> void
    {
        const int      pos_x = SDL_WINDOWPOS_CENTERED;
        const int      pos_y = SDL_WINDOWPOS_CENTERED;
        const int      dim_w = width;
        const int      dim_h = height;
        const uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        if(_drawable.get() == nullptr) {
            _drawable.reset(::SDL_CreateWindow(_title.c_str(), pos_x, pos_y, dim_w, dim_h, flags));
        }
        if(_drawable.get() == nullptr) {
            throw std::runtime_error("SDL_CreateWindow() has failed");
        }
    };

    auto create_renderer = [&]() -> void
    {
        const int      index = -1;
        const uint32_t flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

        if(_renderer.get() == nullptr) {
            _renderer.reset(::SDL_CreateRenderer(_drawable.get(), index, flags));
        }
        if(_renderer.get() == nullptr) {
            throw std::runtime_error("SDL_CreateRenderer() has failed");
        }
    };

    auto create_underlay = [&]() -> void
    {
        const std::unique_ptr<SurfaceType> surface(::IMG_Load("assets/underlay.png"));

        if(surface.get() == nullptr) {
            throw std::runtime_error("IMG_Load() has failed");
        }
        if(_underlay.get() == nullptr) {
            _underlay.reset(::SDL_CreateTextureFromSurface(_renderer.get(), surface.get()));
        }
        if(_underlay.get() == nullptr) {
            throw std::runtime_error("SDL_CreateTextureFromSurface() has failed");
        }
    };

    auto create_overlay = [&]() -> void
    {
        const std::unique_ptr<SurfaceType> surface(::IMG_Load("assets/overlay.png"));

        if(surface.get() == nullptr) {
            throw std::runtime_error("IMG_Load() has failed");
        }
        if(_overlay.get() == nullptr) {
            _overlay.reset(::SDL_CreateTextureFromSurface(_renderer.get(), surface.get()));
        }
        if(_overlay.get() == nullptr) {
            throw std::runtime_error("SDL_CreateTextureFromSurface() has failed");
        }
    };

    auto do_create = [&]() -> void
    {
        create_drawable();
        create_renderer();
        create_underlay();
        create_overlay();
    };

    do_create();
}

auto Canvas::clear() -> void
{
    auto* renderer = _renderer.get();

    if(renderer != nullptr) {
        ::SDL_RenderClear(renderer);
        if(_show_underlay != false) {
            ::SDL_RenderCopy(renderer, _underlay.get(), nullptr, nullptr);
        }
    }
}

auto Canvas::present() -> void
{
    auto* renderer = _renderer.get();

    if(renderer != nullptr) {
        if(_show_overlay != false) {
            ::SDL_RenderCopy(renderer, _overlay.get(), nullptr, nullptr);
        }
        ::SDL_RenderPresent(renderer);
    }
}

auto Canvas::color(const Col4i& color) -> void
{
    auto* renderer = _renderer.get();

    if(renderer != nullptr) {
        ::SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
}

auto Canvas::line(int x1, int y1, int x2, int y2) -> void
{
    auto* renderer = _renderer.get();

    if(renderer != nullptr) {
        ::SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

auto Canvas::circle(int xc, int yc, int r) -> void
{
    auto* renderer = _renderer.get();

    if(renderer != nullptr) {
        int x = 0;
        int y = r;
        int m = (5 - (4 * r));
        while(x <= y) {
            ::SDL_RenderDrawLine(renderer, (xc + x), (yc + y), (xc - x), (yc + y));
            ::SDL_RenderDrawLine(renderer, (xc + y), (yc + x), (xc - y), (yc + x));
            ::SDL_RenderDrawLine(renderer, (xc - x), (yc - y), (xc + x), (yc - y));
            ::SDL_RenderDrawLine(renderer, (xc - y), (yc - x), (xc + y), (yc - x));
            if(m > 0) {
                --y;
                m -= (8 * y);
            }
            ++x;
            m += ((8 * x) + 4);
        }
    }
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
