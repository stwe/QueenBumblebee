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
#include "Fen.h"

TEST_CASE("Valid FEN is constructed and parsed correctly", "[Fen]")
{
    const std::string fenString{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    qb::Fen fen(fenString);
    REQUIRE(fen.Parse());

    SECTION("Piece placement has 8 ranks")
    {
        REQUIRE(fen.GetPiecePlacement().size() == 8);
    }

    SECTION("Side to move is white")
    {
        REQUIRE(fen.GetSideToMove() == qb::WHITE);
    }

    SECTION("All castling rights are set")
    {
        qb::CastlingRights expected{ qb::WHITE_OO | qb::WHITE_OOO | qb::BLACK_OO | qb::BLACK_OOO };
        REQUIRE(fen.GetCastlingRights() == expected);
    }

    SECTION("No en passant square")
    {
        REQUIRE(fen.GetEnPassantSquare() == qb::SQ_NONE);
    }
}

TEST_CASE("Default Fen constructor initializes with FEN_START", "[Fen]")
{
    qb::Fen defaultFen;
    REQUIRE(defaultFen.Parse());

    SECTION("Piece placement matches FEN_START")
    {
        REQUIRE(defaultFen.GetPiecePlacement().size() == 8);
    }

    SECTION("Side to move is white as in FEN_START")
    {
        REQUIRE(defaultFen.GetSideToMove() == qb::WHITE);
    }

    SECTION("All castling rights are set as in FEN_START")
    {
        qb::CastlingRights expected{ qb::WHITE_OO | qb::WHITE_OOO | qb::BLACK_OO | qb::BLACK_OOO };
        REQUIRE(defaultFen.GetCastlingRights() == expected);
    }

    SECTION("En passant square is not set as in FEN_START")
    {
        REQUIRE(defaultFen.GetEnPassantSquare() == qb::SQ_NONE);
    }
}

TEST_CASE("Invalid FEN strings fail parsing", "[Fen]")
{
    SECTION("Too few fields")
    {
        qb::Fen fen("invalid");
        REQUIRE_FALSE(fen.Parse());
    }

    SECTION("Invalid piece placement (too few ranks)")
    {
        qb::Fen fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP w KQkq -");
        REQUIRE_FALSE(fen.Parse());
    }

    SECTION("Invalid side to move")
    {
        qb::Fen fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq -");
        REQUIRE_FALSE(fen.Parse());
    }

    SECTION("Invalid castling character")
    {
        qb::Fen fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KRxq -");
        REQUIRE_FALSE(fen.Parse());
    }

    SECTION("Invalid en passant square")
    {
        qb::Fen fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq zz");
        REQUIRE_FALSE(fen.Parse());
    }
}
