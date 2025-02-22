/*
 * bouncing-ball.cc - Copyright (c) 2024-2025 - Olivier Poncet
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
#include "globals.h"
#include "bouncing-ball.h"

// ---------------------------------------------------------------------------
// BouncingBall
// ---------------------------------------------------------------------------

BouncingBall::BouncingBall(const int width, const int height)
    : Application("Bouncing Ball")
    , _canvas(nullptr)
    , _poly(nullptr)
    , _ball(nullptr)
    , _size()
    , _center()
    , _color(0.12f, 0.12f, 0.12f)
{
    create_canvas(width, height);
    create_poly();
    create_ball();
}

auto BouncingBall::create_canvas(int width, int height) -> void
{
    if(_canvas.get() == nullptr) {
        _canvas = std::make_unique<Canvas>(_title, width, height);
        _size   = Vec2f(width, height);
        _center = Pos2f(Pos2f() + (_size / 2.0f));
    }
}

auto BouncingBall::create_poly() -> void
{
    const int   poly_vertices = Globals::poly_vertices;
    const float poly_radius   = Globals::poly_radius;
    const float poly_omega    = Globals::poly_omega;
    const float poly_friction = Globals::poly_friction;
    const float poly_gravity  = Globals::poly_gravity;

    _poly = std::make_unique<Poly>(_center, poly_vertices, poly_radius);
    _poly->set_omega(poly_omega);
    _poly->set_friction(Vec2f(poly_friction, 0.0f));
    _poly->set_gravity(Vec2f(0.0f, poly_gravity));
}

auto BouncingBall::create_ball() -> void
{
    const float ball_radius   = Globals::ball_radius;
    const float ball_friction = Globals::ball_friction;
    const float ball_gravity  = Globals::ball_gravity;

    _ball = std::make_unique<Ball>(_center, ball_radius);
    _ball->set_friction(Vec2f(ball_friction, ball_friction));
    _ball->set_gravity(Vec2f(0.0f, ball_gravity));
}

auto BouncingBall::toggle_underlay() -> void
{
    _canvas->toggle_underlay();
}

auto BouncingBall::toggle_overlay() -> void
{
    _canvas->toggle_overlay();
}

auto BouncingBall::set_poly_vertices(int poly_vertices) -> void
{
    Globals::set_poly_vertices(poly_vertices);

    create_poly();
}

auto BouncingBall::set_poly_radius(float poly_radius) -> void
{
    Globals::set_poly_radius(poly_radius);

    _poly->set_radius(Globals::poly_radius);
}

auto BouncingBall::set_poly_omega(float poly_omega) -> void
{
    Globals::set_poly_omega(poly_omega);

    _poly->set_omega(Globals::poly_omega);
}

auto BouncingBall::set_ball_radius(float ball_radius) -> void
{
    Globals::set_ball_radius(ball_radius);

    _ball->set_radius(Globals::ball_radius);
}

auto BouncingBall::resized(int width, int height) -> void
{
    const Vec2f size(width, height);
    const Pos2f center(Pos2f() + (size / 2.0f));
    const Vec2f delta(center - _center);

    _size   = size;
    _center = center;
    _poly->set_position(_poly->position() + delta);
    _ball->set_position(_ball->position() + delta);
}

auto BouncingBall::update() -> void
{
    auto& poly(*_poly);
    auto& ball(*_ball);

    poly.update(_dtime);
    ball.update(_dtime);
    ball.collide(poly);
}

auto BouncingBall::render() -> void
{
    auto& canvas(*_canvas);
    auto& poly(*_poly);
    auto& ball(*_ball);

    canvas.color(_color);
    canvas.clear();
    poly.render(canvas);
    ball.render(canvas);
    canvas.present();
}

auto BouncingBall::shutdown() -> void
{
}

auto BouncingBall::on_window(const WindowEventType& event) -> void
{
    switch(event.event) {
        case SDL_WINDOWEVENT_RESIZED:
            resized(event.data1, event.data2);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            quit();
            break;
        default:
            break;
    }
}

auto BouncingBall::on_key_press(const KeyboardEventType& event) -> void
{
    const auto mods = ::SDL_GetModState();

    auto process_event = [&]() -> void
    {
        switch(event.keysym.sym) {
            case SDLK_h:
                toggle_overlay();
                break;
            case SDLK_u:
                toggle_underlay();
                break;
            case SDLK_r:
                create_poly();
                create_ball();
                break;
            case SDLK_q:
                quit();
                break;
            case SDLK_UP:
                set_poly_vertices(Globals::poly_vertices + 1);
                break;
            case SDLK_DOWN:
                set_poly_vertices(Globals::poly_vertices - 1);
                break;
            case SDLK_LEFT:
                {
                    const float value = (1.5f * _dtime);
                    if(mods & (KMOD_LSHIFT | KMOD_RSHIFT)) {
                        set_poly_omega(_poly->omega() - (2.0f * value));
                    }
                    else {
                        set_poly_omega(_poly->omega() - (1.0f * value));
                    }
                }
                break;
            case SDLK_RIGHT:
                {
                    const float value = (1.5f * _dtime);
                    if(mods & (KMOD_LSHIFT | KMOD_RSHIFT)) {
                        set_poly_omega(_poly->omega() + (2.0f * value));
                    }
                    else {
                        set_poly_omega(_poly->omega() + (1.0f * value));
                    }
                }
                break;
            case SDLK_ESCAPE:
                quit();
                break;
            default:
                break;
        }
    };

    return process_event();
}

auto BouncingBall::on_key_release(const KeyboardEventType& event) -> void
{
}

auto BouncingBall::on_mouse_motion(const MouseMotionEventType& event) -> void
{
    const auto mods = ::SDL_GetModState();

    if((event.state & SDL_BUTTON_LMASK) != 0) {
        auto& poly(*_poly);
        auto& ball(*_ball);
        if(mods & (KMOD_LSHIFT | KMOD_RSHIFT)) {
            const float pos_x = float(event.x);
            const float pos_y = float(event.y);
            const float vel_x = float(event.xrel * 50);
            const float vel_y = float(event.yrel * 50);
            ball.set_position(Pos2f(pos_x, pos_y));
            ball.set_velocity(Vec2f(vel_x, vel_y));
            ball.set_frozen(true);
            poly.set_frozen(false);
        }
        else {
            const float pos_x = float(event.x);
            const float pos_y = float(event.y);
            const float vel_x = float(0);
            const float vel_y = float(0);
            poly.set_position(Pos2f(pos_x, pos_y));
            poly.set_velocity(Vec2f(vel_x, vel_y));
            poly.set_frozen(false);
            ball.set_frozen(false);
        }
    }
}

auto BouncingBall::on_mouse_button_press(const MouseButtonEventType& event) -> void
{
    const auto mods = ::SDL_GetModState();

    if(event.button == SDL_BUTTON_LEFT) {
        auto& poly(*_poly);
        auto& ball(*_ball);
        if(mods & (KMOD_LSHIFT | KMOD_RSHIFT)) {
            const float pos_x = float(event.x);
            const float pos_y = float(event.y);
            const float vel_x = float(0);
            const float vel_y = float(0);
            ball.set_position(Pos2f(pos_x, pos_y));
            ball.set_velocity(Vec2f(vel_x, vel_y));
            ball.set_frozen(true);
            poly.set_frozen(false);
        }
        else {
            const float pos_x = float(event.x);
            const float pos_y = float(event.y);
            const float vel_x = float(0);
            const float vel_y = float(0);
            poly.set_position(Pos2f(pos_x, pos_y));
            poly.set_velocity(Vec2f(vel_x, vel_y));
            poly.set_frozen(false);
            ball.set_frozen(false);
        }
    }
}

auto BouncingBall::on_mouse_button_release(const MouseButtonEventType& event) -> void
{
    if(event.button == SDL_BUTTON_LEFT) {
        auto& poly(*_poly);
        auto& ball(*_ball);
        poly.set_frozen(false);
        ball.set_frozen(false);
    }
}

auto BouncingBall::on_mouse_wheel(const MouseWheelEventType& event) -> void
{
    const auto mods = ::SDL_GetModState();

    if(mods & (KMOD_LSHIFT | KMOD_RSHIFT)) {
        set_ball_radius(_ball->radius() + (float(event.y * 100) * _dtime));
    }
    else {
        set_poly_radius(_poly->radius() + (float(event.y * 100) * _dtime));
    }
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
