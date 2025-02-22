/*
 * globals.cc - Copyright (c) 2024-2025 - Olivier Poncet
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

// ---------------------------------------------------------------------------
// <anonymous>::utilities
// ---------------------------------------------------------------------------

namespace {

inline auto clampi(int val, int min, int max) -> int
{
    if(val < min) {
        val = min;
    }
    if(val > max) {
        val = max;
    }
    return val;
}

inline auto clampf(float val, float min, float max) -> float
{
    if(val < min) {
        val = min;
    }
    if(val > max) {
        val = max;
    }
    return val;
}

}

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

#ifdef __EMSCRIPTEN__
int   Globals::app_width     =  960;
int   Globals::app_height    =  540;
int   Globals::poly_vertices =    6;
float Globals::poly_radius   =  250.00f;
float Globals::poly_omega    =    2.09f;
float Globals::poly_friction =    0.00f;
float Globals::poly_gravity  =    0.00f;
float Globals::ball_radius   =  100.00f;
float Globals::ball_friction =    0.25f;
float Globals::ball_gravity  = 9806.65f;
#else
int   Globals::app_width     = 1280;
int   Globals::app_height    =  720;
int   Globals::poly_vertices =    6;
float Globals::poly_radius   =  350.00f;
float Globals::poly_omega    =    2.09f;
float Globals::poly_friction =    0.00f;
float Globals::poly_gravity  =    0.00f;
float Globals::ball_radius   =  100.00f;
float Globals::ball_friction =    0.25f;
float Globals::ball_gravity  = 9806.65f;
#endif

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

auto Globals::init() -> void
{
    set_app_width(app_width);
    set_app_height(app_height);
    set_poly_vertices(poly_vertices);
    set_poly_radius(poly_radius);
    set_poly_omega(poly_omega);
    set_poly_friction(poly_friction);
    set_poly_gravity(poly_gravity);
    set_ball_radius(ball_radius);
    set_ball_friction(ball_friction);
    set_ball_gravity(ball_gravity);
}

auto Globals::dump(std::ostream& stream) -> void
{
    stream << "app_width" << " ....... " << app_width     << std::endl;
    stream << "app_height" << " ...... " << app_height    << std::endl;
    stream << "poly_vertices" << " ... " << poly_vertices << std::endl;
    stream << "poly_radius" << " ..... " << poly_radius   << std::endl;
    stream << "poly_omega" << " ...... " << poly_omega    << std::endl;
    stream << "poly_friction" << " ... " << poly_friction << std::endl;
    stream << "poly_gravity" << " .... " << poly_gravity  << std::endl;
    stream << "ball_radius" << " ..... " << ball_radius   << std::endl;
    stream << "ball_friction" << " ... " << ball_friction << std::endl;
    stream << "ball_gravity" << " .... " << ball_gravity  << std::endl;
}

auto Globals::set_app_width(int m_app_width) -> void
{
    app_width = clampi(m_app_width, GlobalsMin::app_width, GlobalsMax::app_width);
}

auto Globals::set_app_height(int m_app_height) -> void
{
    app_height = clampi(m_app_height, GlobalsMin::app_height, GlobalsMax::app_height);
}

auto Globals::set_poly_vertices(int m_poly_vertices) -> void
{
    poly_vertices = clampi(m_poly_vertices, GlobalsMin::poly_vertices, GlobalsMax::poly_vertices);
}

auto Globals::set_poly_radius(float m_poly_radius) -> void
{
    poly_radius = clampf(m_poly_radius, GlobalsMin::poly_radius, GlobalsMax::poly_radius);
}

auto Globals::set_poly_omega(float m_poly_omega) -> void
{
    poly_omega = clampf(m_poly_omega, GlobalsMin::poly_omega, GlobalsMax::poly_omega);
}

auto Globals::set_poly_friction(float m_poly_friction) -> void
{
    poly_friction = clampf(m_poly_friction, GlobalsMin::poly_friction, GlobalsMax::poly_friction);
}

auto Globals::set_poly_gravity(float m_poly_gravity) -> void
{
    poly_gravity = clampf(m_poly_gravity, GlobalsMin::poly_gravity, GlobalsMax::poly_gravity);
}

auto Globals::set_ball_radius(float m_ball_radius) -> void
{
    ball_radius = clampf(m_ball_radius, GlobalsMin::ball_radius, GlobalsMax::ball_radius);
}

auto Globals::set_ball_friction(float m_ball_friction) -> void
{
    ball_friction = clampf(m_ball_friction, GlobalsMin::ball_friction, GlobalsMax::ball_friction);
}

auto Globals::set_ball_gravity(float m_ball_gravity) -> void
{
    ball_gravity = clampf(m_ball_gravity, GlobalsMin::ball_gravity, GlobalsMax::ball_gravity);
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
