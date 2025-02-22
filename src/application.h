/*
 * application.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __Application_h__
#define __Application_h__

#include "canvas.h"

// ---------------------------------------------------------------------------
// Application
// ---------------------------------------------------------------------------

class Application
{
public: // public interface
    Application(const std::string& title);

    Application(const Application&) = delete;

    Application& operator=(const Application&) = delete;

    virtual ~Application();

    auto main() -> void;

    auto quit() -> void;

    auto running() const -> bool
    {
        return _quit == false;
    }

    auto stopped() const -> bool
    {
        return _quit != false;
    }

protected: // protected interface
    virtual auto update() -> void = 0;

    virtual auto render() -> void = 0;

    virtual auto shutdown() -> void = 0;

    virtual auto on_window(const WindowEventType& event) -> void = 0;

    virtual auto on_key_press(const KeyboardEventType& event) -> void = 0;

    virtual auto on_key_release(const KeyboardEventType& event) -> void = 0;

    virtual auto on_mouse_motion(const MouseMotionEventType& event) -> void = 0;

    virtual auto on_mouse_button_press(const MouseButtonEventType& event) -> void = 0;

    virtual auto on_mouse_button_release(const MouseButtonEventType& event) -> void = 0;

    virtual auto on_mouse_wheel(const MouseWheelEventType& event) -> void = 0;

protected: // protected data
    const std::string _title;
    uint32_t          _ptime;
    uint32_t          _ctime;
    float             _dtime;
    bool              _quit;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __Application_h__ */
