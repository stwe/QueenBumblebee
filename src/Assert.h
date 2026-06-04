// This file is part of the QueenBumblebee project.
//
// Copyright (c) 2026. stwe <https://github.com/stwe/QueenBumblebee>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#pragma once

#include <source_location>
#include <string_view>
#include <exception>
#include <csignal>
#include "Logger.h"

namespace qb
{
    [[noreturn]] inline void assert_fail(
        std::string_view t_message,
        const std::source_location& t_loc = std::source_location::current())
    {
        QB_LOG_CRITICAL(
            "Assertion failed: {}\n  at {}:{} in {}",
            t_message,
            t_loc.file_name(),
            t_loc.line(),
            t_loc.function_name()
        );

        std::raise(SIGTRAP);

        std::terminate();
    }
}

#if defined(QB_DEBUG_BUILD)
    #define QB_ASSERT(cond, msg)          \
        do {                              \
            if (!(cond))                  \
                ::qb::assert_fail((msg)); \
        } while (false)
#else
    #define QB_ASSERT(cond, msg) ((void)0)
#endif
