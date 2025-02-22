/*
 * program.cc - Copyright (c) 2024-2025 - Olivier Poncet
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
#include "program.h"
#include "bouncing-ball.h"

// ---------------------------------------------------------------------------
// Program
// ---------------------------------------------------------------------------

auto Program::init(const ArgList& args) -> bool
{
    static const std::locale new_locale("");
    static const std::locale old_locale(std::locale::global(new_locale));

    auto init_globals = [&]() -> bool
    {
        Globals::init();

        return true;
    };

    auto do_init = [&]() -> bool
    {
        int argi = -1;
        for(auto& arg : args) {
            if(++argi == 0) {
                continue;
            }
            else if(arg == "-h") {
                return false;
            }
            else if(arg == "--help") {
                return false;
            }
            else if(arg == "triangle") {
                Globals::set_poly_vertices(3);
            }
            else if(arg == "square") {
                Globals::set_poly_vertices(4);
            }
            else if(arg == "pentagon") {
                Globals::set_poly_vertices(5);
            }
            else if(arg == "hexagon") {
                Globals::set_poly_vertices(6);
            }
            else if(arg == "heptagon") {
                Globals::set_poly_vertices(7);
            }
            else if(arg == "octagon") {
                Globals::set_poly_vertices(8);
            }
            else if(arg == "nonagon") {
                Globals::set_poly_vertices(9);
            }
            else if(arg == "decagon") {
                Globals::set_poly_vertices(10);
            }
            else if(arg == "hendecagon") {
                Globals::set_poly_vertices(11);
            }
            else if(arg == "dodecagon") {
                Globals::set_poly_vertices(12);
            }
            else if(arg == "mercury") {
                Globals::set_ball_gravity(3700.00f);
            }
            else if(arg == "venus") {
                Globals::set_ball_gravity(8870.00f);
            }
            else if(arg == "earth") {
                Globals::set_ball_gravity(9806.65f);
            }
            else if(arg == "mars") {
                Globals::set_ball_gravity(3728.00f);
            }
            else if(arg == "moon") {
                Globals::set_ball_gravity(1625.00f);
            }
            else {
                throw std::runtime_error(std::string("invalid argument") + ' ' + '\'' + arg + '\'');
            }
        }
        return init_globals();
    };

    return do_init();
}

auto Program::main(const ArgList& args) -> void
{
    std::unique_ptr<Application> application(new BouncingBall(Globals::app_width, Globals::app_height));

#ifdef __EMSCRIPTEN__
    auto do_loop = +[](void* data) -> void
    {
        Application* application(reinterpret_cast<Application*>(data));

        if(application->running()) {
            application->main();
        }
        else {
            application = (delete application, nullptr);
            ::emscripten_cancel_main_loop();
        }
    };

    auto loop = [&]() -> void
    {
        return ::emscripten_set_main_loop_arg(do_loop, application.release(), 0, 1);
    };
#else
    auto loop = [&]() -> void
    {
        return application->main();
    };
#endif

    auto do_main = [&](std::ostream& stream) -> void
    {
        Globals::dump(stream);

        stream << "Pro tip: type <h> to display help" << std::endl;

        return loop();
    };

    return do_main(std::cout);
}

auto Program::help(const ArgList& args) -> void
{
    auto program_name = [&]() -> const char*
    {
        const char* arg = args[0].c_str();
        const char* sep = ::strrchr(arg, '/');
        if(sep != nullptr) {
            arg = sep + 1;
        }
        return arg;
    };

    auto do_help = [&](std::ostream& stream) -> void
    {
        stream << "Usage: " << program_name() << " [OPTIONS...] [SHAPE] [PLANET]" << std::endl;
        stream << ""                                                              << std::endl;
        stream << "Options:"                                                      << std::endl;
        stream << ""                                                              << std::endl;
        stream << "  -h, --help                    display this help and exit"    << std::endl;
        stream << ""                                                              << std::endl;
        stream << "Shapes:"                                                       << std::endl;
        stream << ""                                                              << std::endl;
        stream << "  triangle, square, pentagon,"                                 << std::endl;
        stream << "  hexagon, heptagon, octagon,"                                 << std::endl;
        stream << "  nonagon, decagon, hendecagon,"                               << std::endl;
        stream << "  dodecagon"                                                   << std::endl;
        stream << ""                                                              << std::endl;
        stream << "Planets:"                                                      << std::endl;
        stream << "  mercury, venus, earth, mars,"                                << std::endl;
        stream << "  moon"                                                        << std::endl;
        stream << ""                                                              << std::endl;
        stream << ""                                                              << std::endl;
        stream << "Controls:"                                                     << std::endl;
        stream << ""                                                              << std::endl;
        stream << "h ................ toggle help overlay"                        << std::endl;
        stream << "u ................ toggle back underlay"                       << std::endl;
        stream << "r ................ reset simulation"                           << std::endl;
        stream << "q ................ quit the program"                           << std::endl;
        stream << "up ............... increase polygon vertices"                  << std::endl;
        stream << "down ............. decrease polygon vertices"                  << std::endl;
        stream << "left ............. accelerate polygon to the left"             << std::endl;
        stream << "right ............ accelerate polygon to the right"            << std::endl;
        stream << "shift-left ....... accelerate polygon faster to the left"      << std::endl;
        stream << "shift-right ...... accelerate polygon faster to the right"     << std::endl;
        stream << "button ........... modify polygon position"                    << std::endl;
        stream << "wheel ............ modify polygon radius"                      << std::endl;
        stream << "shift-button ..... modify ball position"                       << std::endl;
        stream << "shift-wheel ...... modify ball radius"                         << std::endl;
        stream << "escape ........... quit the program"                           << std::endl;
        stream << ""                                                              << std::endl;
    };

    return do_help(std::cout);
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    const ArgList args(argv, argv + argc);

    try {
        if(Program::init(args) != false) {
            Program::main(args);
        }
        else {
            Program::help(args);
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "error!" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
