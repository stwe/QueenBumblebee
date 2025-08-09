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

#include <catch2/catch_test_macros.hpp>
#include "Types.h"

TEST_CASE("Square enum values are correctly ordered")
{
    REQUIRE(qb::SQ_A1 == 0);
    REQUIRE(qb::SQ_H1 == 7);
    REQUIRE(qb::SQ_A8 == 56);
    REQUIRE(qb::SQ_H8 == 63);
    REQUIRE(qb::SQ_NONE == 64);
}

TEST_CASE("File enum values are correct")
{
    REQUIRE(qb::FILE_A == 0);
    REQUIRE(qb::FILE_H == 7);
}

TEST_CASE("Rank enum values are correct")
{
    REQUIRE(qb::RANK_1 == 0);
    REQUIRE(qb::RANK_8 == 7);
}

TEST_CASE("Square increment and decrement operators work")
{
    qb::Square sq{ qb::SQ_A1 };
    ++sq;
    REQUIRE(sq == qb::SQ_B1);
    --sq;
    REQUIRE(sq == qb::SQ_A1);
}

TEST_CASE("File increment and decrement operators work")
{
    qb::File file{ qb::FILE_A };
    ++file;
    REQUIRE(file == qb::FILE_B);
    --file;
    REQUIRE(file == qb::FILE_A);
}

TEST_CASE("Rank increment and decrement operators work")
{
    qb::Rank rank{ qb::RANK_1 };
    ++rank;
    REQUIRE(rank == qb::RANK_2);
    --rank;
    REQUIRE(rank == qb::RANK_1);
}

TEST_CASE("Color enum values")
{
    REQUIRE(qb::WHITE == 0);
    REQUIRE(qb::BLACK == 1);
}

TEST_CASE("PieceType enum values")
{
    REQUIRE(qb::PAWN == 0);
    REQUIRE(qb::KING == 5);
    REQUIRE(qb::NO_PIECE_TYPE == 6);
}

TEST_CASE("CastlingRights bitwise operations work")
{
    qb::CastlingRights cr{ qb::NO_CASTLING };
    cr |= qb::WHITE_OO;
    REQUIRE((cr & qb::WHITE_OO) == qb::WHITE_OO);

    cr |= qb::BLACK_OOO;
    REQUIRE((cr & qb::BLACK_OOO) == qb::BLACK_OOO);

    auto combined{ qb::WHITE_OO | qb::BLACK_OO };
    REQUIRE(combined == qb::KING_SIDE);
}

TEST_CASE("CastlingRights bitwise AND operation works correctly")
{
    // Test basic AND operation
    auto rights{ qb::WHITE_OO | qb::BLACK_OO };
    REQUIRE((rights & qb::WHITE_OO) == qb::WHITE_OO);
    REQUIRE((rights & qb::BLACK_OO) == qb::BLACK_OO);
    REQUIRE((rights & qb::WHITE_OOO) == qb::NO_CASTLING);

    // Test with combined rights
    auto kingside{ qb::KING_SIDE };
    REQUIRE((kingside & qb::WHITE_OO) == qb::WHITE_OO);
    REQUIRE((kingside & qb::BLACK_OO) == qb::BLACK_OO);
    REQUIRE((kingside & qb::WHITE_OOO) == qb::NO_CASTLING);
}

TEST_CASE("CastlingRights compound AND assignment works correctly")
{
    qb::CastlingRights rights{ qb::ANY_CASTLING };

    // Test &= operation
    rights &= qb::KING_SIDE;
    REQUIRE((rights & qb::WHITE_OOO) == qb::NO_CASTLING);
    REQUIRE((rights & qb::BLACK_OOO) == qb::NO_CASTLING);
    REQUIRE((rights & qb::WHITE_OO) == qb::WHITE_OO);
    REQUIRE((rights & qb::BLACK_OO) == qb::BLACK_OO);

    // Further restriction
    rights &= qb::WHITE_CASTLING;
    REQUIRE(rights == qb::WHITE_OO);
}

TEST_CASE("CastlingRights bitwise NOT operation works correctly")
{
    // Test NOT operation on NO_CASTLING
    const auto invertedNone{ ~qb::NO_CASTLING };
    REQUIRE((invertedNone & qb::ANY_CASTLING) == qb::ANY_CASTLING);

    // Test NOT operation on specific rights
    const auto invertedKingside{ ~qb::KING_SIDE };
    REQUIRE((invertedKingside & qb::WHITE_OO) == qb::NO_CASTLING);
    REQUIRE((invertedKingside & qb::BLACK_OO) == qb::NO_CASTLING);
    REQUIRE((invertedKingside & qb::WHITE_OOO) == qb::WHITE_OOO);
    REQUIRE((invertedKingside & qb::BLACK_OOO) == qb::BLACK_OOO);
}

TEST_CASE("SQUARE_NAMES array correctness")
{
    REQUIRE(qb::SQUARE_NAMES[qb::SQ_A1] == "a1");
    REQUIRE(qb::SQUARE_NAMES[qb::SQ_H8] == "h8");
    REQUIRE(qb::SQUARE_NAMES[qb::SQ_D4] == "d4");
}

TEST_CASE("COLOR_NAMES correctness")
{
    REQUIRE(qb::COLOR_NAMES[qb::WHITE] == "White");
    REQUIRE(qb::COLOR_NAMES[qb::BLACK] == "Black");
}

TEST_CASE("PIECE_SHORT_NAMES correctness")
{
    REQUIRE(qb::PIECE_SHORT_NAMES[qb::PAWN] == "P");
    REQUIRE(qb::PIECE_SHORT_NAMES[5] == "K");
    REQUIRE(qb::PIECE_SHORT_NAMES[6] == "p");
}

TEST_CASE("MOVE_TYPE_NAMES correctness")
{
    REQUIRE(qb::MOVE_TYPE_NAMES[qb::NORMAL] == "Normal");
    REQUIRE(qb::MOVE_TYPE_NAMES[qb::PROMOTION] == "Promotion");
    REQUIRE(qb::MOVE_TYPE_NAMES[qb::CASTLING] == "Castling");
}
