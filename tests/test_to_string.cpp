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

#define CATCH_CONFIG_MAIN

#include <regex>
#include <catch2/catch_test_macros.hpp>
#include "ToString.h"

//-------------------------------------------------
// Piece string
//-------------------------------------------------

TEST_CASE("create_short_piece_string works correctly", "[create_short_piece_string]")
{
    REQUIRE(qb::create_short_piece_string(qb::PAWN, qb::WHITE) == "P");
    REQUIRE(qb::create_short_piece_string(qb::KNIGHT, qb::BLACK) == "n");
    REQUIRE(qb::create_short_piece_string(qb::KING, qb::WHITE) == "K");
    REQUIRE(qb::create_short_piece_string(qb::QUEEN, qb::BLACK) == "q");
    REQUIRE(qb::create_short_piece_string(qb::NO_PIECE_TYPE, qb::WHITE).empty());
    REQUIRE(qb::create_short_piece_string(qb::PAWN, qb::NO_COLOR).empty());
}

TEST_CASE("get_piece_string returns correct piece representations", "[get_piece_string]")
{
    qb::Bitboards bitboards{};

    SECTION("Empty board returns empty string and NO_COLOR")
    {
        const auto [fst, snd]{ qb::get_piece_string(qb::FILE_A, qb::RANK_1, bitboards) };
        REQUIRE(fst == "   ");
        REQUIRE(snd == qb::NO_COLOR);
    }

    SECTION("White pieces return correct strings and color")
    {
        qb::set_bit(bitboards.whitePawns, qb::FILE_A, qb::RANK_2);
        qb::set_bit(bitboards.whiteKnights, qb::FILE_B, qb::RANK_1);
        qb::set_bit(bitboards.whiteBishops, qb::FILE_C, qb::RANK_1);
        qb::set_bit(bitboards.whiteRooks, qb::FILE_A, qb::RANK_1);
        qb::set_bit(bitboards.whiteQueens, qb::FILE_D, qb::RANK_1);
        qb::set_bit(bitboards.whiteKing, qb::FILE_E, qb::RANK_1);

        const auto pawn{ qb::get_piece_string(qb::FILE_A, qb::RANK_2, bitboards) };
        REQUIRE(pawn.first == " P ");
        REQUIRE(pawn.second == qb::WHITE);

        const auto knight{ qb::get_piece_string(qb::FILE_B, qb::RANK_1, bitboards) };
        REQUIRE(knight.first == " N ");
        REQUIRE(knight.second == qb::WHITE);

        const auto bishop{ qb::get_piece_string(qb::FILE_C, qb::RANK_1, bitboards) };
        REQUIRE(bishop.first == " B ");
        REQUIRE(bishop.second == qb::WHITE);

        const auto rook{ qb::get_piece_string(qb::FILE_A, qb::RANK_1, bitboards) };
        REQUIRE(rook.first == " R ");
        REQUIRE(rook.second == qb::WHITE);

        const auto queen{ qb::get_piece_string(qb::FILE_D, qb::RANK_1, bitboards) };
        REQUIRE(queen.first == " Q ");
        REQUIRE(queen.second == qb::WHITE);

        const auto king{ qb::get_piece_string(qb::FILE_E, qb::RANK_1, bitboards) };
        REQUIRE(king.first == " K ");
        REQUIRE(king.second == qb::WHITE);
    }

    SECTION("Black pieces return correct strings and color")
    {
        qb::set_bit(bitboards.blackPawns, qb::FILE_A, qb::RANK_7);
        qb::set_bit(bitboards.blackKnights, qb::FILE_B, qb::RANK_8);
        qb::set_bit(bitboards.blackBishops, qb::FILE_C, qb::RANK_8);
        qb::set_bit(bitboards.blackRooks, qb::FILE_A, qb::RANK_8);
        qb::set_bit(bitboards.blackQueens, qb::FILE_D, qb::RANK_8);
        qb::set_bit(bitboards.blackKing, qb::FILE_E, qb::RANK_8);

        const auto pawn{ qb::get_piece_string(qb::FILE_A, qb::RANK_7, bitboards) };
        REQUIRE(pawn.first == " p ");
        REQUIRE(pawn.second == qb::BLACK);

        const auto knight{ qb::get_piece_string(qb::FILE_B, qb::RANK_8, bitboards) };
        REQUIRE(knight.first == " n ");
        REQUIRE(knight.second == qb::BLACK);

        const auto bishop{ qb::get_piece_string(qb::FILE_C, qb::RANK_8, bitboards) };
        REQUIRE(bishop.first == " b ");
        REQUIRE(bishop.second == qb::BLACK);

        const auto rook{ qb::get_piece_string(qb::FILE_A, qb::RANK_8, bitboards) };
        REQUIRE(rook.first == " r ");
        REQUIRE(rook.second == qb::BLACK);

        const auto queen{ qb::get_piece_string(qb::FILE_D, qb::RANK_8, bitboards) };
        REQUIRE(queen.first == " q ");
        REQUIRE(queen.second == qb::BLACK);

        const auto king{ qb::get_piece_string(qb::FILE_E, qb::RANK_8, bitboards) };
        REQUIRE(king.first == " k ");
        REQUIRE(king.second == qb::BLACK);
    }
}

//-------------------------------------------------
// Bitboard string
//-------------------------------------------------

TEST_CASE("bitboard_to_string produces correct output", "[bitboard_to_string]")
{
    constexpr U64 BOARD{ qb::A1 | qb::D4 | qb::H8 };
    std::string expected =
R"(8|00000001
7|00000000
6|00000000
5|00000000
4|00010000
3|00000000
2|00000000
1|10000000
  --------
  ABCDEFGH
)";

    REQUIRE(qb::bitboard_to_string(BOARD) == expected);
}
