/*
 * objects.cc - Copyright (c) 2024-2025 - Olivier Poncet
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
#include "objects.h"

// ---------------------------------------------------------------------------
// Object
// ---------------------------------------------------------------------------

Object::Object(const Pos2f& position, const Col4i& color)
    : _position(position)
    , _velocity(0.0f, 0.0f)
    , _friction(0.0f, 0.0f)
    , _gravity(0.0f, 0.0f)
    , _color(color)
    , _frozen(false)
{
}

// ---------------------------------------------------------------------------
// Poly
// ---------------------------------------------------------------------------

Poly::Poly(const Pos2f& position, int vertices, float radius)
    : Object(position, Col4i(1.00f, 1.00f, 1.00f))
    , _vertices(vertices)
    , _radius(radius)
    , _omega(0.0f)
    , _angle(0.0f)
{
}

void Poly::update(const float dt)
{
    constexpr float m_2pi = 2.0f * M_PI;

    auto update_poly = [&]() -> void
    {
        int       index = 0;
        const int count = _vertices.size();
        for(auto& vertex : _vertices) {
            const float angle = _angle + (float(index) * (m_2pi / float(count)));
            vertex.x = _position.x + (_radius * ::cosf(angle));
            vertex.y = _position.y + (_radius * ::sinf(angle));
            ++index;
        }
    };

    if(_frozen == false) {
        _velocity += (_gravity * dt);
        _position += (_velocity * dt);
        _velocity *= (Vec2f(1.0f) - (_friction * dt));
        _angle    += (_omega * dt);
        while(_angle >= +m_2pi) {
            _angle -= m_2pi;
        }
        while(_angle <= -m_2pi) {
            _angle += m_2pi;
        }
        update_poly();
    }
}

void Poly::render(Canvas& canvas)
{
    auto render_poly = [&]() -> void
    {
        const Pos2f* prev(&(*rbegin()));
        for(auto& vertex : _vertices) {
            const int x1 = int((*prev).x);
            const int y1 = int((*prev).y);
            const int x2 = int(vertex.x);
            const int y2 = int(vertex.y);
            canvas.line(x1, y1, x2, y2);
            prev = &vertex;
        }
    };

    canvas.color(_color);
    render_poly();
}

// ---------------------------------------------------------------------------
// Ball
// ---------------------------------------------------------------------------

Ball::Ball(const Pos2f& position, float radius)
    : Object(position, Col4i(1.00f, 0.39f, 0.39f))
    , _radius(radius)
{
}

void Ball::update(const float dt)
{
    if(_frozen == false) {
        _velocity += (_gravity * dt);
        _position += (_velocity * dt);
        _velocity *= (Vec2f(1.0f) - (_friction * dt));
    }
}

void Ball::render(Canvas& canvas)
{
    canvas.color(_color);
    canvas.circle(_position.x, _position.y, _radius);
}

void Ball::collide(const Poly& poly)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    auto process = [&](const Pos2f& A, const Pos2f& B, const Pos2f& C, const float R) -> void
    {
        const Vec2f AB(B - A);
        const Vec2f AC(C - A);
        const float AB2 = dot(AB, AB);

        if(AB2 != 0.0f) {
            const float t = (dot(AC, AB) / AB2);
            if((t >= 0.0f) && (t <= 1.0f)) {
                const Pos2f P(A + (AB * t));
                const Vec2f PC(C - P);
                const float PC_length = (length(PC) + epsilon);
                if(PC_length <= R) {
                    const Vec2f normal(normalize(PC));
                    const Vec2f ball_velocity(_velocity);
                    const Vec2f poly_velocity(perpendicular(P - poly.position()) * poly.omega());
                    const Vec2f relative_velocity(ball_velocity - poly_velocity);
                    if(dot(relative_velocity, normal) < 0.0f) {
                        _velocity = reflect(relative_velocity, normal) + poly_velocity ;
                        _position += (normal * (R - PC_length));
                    }
                }
            }
        }
    };

    auto do_collide = [&]() -> void
    {
        const Pos2f* prev(&(*poly.rbegin()));
        for(auto& vertex : poly) {
            process(*prev, vertex, _position, _radius);
            prev = &vertex;
        }
    };

    return do_collide();
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
