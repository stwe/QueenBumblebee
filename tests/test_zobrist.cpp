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

#define CATCH_CONFIG_MAIN

#include <unordered_set>
#include <catch2/catch_test_macros.hpp>
#include "Zobrist.h"

TEST_CASE("ZobristKeys deterministic generation")
{
    constexpr uint64_t TEST_SEED{ 123456789 };
    const qb::ZobristKeys zobrist1(TEST_SEED);
    const qb::ZobristKeys zobrist2(TEST_SEED);

    SECTION("PieceSquareKeys identical with same seed")
    {
        for (size_t color{ 0 }; color < 2; ++color)
        {
            for (size_t piece{ 0 }; piece < 6; ++piece)
            {
                for (size_t square{ 0 }; square < 64; ++square)
                {
                    REQUIRE(
                        zobrist1.GetPieceSquareKey(static_cast<qb::Color>(color), static_cast<qb::PieceType>(piece), (qb::Square)square) ==
                        zobrist2.GetPieceSquareKey(static_cast<qb::Color>(color), static_cast<qb::PieceType>(piece), (qb::Square)square)
                    );
                }
            }
        }
    }

    SECTION("SideToMoveKey identical with same seed")
    {
        REQUIRE(zobrist1.GetSideToMoveKey() == zobrist2.GetSideToMoveKey());
    }

    SECTION("Keys are unique and non-zero")
    {
        std::unordered_set<uint64_t> keys;

        for (size_t color{ 0 }; color < 2; ++color)
        {
            for (size_t piece{ 0 }; piece < 6; ++piece)
            {
                for (size_t square{ 0 }; square < 64; ++square)
                {
                    auto key { zobrist1.GetPieceSquareKey(
                        static_cast<qb::Color>(color),
                        static_cast<qb::PieceType>(piece),
                        static_cast<qb::Square>(square)
                    ) };

                    REQUIRE(key != 0);

                    auto [fst, snd]{ keys.insert(key) };
                    REQUIRE(snd == true); // .second ist false, wenn das Element schon existierte
                }
            }
        }
    }
}
