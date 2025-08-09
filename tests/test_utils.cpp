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
#include "Utils.h"

//-------------------------------------------------
// Validation
//-------------------------------------------------

TEST_CASE("is_valid works correctly", "[is_valid_square]")
{
    REQUIRE(qb::is_valid_square(qb::SQ_A1));
    REQUIRE(qb::is_valid_square(qb::SQ_H8));
    REQUIRE_FALSE(qb::is_valid_square(qb::SQ_NONE));
    REQUIRE_FALSE(qb::is_valid_square(static_cast<qb::Square>(64)));
}

//-------------------------------------------------
// Square utils
//-------------------------------------------------

TEST_CASE("make_square works correctly", "[make_square]")
{
    REQUIRE(qb::make_square(qb::FILE_A, qb::RANK_1) == qb::SQ_A1);
    REQUIRE(qb::make_square(qb::FILE_H, qb::RANK_8) == qb::SQ_H8);
    REQUIRE(qb::make_square(qb::FILE_D, qb::RANK_4) == qb::SQ_D4);
}

TEST_CASE("flip_square vertically mirrors a square index", "[flip_square]")
{
    REQUIRE(qb::flip_square(qb::SQ_A1) == qb::SQ_A8);
    REQUIRE(qb::flip_square(qb::SQ_H1) == qb::SQ_H8);

    REQUIRE(qb::flip_square(qb::SQ_B2) == qb::SQ_B7);
    REQUIRE(qb::flip_square(qb::SQ_D4) == qb::SQ_D5);

    REQUIRE(qb::flip_square(qb::SQ_E8) == qb::SQ_E1);
}

TEST_CASE("file_of and rank_of work correctly", "[file_rank_of]")
{
    for (int rank{ qb::RANK_1 }; rank <= qb::RANK_8; ++rank)
    {
        for (int file{ qb::FILE_A }; file <= qb::FILE_H; ++file)
        {
            const auto sq{ qb::make_square(static_cast<qb::File>(file), static_cast<qb::Rank>(rank)) };
            REQUIRE(file_of(sq) == file);
            REQUIRE(rank_of(sq) == rank);
        }
    }
}

TEST_CASE("diagonal_index and antidiagonal_index produce correct values", "[diagonals]")
{
    // Diagonale rank + file
    REQUIRE(qb::diagonal_index(qb::SQ_A1) == 0);   // a1
    REQUIRE(qb::diagonal_index(qb::SQ_B2) == 2);   // b2
    REQUIRE(qb::diagonal_index(qb::SQ_H8) == 14);  // h8

    // Anti‑Diagonale 7 + file − rank
    REQUIRE(qb::antidiagonal_index(qb::SQ_H1) == 14); // h1
    REQUIRE(qb::antidiagonal_index(qb::SQ_G2) == 12); // g2
    REQUIRE(qb::antidiagonal_index(qb::SQ_A8) == 0);  // a8
}

//-------------------------------------------------
// Color
//-------------------------------------------------

TEST_CASE("opposite_color works correctly", "[opposite_color]")
{
    REQUIRE(qb::opposite_color(qb::WHITE) == qb::BLACK);
    REQUIRE(qb::opposite_color(qb::BLACK) == qb::WHITE);
}

//-------------------------------------------------
// Piece
//-------------------------------------------------

TEST_CASE("encode_piece maps every (piece, color) pair to a unique index 0-11", "[encode_piece]")
{
    REQUIRE(qb::encode_piece(qb::PieceType::PAWN, qb::Color::WHITE) == 0);
    REQUIRE(qb::encode_piece(qb::PieceType::PAWN, qb::Color::BLACK) == 1);

    REQUIRE(qb::encode_piece(qb::PieceType::KNIGHT, qb::Color::WHITE) == 2);
    REQUIRE(qb::encode_piece(qb::PieceType::KNIGHT, qb::Color::BLACK) == 3);

    REQUIRE(qb::encode_piece(qb::PieceType::KING, qb::Color::WHITE) == 10);
    REQUIRE(qb::encode_piece(qb::PieceType::KING, qb::Color::BLACK) == 11);
}
