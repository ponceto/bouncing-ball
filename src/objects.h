/*
 * objects.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __Objects_h__
#define __Objects_h__

#include "geometry.h"
#include "canvas.h"

// ---------------------------------------------------------------------------
// Object
// ---------------------------------------------------------------------------

class Object
{
public: // public interface
    Object(const Pos2f& position, const Col4i& color);

    Object(const Object&) = delete;

    Object& operator=(const Object&) = delete;

    virtual ~Object() = default;

    virtual auto update(const float dt) -> void = 0;

    virtual auto render(Canvas& canvas) -> void = 0;

public: // public accessors
    auto position() const -> const Pos2f&
    {
        return _position;
    }

    auto velocity() const -> const Vec2f&
    {
        return _velocity;
    }

    auto friction() const -> const Vec2f&
    {
        return _friction;
    }

    auto gravity() const -> const Vec2f&
    {
        return _gravity;
    }

    auto color() const -> const Col4i&
    {
        return _color;
    }

    auto frozen() const -> bool
    {
        return _frozen;
    }

public: // public mutators
    auto set_position(const Pos2f& position) -> void
    {
        _position = position;
    }

    auto set_velocity(const Vec2f& velocity) -> void
    {
        _velocity = velocity;
    }

    auto set_friction(const Vec2f& friction) -> void
    {
        _friction = friction;
    }

    auto set_gravity(const Vec2f& gravity) -> void
    {
        _gravity = gravity;
    }

    auto set_color(const Col4i& color) -> void
    {
        _color = color;
    }

    auto set_frozen(bool frozen) -> void
    {
        _frozen = frozen;
    }

protected: // protected data
    Pos2f _position;
    Vec2f _velocity;
    Vec2f _friction;
    Vec2f _gravity;
    Col4i _color;
    bool  _frozen;
};

// ---------------------------------------------------------------------------
// Poly
// ---------------------------------------------------------------------------

class Poly final
    : public Object
{
public: // public interface
    Poly(const Pos2f& position, int vertices, float radius);

    Poly(const Poly&) = delete;

    Poly& operator=(const Poly&) = delete;

    virtual ~Poly() = default;

    virtual auto update(const float dt) -> void override final;

    virtual auto render(Canvas& canvas) -> void override final;

public: // public accessors
    auto radius() const -> float
    {
        return _radius;
    }

    auto omega() const -> float
    {
        return _omega;
    }

    auto angle() const -> float
    {
        return _angle;
    }

public: // public mutators
    auto set_radius(float radius) -> void
    {
        _radius = radius;
    }

    auto set_omega(float omega) -> void
    {
        _omega = omega;
    }

    auto set_angle(float angle) -> void
    {
        _angle = angle;
    }

public: // public iterators
    auto begin() const -> auto
    {
        return _vertices.begin();
    }

    auto end() const -> auto
    {
        return _vertices.end();
    }

    auto rbegin() const -> auto
    {
        return _vertices.rbegin();
    }

    auto rend() const -> auto
    {
        return _vertices.rend();
    }

private: // private data
    std::vector<Pos2f> _vertices;
    float              _radius;
    float              _omega;
    float              _angle;
};

// ---------------------------------------------------------------------------
// Ball
// ---------------------------------------------------------------------------

class Ball final
    : public Object
{
public: // public interface
    Ball(const Pos2f& position, float radius);

    Ball(const Ball&) = delete;

    Ball& operator=(const Ball&) = delete;

    virtual ~Ball() = default;

    virtual auto update(const float dt) -> void override final;

    virtual auto render(Canvas& canvas) -> void override final;

    auto collide(const Poly& poly) -> void;

public: // public accessors
    auto radius() const -> float
    {
        return _radius;
    }

public: // public mutators
    auto set_radius(float radius) -> void
    {
        _radius = radius;
    }

private: // private data
    float _radius;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __Objects_h__ */
