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

Canvas::Canvas(const std::string& title, const int width, const int height)
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

auto Canvas::create(const int width, const int height) -> void
{
    auto create_drawable = [&]() -> void
    {
        const int      pos_x = SDL_WINDOWPOS_UNDEFINED;
        const int      pos_y = SDL_WINDOWPOS_UNDEFINED;
        const int      dim_w = width;
        const int      dim_h = height;
        const uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        if(bool(_drawable) == false) {
            _drawable.reset(::SDL_CreateWindow(_title.c_str(), pos_x, pos_y, dim_w, dim_h, flags));
        }
        if(bool(_drawable) == false) {
            throw std::runtime_error("SDL_CreateWindow() has failed");
        }
    };

    auto create_renderer = [&]() -> void
    {
        const int      index = -1;
        const uint32_t flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

        if(bool(_renderer) == false) {
            _renderer.reset(::SDL_CreateRenderer(_drawable.get(), index, flags));
        }
        if(bool(_renderer) == false) {
            throw std::runtime_error("SDL_CreateRenderer() has failed");
        }
    };

    auto create_underlay = [&]() -> void
    {
        if(bool(_underlay) == false) {
            _underlay.reset(::IMG_LoadTexture(_renderer.get(), "assets/underlay.png"));
        }
        if(bool(_underlay) == false) {
            throw std::runtime_error("SDL_CreateTextureFromSurface() has failed");
        }
    };

    auto create_overlay = [&]() -> void
    {
        if(bool(_overlay) == false) {
            _overlay.reset(::IMG_LoadTexture(_renderer.get(), "assets/overlay.png"));
        }
        if(bool(_overlay) == false) {
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
    auto do_clear = [&](RendererType* renderer, TextureType* texture) -> void
    {
        if(renderer != nullptr) {
            ::SDL_RenderClear(renderer);
            if(texture != nullptr) {
                ::SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            }
        }
    };

    return do_clear(_renderer.get(), (_show_underlay != false ? _underlay.get() : nullptr));
}

auto Canvas::present() -> void
{
    auto do_present = [&](RendererType* renderer, TextureType* texture) -> void
    {
        if(renderer != nullptr) {
            if(texture != nullptr) {
                ::SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            }
            ::SDL_RenderPresent(renderer);
        }
    };

    return do_present(_renderer.get(), (_show_overlay != false ? _overlay.get() : nullptr));
}

auto Canvas::color(const Col4i& color) -> void
{
    auto do_color = [&](RendererType* renderer) -> void
    {
        if(renderer != nullptr) {
            ::SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        }
    };

    return do_color(_renderer.get());
}

auto Canvas::line(int x1, int y1, int x2, int y2) -> void
{
    auto do_line = [&](RendererType* renderer) -> void
    {
        if(renderer != nullptr) {
            ::SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    };

    return do_line(_renderer.get());
}

auto Canvas::circle(int xc, int yc, int r) -> void
{
    auto do_circle = [&](RendererType* renderer) -> void
    {
        if(renderer != nullptr) {
            int x = 0;
            int y = r;
            int m = (5 - (4 * r));
            while(x <= y) {
                const int x1 = (xc - x);
                const int x2 = (xc + x);
                const int x3 = (xc - y);
                const int x4 = (xc + y);
                const int y1 = (yc - y);
                const int y2 = (yc + y);
                const int y3 = (yc - x);
                const int y4 = (yc + x);
                ::SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
                ::SDL_RenderDrawLine(renderer, x1, y2, x2, y2);
                ::SDL_RenderDrawLine(renderer, x3, y3, x4, y3);
                ::SDL_RenderDrawLine(renderer, x3, y4, x4, y4);
                if(m > 0) {
                    m -= (8 * --y);
                }
                m += ((8 * ++x) + 4);
            }
        }
    };

    return do_circle(_renderer.get());
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
