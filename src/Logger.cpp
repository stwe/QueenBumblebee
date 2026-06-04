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

#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

#ifndef LOG_TO_TERMINAL
#include "spdlog/sinks/basic_file_sink.h"
#endif

//-------------------------------------------------
// Getter
//-------------------------------------------------

std::shared_ptr<spdlog::logger> qb::Logger::GetLogger()
{
    static struct Initializer
    {
        Initializer() { Init(); }
    } initializer;

    return s_logger;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void qb::Logger::Init()
{
    if (s_logger)
    {
        return;
    }

#ifdef LOG_TO_TERMINAL
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_logger = spdlog::stdout_color_mt("QB");
#else
    auto fileSink{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "qb-debug.log",
        true)
    };

    s_logger = std::make_shared<spdlog::logger>("QB", fileSink);
    spdlog::set_default_logger(s_logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

#ifdef QB_DEBUG_BUILD
    s_logger->flush_on(spdlog::level::trace);
#else
    s_logger->flush_on(spdlog::level::info);
#endif
#endif

#ifdef QB_DEBUG_BUILD
    s_logger->set_level(spdlog::level::trace);
#else
    s_logger->set_level(spdlog::level::info);
#endif

    s_logger->info("[Logger::Init()] Logger initialized.");
}
