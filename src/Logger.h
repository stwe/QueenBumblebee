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

#include <memory>
#include <spdlog/spdlog.h>

namespace qb
{
    //-------------------------------------------------
    // Logger
    //-------------------------------------------------

    /**
     * @brief Provides thread-safe logging functionality using the spdlog library.
     */
    class Logger
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Logger() = delete;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Retrieves the global logger instance.
         *
         * @return A shared_ptr to the logger instance.
         */
        static std::shared_ptr<spdlog::logger> GetLogger();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static std::shared_ptr<spdlog::logger> s_logger; // NOLINT(*-identifier-naming)

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * @brief Initializes and configures the logger.
         */
        static void Init();
    };
}

#define QB_LOG_TRACE(...)    ::qb::Logger::GetLogger()->trace(__VA_ARGS__)
#define QB_LOG_DEBUG(...)    ::qb::Logger::GetLogger()->debug(__VA_ARGS__)
#define QB_LOG_INFO(...)     ::qb::Logger::GetLogger()->info(__VA_ARGS__)
#define QB_LOG_WARN(...)     ::qb::Logger::GetLogger()->warn(__VA_ARGS__)
#define QB_LOG_ERROR(...)    ::qb::Logger::GetLogger()->error(__VA_ARGS__)
#define QB_LOG_CRITICAL(...) ::qb::Logger::GetLogger()->critical(__VA_ARGS__)
