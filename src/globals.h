/*
 * globals.h - Copyright (c) 2024-2025 - Olivier Poncet
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
#ifndef __Globals_h__
#define __Globals_h__

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

struct Globals
{
    static auto init() -> void;

    static auto dump(std::ostream& stream) -> void;

    static auto set_app_width(int app_hwidth) -> void;

    static auto set_app_height(int app_hheight) -> void;

    static auto set_poly_vertices(int poly_vertices) -> void;

    static auto set_poly_radius(float poly_radius) -> void;

    static auto set_poly_omega(float poly_omega) -> void;

    static auto set_poly_friction(float poly_friction) -> void;

    static auto set_poly_gravity(float poly_gravity) -> void;

    static auto set_ball_radius(float ball_radius) -> void;

    static auto set_ball_friction(float ball_friction) -> void;

    static auto set_ball_gravity(float ball_gravity) -> void;

    static int   app_width;
    static int   app_height;
    static int   poly_vertices;
    static float poly_radius;
    static float poly_omega;
    static float poly_friction;
    static float poly_gravity;
    static float ball_radius;
    static float ball_friction;
    static float ball_gravity;
};

// ---------------------------------------------------------------------------
// GlobalsMin
// ---------------------------------------------------------------------------

struct GlobalsMin
{
    static constexpr int   app_width     =  480;
    static constexpr int   app_height    =  270;
    static constexpr int   poly_vertices =    3;
    static constexpr float poly_radius   =  100.0f;
    static constexpr float poly_omega    = -100.0f;
    static constexpr float poly_friction =    0.0f;
    static constexpr float poly_gravity  =    0.0f;
    static constexpr float ball_radius   =   25.0f;
    static constexpr float ball_friction =    0.0f;
    static constexpr float ball_gravity  =    0.0f;
};

// ---------------------------------------------------------------------------
// GlobalsMax
// ---------------------------------------------------------------------------

struct GlobalsMax
{
    static constexpr int   app_width     = 1920;
    static constexpr int   app_height    = 1080;
    static constexpr int   poly_vertices =   36;
    static constexpr float poly_radius   =  500.0f;
    static constexpr float poly_omega    = +100.0f;
    static constexpr float poly_friction =   10.0f;
    static constexpr float poly_gravity  = 9999.0f;
    static constexpr float ball_radius   =  250.0f;
    static constexpr float ball_friction =   10.0f;
    static constexpr float ball_gravity  = 9999.0f;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __Globals_h__ */
