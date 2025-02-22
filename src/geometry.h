/*
 * geometry.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __Geometry_h__
#define __Geometry_h__

// ---------------------------------------------------------------------------
// Pos2f
// ---------------------------------------------------------------------------

struct Pos2f
{
    Pos2f()
        : x(0.0f)
        , y(0.0f)
    {
    }

    Pos2f(float xy)
        : x(xy)
        , y(xy)
    {
    }

    Pos2f(float vx, float vy)
        : x(vx)
        , y(vy)
    {
    }

    float x;
    float y;
};

// ---------------------------------------------------------------------------
// Vec2f
// ---------------------------------------------------------------------------

struct Vec2f
{
    Vec2f()
        : x(0.0f)
        , y(0.0f)
    {
    }

    Vec2f(float xy)
        : x(xy)
        , y(xy)
    {
    }

    Vec2f(float vx, float vy)
        : x(vx)
        , y(vy)
    {
    }

    float x;
    float y;
};

// ---------------------------------------------------------------------------
// Pos2f operators
// ---------------------------------------------------------------------------

inline auto operator+(const Pos2f& lhs, const Vec2f& rhs) -> Pos2f
{
    return Pos2f((lhs.x + rhs.x), (lhs.y + rhs.y));
}

inline auto operator-(const Pos2f& lhs, const Vec2f& rhs) -> Pos2f
{
    return Pos2f((lhs.x + rhs.x), (lhs.y + rhs.y));
}

inline auto operator*(const Pos2f& lhs, const Vec2f& rhs) -> Pos2f
{
    return Pos2f((lhs.x * rhs.x), (lhs.y * rhs.y));
}

inline auto operator/(const Pos2f& lhs, const Vec2f& rhs) -> Pos2f
{
    return Pos2f((lhs.x / rhs.x), (lhs.y / rhs.y));
}

inline auto operator+=(Pos2f& lhs, const Vec2f& rhs) -> Pos2f&
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline auto operator-=(Pos2f& lhs, const Vec2f& rhs) -> Pos2f&
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline auto operator*=(Pos2f& lhs, const Vec2f& rhs) -> Pos2f&
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

inline auto operator/=(Pos2f& lhs, const Vec2f& rhs) -> Pos2f&
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

inline auto operator-(const Pos2f& lhs, const Pos2f& rhs) -> Vec2f
{
    return Vec2f((lhs.x - rhs.x), (lhs.y - rhs.y));
}

// ---------------------------------------------------------------------------
// Vec2f operators
// ---------------------------------------------------------------------------

inline auto operator+(const Vec2f& lhs, const Vec2f& rhs) -> Vec2f
{
    return Vec2f((lhs.x + rhs.x), (lhs.y + rhs.y));
}

inline auto operator-(const Vec2f& lhs, const Vec2f& rhs) -> Vec2f
{
    return Vec2f((lhs.x - rhs.x), (lhs.y - rhs.y));
}

inline auto operator*(const Vec2f& lhs, const Vec2f& rhs) -> Vec2f
{
    return Vec2f((lhs.x * rhs.x), (lhs.y * rhs.y));
}

inline auto operator/(const Vec2f& lhs, const Vec2f& rhs) -> Vec2f
{
    return Vec2f((lhs.x / rhs.x), (lhs.y / rhs.y));
}

inline auto operator*(const Vec2f& lhs, const float value) -> Vec2f
{
    return Vec2f((lhs.x * value), (lhs.y * value));
}

inline auto operator/(const Vec2f& lhs, const float value) -> Vec2f
{
    return Vec2f((lhs.x / value), (lhs.y / value));
}

inline auto operator+=(Vec2f& lhs, const Vec2f& rhs) -> Vec2f&
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline auto operator-=(Vec2f& lhs, const Vec2f& rhs) -> Vec2f&
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline auto operator*=(Vec2f& lhs, const Vec2f& rhs) -> Vec2f&
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

inline auto operator/=(Vec2f& lhs, const Vec2f& rhs) -> Vec2f&
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

inline auto operator*=(Vec2f& lhs, const float value) -> Vec2f&
{
    lhs.x *= value;
    lhs.y *= value;
    return lhs;
}

inline auto operator/=(Vec2f& lhs, const float value) -> Vec2f&
{
    lhs.x /= value;
    lhs.y /= value;
    return lhs;
}

inline auto dot(const Vec2f& lhs, const Vec2f& rhs) -> float
{
    return float((lhs.x * rhs.x) + (lhs.y * rhs.y));
}

inline auto length(const Vec2f& vector) -> float
{
    return ::hypotf(vector.x, vector.y);
}

inline auto normalize(const Vec2f& vector) -> Vec2f
{
    const float length = ::hypotf(vector.x, vector.y);

    if(length == 0.0f) {
        return Vec2f();
    }
    return Vec2f((vector.x / length), (vector.y / length));
}

inline auto perpendicular(const Vec2f& vector) -> Vec2f
{
    return Vec2f(-vector.y, +vector.x);
}

inline auto reflect(const Vec2f& vector, const Vec2f& normal) -> Vec2f
{
    return Vec2f(vector - (normal * (2.0f * dot(vector, normal))));
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __Geometry_h__ */
