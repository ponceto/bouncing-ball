/*
 * canvas.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __Canvas_h__
#define __Canvas_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// ---------------------------------------------------------------------------
// type aliases
// ---------------------------------------------------------------------------

using DrawableType                = SDL_Window;
using RendererType                = SDL_Renderer;
using SurfaceType                 = SDL_Surface;
using TextureType                 = SDL_Texture;
using EventType                   = SDL_Event;
using CommonEventType             = SDL_CommonEvent;
using DisplayEventType            = SDL_DisplayEvent;
using WindowEventType             = SDL_WindowEvent;
using KeyboardEventType           = SDL_KeyboardEvent;
using TextEditingEventType        = SDL_TextEditingEvent;
using TextEditingExtEventType     = SDL_TextEditingExtEvent;
using TextInputEventType          = SDL_TextInputEvent;
using MouseMotionEventType        = SDL_MouseMotionEvent;
using MouseButtonEventType        = SDL_MouseButtonEvent;
using MouseWheelEventType         = SDL_MouseWheelEvent;
using JoyAxisEventType            = SDL_JoyAxisEvent;
using JoyBallEventType            = SDL_JoyBallEvent;
using JoyHatEventType             = SDL_JoyHatEvent;
using JoyButtonEventType          = SDL_JoyButtonEvent;
using JoyDeviceEventType          = SDL_JoyDeviceEvent;
using JoyBatteryEventType         = SDL_JoyBatteryEvent;
using ControllerAxisEventType     = SDL_ControllerAxisEvent;
using ControllerButtonEventType   = SDL_ControllerButtonEvent;
using ControllerDeviceEventType   = SDL_ControllerDeviceEvent;
using ControllerTouchpadEventType = SDL_ControllerTouchpadEvent;
using ControllerSensorEventType   = SDL_ControllerSensorEvent;
using AudioDeviceEventType        = SDL_AudioDeviceEvent;
using SensorEventType             = SDL_SensorEvent;
using QuitEventType               = SDL_QuitEvent;
using UserEventType               = SDL_UserEvent;
using SysWMEventType              = SDL_SysWMEvent;
using TouchFingerEventType        = SDL_TouchFingerEvent;
using MultiGestureEventType       = SDL_MultiGestureEvent;
using DollarGestureEventType      = SDL_DollarGestureEvent;
using DropEventType               = SDL_DropEvent;

// ---------------------------------------------------------------------------
// std::default_delete<DrawableType>
// ---------------------------------------------------------------------------

template <>
struct std::default_delete<DrawableType>
{
    auto operator()(DrawableType* drawable) const -> void
    {
        if(drawable != nullptr) {
            ::SDL_DestroyWindow(drawable);
        }
    }
};

// ---------------------------------------------------------------------------
// std::default_delete<RendererType>
// ---------------------------------------------------------------------------

template <>
struct std::default_delete<RendererType>
{
    auto operator()(RendererType* renderer) const -> void
    {
        if(renderer != nullptr) {
            ::SDL_DestroyRenderer(renderer);
        }
    }
};

// ---------------------------------------------------------------------------
// std::default_delete<SurfaceType>
// ---------------------------------------------------------------------------

template <>
struct std::default_delete<SurfaceType>
{
    auto operator()(SurfaceType* surface) const -> void
    {
        if(surface != nullptr) {
            ::SDL_FreeSurface(surface);
        }
    }
};

// ---------------------------------------------------------------------------
// std::default_delete<TextureType>
// ---------------------------------------------------------------------------

template <>
struct std::default_delete<TextureType>
{
    auto operator()(TextureType* texture) const -> void
    {
        if(texture != nullptr) {
            ::SDL_DestroyTexture(texture);
        }
    }
};

// ---------------------------------------------------------------------------
// Col4i
// ---------------------------------------------------------------------------

struct Col4i
{
    Col4i()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {
    }

    Col4i(float cr, float cg, float cb)
        : r(cr * 255.0f)
        , g(cg * 255.0f)
        , b(cb * 255.0f)
        , a(255)
    {
    }

    Col4i(float cr, float cg, float cb, float ca)
        : r(cr * 255.0f)
        , g(cg * 255.0f)
        , b(cb * 255.0f)
        , a(ca * 255.0f)
    {
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// ---------------------------------------------------------------------------
// Seg4i
// ---------------------------------------------------------------------------

struct Seg4i
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

// ---------------------------------------------------------------------------
// Canvas
// ---------------------------------------------------------------------------

class Canvas
{
public: // public interface
    Canvas(const std::string& title, const int width, const int height);

    Canvas(const Canvas&) = delete;

    Canvas& operator=(const Canvas&) = delete;

    virtual ~Canvas() = default;

    auto clear() -> void;

    auto present() -> void;

    auto color(const Col4i& color) -> void;

    auto line(int x1, int y1, int x2, int y2) -> void;

    auto circle(int xc, int yc, int r) -> void;

    auto toggle_underlay() -> void
    {
        _show_underlay = !_show_underlay;
    }

    auto toggle_overlay() -> void
    {
        _show_overlay = !_show_overlay;
    }

    operator DrawableType*() const
    {
        return _drawable.get();
    }

    operator RendererType*() const
    {
        return _renderer.get();
    }

protected: // protected interface
    auto create(const int width, const int height) -> void;

protected: // protected data
    std::string                   _title;
    std::unique_ptr<DrawableType> _drawable;
    std::unique_ptr<RendererType> _renderer;
    std::unique_ptr<TextureType>  _underlay;
    std::unique_ptr<TextureType>  _overlay;
    bool                          _show_underlay;
    bool                          _show_overlay;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __Canvas_h__ */
