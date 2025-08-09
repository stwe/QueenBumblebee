// This file is part of the QueenBumblebee project.
//
// Copyright (c) 2025. stwe <https://github.com/stwe/QueenBumblebee>
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

#include "Logger.h"

#if defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 11) && !defined(QB_ASSERT_TEST_MODE)
    #include <csignal>

    #define QB_DEBUG_BREAK raise(SIGTRAP)

    #ifdef QB_DEBUG_BUILD
        #define QB_ENABLE_ASSERTS
    #endif

    #ifdef QB_ENABLE_ASSERTS
        #define QB_ASSERT(x, ...)                                               \
            {                                                                   \
                if (!(x))                                                       \
                {                                                               \
                    QB_LOG_ERROR("Assertion Failed: {0}, File: {1}, Line: {2}", \
                    __VA_ARGS__, __FILE__, __LINE__);                           \
                    QB_DEBUG_BREAK;                                             \
                }                                                               \
            }
    #else
        #define QB_ASSERT(x, ...)
    #endif
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 11) && defined(QB_ASSERT_TEST_MODE)
    #include <stdexcept>

    #define QB_ASSERT(x, ...) \
        { \
            if (!(x)) \
            { \
                throw std::logic_error("QB_ASSERT failed: " #x); \
            } \
        }
#else
    #error "Compiler not supported."
#endif
