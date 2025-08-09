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

#include <cstdint>

namespace qb::polyglot
{
    //-------------------------------------------------
    // Entry structure
    //-------------------------------------------------

    /**
     * @brief Represents a single record in a Polyglot opening book.
     *
     * The Polyglot binary book format stores each move along with its
     * associated metadata in fixed‑width, big‑endian records.
     *
     * Field layout (big‑endian):
     * | Bytes | Name   | Description                               |
     * |-------|--------|-------------------------------------------|
     * | 0–7   | key    | 64‑bit Polyglot Zobrist hash of the board |
     * |       |        | position.                                 |
     * | 8–9   | move   | Encoded move in 16‑bit Polyglot move      |
     * |       |        | format (from/to/prom flags).              |
     * | 10‑11 | weight | Relative move weight (0‑65535).           |
     * | 12‑15 | learn  | Learn score used by engines for           |
     * |       |        | self‑learning.                            |
     *
     * @note All fields are stored big‑endian on disk; use the helper
     *       byte‑swap functions below to map to native endianness.
     */
#pragma pack(push, 1)
    struct Entry
    {
        uint64_t key; //!< Zobrist hash of the position.
        uint16_t move; //!< Encoded move (Polyglot format).
        uint16_t weight; //!< Relative weight; higher = preferred.
        uint32_t learn; //!< Learning value for engine tuning.
    };
#pragma pack(pop)

    //-------------------------------------------------
    // Byte‑swap helpers (big‑endian to host order)
    //-------------------------------------------------

    /**
     * @brief Swap a 64‑bit value from big‑endian to host order.
     *
     * @param t_val Big‑endian value.
     *
     * @return Value in host byte order.
     */
    static uint64_t Be64(const uint64_t t_val) { return __builtin_bswap64(t_val); }

    /**
     * @brief Swap a 16‑bit value from big‑endian to host order.
     *
     * @param t_val Big‑endian value.
     *
     * @return Value in host byte order.
     */
    static uint16_t Be16(const uint16_t t_val) { return __builtin_bswap16(t_val); }

    /**
     * @brief Swap a 32‑bit value from big‑endian to host order.
     *
     * @param t_val Big‑endian value.
     *
     * @return Value in host byte order.
     */
    static uint32_t Be32(const uint32_t t_val) { return __builtin_bswap32(t_val); }
}
