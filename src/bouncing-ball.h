/*
 * bouncing-ball.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __BouncingBall_h__
#define __BouncingBall_h__

#include "application.h"
#include "objects.h"

// ---------------------------------------------------------------------------
// BouncingBall
// ---------------------------------------------------------------------------

class BouncingBall final
    : public Application
{
public: // public interface
    BouncingBall(const int width, const int height);

    BouncingBall(const BouncingBall&) = delete;

    BouncingBall& operator=(const BouncingBall&) = delete;

    virtual ~BouncingBall() = default;

protected: // protected interface
    virtual auto update() -> void override final;

    virtual auto render() -> void override final;

    virtual auto shutdown() -> void override final;

    virtual auto on_quit(const QuitEventType&) -> void override final;

    virtual auto on_window(const WindowEventType&) -> void override final;

    virtual auto on_key_press(const KeyboardEventType&) -> void override final;

    virtual auto on_key_release(const KeyboardEventType&) -> void override final;

    virtual auto on_mouse_motion(const MouseMotionEventType&) -> void override final;

    virtual auto on_mouse_button_press(const MouseButtonEventType&) -> void override final;

    virtual auto on_mouse_button_release(const MouseButtonEventType&) -> void override final;

    virtual auto on_mouse_wheel(const MouseWheelEventType&) -> void override final;

private: // private interface
    auto create_canvas(int width, int height) -> void;

    auto create_poly() -> void;

    auto create_ball() -> void;

    auto toggle_underlay() -> void;

    auto toggle_overlay() -> void;

    auto set_poly_vertices(int vertices) -> void;

    auto set_poly_radius(float radius) -> void;

    auto set_poly_omega(float omega) -> void;

    auto set_ball_radius(float radius) -> void;

    auto resized(int width, int height) -> void;

private: // private data
    std::unique_ptr<Canvas> _canvas;
    std::unique_ptr<Poly>   _poly;
    std::unique_ptr<Ball>   _ball;
    Vec2f                   _size;
    Pos2f                   _center;
    Col4i                   _color;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __BouncingBall_h__ */
