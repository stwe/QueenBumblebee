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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifndef LOG_TO_TERMINAL
#include "spdlog/sinks/basic_file_sink.h"
#endif

namespace qb
{
    //-------------------------------------------------
    // Logger
    //-------------------------------------------------

    /**
     * @brief Provides logging functionality using the spdlog library.
     */
    class Logger
    {
    public:
        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        static std::shared_ptr<spdlog::logger>& GetLogger()
        {
            if (!s_logger)
            {
                Init();
            }

            return s_logger;
        }

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static std::shared_ptr<spdlog::logger> s_logger; // NOLINT(*-identifier-naming)

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        static void Init()
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
                "/home/steffen/DevProjects/QueenBumblebee/qb-debug.log",
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
    };
}

#define QB_LOG_TRACE(...)    ::qb::Logger::GetLogger()->trace(__VA_ARGS__)
#define QB_LOG_DEBUG(...)    ::qb::Logger::GetLogger()->debug(__VA_ARGS__)
#define QB_LOG_INFO(...)     ::qb::Logger::GetLogger()->info(__VA_ARGS__)
#define QB_LOG_WARN(...)     ::qb::Logger::GetLogger()->warn(__VA_ARGS__)
#define QB_LOG_ERROR(...)    ::qb::Logger::GetLogger()->error(__VA_ARGS__)
#define QB_LOG_CRITICAL(...) ::qb::Logger::GetLogger()->critical(__VA_ARGS__)
