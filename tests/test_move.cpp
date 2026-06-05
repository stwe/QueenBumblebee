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

#include <catch2/catch_test_macros.hpp>
#include "Move.h"

TEST_CASE("Move - Size", "[Move]")
{
    REQUIRE(sizeof(qb::Move) == 4);
}

TEST_CASE("Move - ToString", "[Move]")
{
    constexpr qb::Move M1(qb::SQ_E2, qb::SQ_E4, qb::PAWN);
    REQUIRE(M1.ToString() == "e2e4");

    constexpr qb::Move M2(qb::SQ_E7, qb::SQ_E8, qb::PAWN, qb::QUEEN, qb::PROMOTION);
    REQUIRE(M2.ToString() == "e7e8q");
}

TEST_CASE("Move - Basic Construction and Getters", "[Move]")
{
    qb::Move m(qb::SQ_A2, qb::SQ_A4, qb::PAWN);

    REQUIRE(m.FromSquare() == qb::SQ_A2);
    REQUIRE(m.ToSquare() == qb::SQ_A4);
    REQUIRE(m.TypeOfPiece() == qb::PAWN);
    REQUIRE(m.TypeOfPromotionPiece() == qb::NO_PIECE_TYPE);
    REQUIRE(m.TypeOfMove() == qb::NORMAL);
    REQUIRE(m.TypeOfCapturedPiece() == qb::NO_PIECE_TYPE);
}

TEST_CASE("Move - With Capture", "[Move]")
{
    constexpr qb::Move M(qb::SQ_F3, qb::SQ_G1, qb::KNIGHT, qb::NO_PIECE_TYPE, qb::NORMAL, qb::ROOK);

    REQUIRE(M.TypeOfCapturedPiece() == qb::ROOK);
    REQUIRE(M.TypeOfPiece() == qb::KNIGHT);
    REQUIRE(M.TypeOfMove() == qb::NORMAL);
}

TEST_CASE("Move - Promotion", "[Move]")
{
    constexpr qb::Move M(qb::SQ_E7, qb::SQ_E8, qb::PAWN, qb::QUEEN, qb::PROMOTION);

    REQUIRE(M.FromSquare() == qb::SQ_E7);
    REQUIRE(M.ToSquare() == qb::SQ_E8);
    REQUIRE(M.TypeOfPiece() == qb::PAWN);
    REQUIRE(M.TypeOfPromotionPiece() == qb::QUEEN);
    REQUIRE(M.TypeOfMove() == qb::PROMOTION);
}

TEST_CASE("Move - Castling", "[Move]")
{
    constexpr qb::Move M(qb::SQ_E8, qb::SQ_G8, qb::KING, qb::NO_PIECE_TYPE, qb::CASTLING);

    REQUIRE(M.TypeOfMove() == qb::CASTLING);
    REQUIRE(M.TypeOfPiece() == qb::KING);
}
