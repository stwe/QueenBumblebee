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
#define QB_ASSERT_TEST_MODE

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "MoveGenerator.h"
#include "Board.h"

TEST_CASE("Black Queen attacks white King on F4", "[QueenF4]")
{
    qb::Bitboards bb{};
    bb.whiteKing = qb::F4;
    bb.blackQueens = qb::C1;

    qb::Board board{};
    board.InitWithBitboards(bb, {});

    const qb::MoveGenerator moveGen{ &board };
    const U64 attacks{ moveGen.GenerateAttackedSquares(qb::BLACK) };

    REQUIRE(attacks == 289360691890294523ULL);
    REQUIRE(is_bit_set(attacks, qb::SQ_F4));
}

TEST_CASE("Black Rooks attacks white King on F4", "[RooksF4]")
{
    qb::Bitboards bb{};
    bb.whiteKing = qb::F4;
    bb.blackRooks = qb::A4 | qb::F2;

    qb::Board board{};
    board.InitWithBitboards(bb, {});

    const qb::MoveGenerator moveGen{ &board };
    const U64 attacks{ moveGen.GenerateAttackedSquares(qb::BLACK) };

    REQUIRE(attacks == 72340173863640865ULL);
    REQUIRE(is_bit_set(attacks, qb::SQ_F4));
}

TEST_CASE("White Bishops attacks black King on C3", "[BishopsC3]")
{
    qb::Bitboards bb{};
    bb.blackKing = qb::C3;
    bb.whiteBishops = qb::A1 | qb::E1;

    qb::Board board{};
    board.InitWithBitboards(bb, {});

    const qb::MoveGenerator moveGen{ &board };
    const U64 attacks{ moveGen.GenerateAttackedSquares(qb::WHITE) };

    REQUIRE(attacks == 2151950848ULL);
    REQUIRE(is_bit_set(attacks, qb::SQ_C3));
}


TEST_CASE("White Pawns and Knights attacks black King on C3", "[PawnsKnightsC3]")
{
    qb::Bitboards bb{};
    bb.blackKing = qb::C3;
    bb.whitePawns = qb::B2 | qb::D2;
    bb.whiteKnights = qb::E2 | qb::D5;

    qb::Board board{};
    board.InitWithBitboards(bb, {});

    const qb::MoveGenerator moveGen{ &board };
    const U64 attacks{ moveGen.GenerateAttackedSquares(qb::WHITE) };

    REQUIRE(attacks == 5666883639771204ULL);
    REQUIRE(is_bit_set(attacks, qb::SQ_C3));
}

TEST_CASE("IsSquareAttacked handles all piece types correctly", "[movegen]")
{
    SECTION("Pawn attacks")
    {
        qb::Bitboards bb{};
        bb.whitePawns = qb::E4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D5));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_F5));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_E5));
    }

    SECTION("Knight attacks")
    {
        qb::Bitboards bb{};
        bb.whiteKnights = qb::D4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_C6));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_E6));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_B5));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D6));
    }

    SECTION("King attacks")
    {
        qb::Bitboards bb{};
        bb.whiteKing = qb::E4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D4));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_F5));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_G6));
    }

    SECTION("Bishop attacks (unblocked)")
    {
        qb::Bitboards bb{};
        bb.whiteBishops = qb::D4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_C3));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_F6));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D5));
    }

    SECTION("Bishop attacks (blocked)")
    {
        qb::Bitboards bb{};
        bb.whiteBishops = qb::D4;
        bb.blackPawns = qb::E5;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_F6));
    }

    SECTION("Rook attacks (unblocked)")
    {
        qb::Bitboards bb{};
        bb.whiteRooks = qb::D4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D6));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_G4));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_E5));
    }

    SECTION("Rook attacks (blocked)")
    {
        qb::Bitboards bb{};
        bb.whiteRooks = qb::D4;
        bb.blackPawns = qb::D5;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D6));
    }

    SECTION("Queen attacks (combined bishop/rook logic)")
    {
        qb::Bitboards bb{};
        bb.whiteQueens = qb::D4;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_G7));
        REQUIRE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D7));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_B5));
    }

    SECTION("Queen attacks (blocked)")
    {
        qb::Bitboards bb{};
        bb.whiteQueens = qb::D4;
        bb.blackPawns = qb::D5;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_D6));
    }

    SECTION("Enemy color checks only their attacks")
    {
        qb::Bitboards bb{};
        bb.whiteKing = qb::E1;
        bb.blackQueens = qb::E8;

        qb::Board board{};
        board.InitWithBitboards(bb, {});

        const qb::MoveGenerator moveGen{ &board };

        REQUIRE(board.IsSquareAttacked(qb::BLACK, qb::SQ_E1));
        REQUIRE_FALSE(board.IsSquareAttacked(qb::WHITE, qb::SQ_E1));
    }
}

TEST_CASE("CountAttackers correctly counts attacks", "[movegen][attackers][count]")
{
    SECTION("CountAttackers correctly counts pawn attacks")
    {
        qb::Board board;
        board.InitWithFen("4k3/1p6/P1P5/3ppp2/2P1P3/8/8/4K3 b - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_E4, qb::BLACK) == 2);
        REQUIRE(moveGen.CountAttackers(qb::SQ_C4, qb::BLACK) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_A6, qb::BLACK) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_C6, qb::BLACK) == 1);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B7, qb::WHITE) == 2);
        REQUIRE(moveGen.CountAttackers(qb::SQ_D5, qb::WHITE) == 2);
        REQUIRE(moveGen.CountAttackers(qb::SQ_E5, qb::WHITE) == 0);
        REQUIRE(moveGen.CountAttackers(qb::SQ_F5, qb::WHITE) == 1);
    }

    SECTION("CountAttackers correctly counts king attacks")
    {
        qb::Board board;
        board.InitWithFen("8/1P6/1k6/1P3p2/5K2/5p2/8/8 b - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B7, qb::BLACK) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_B5, qb::BLACK) == 1);

        REQUIRE(moveGen.CountAttackers(qb::SQ_F5, qb::WHITE) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_F3, qb::WHITE) == 1);
    }

    SECTION("CountAttackers correctly counts knight attacks")
    {
        qb::Board board;
        board.InitWithFen("N3k3/3N4/1p6/6n1/4P3/8/3n4/4K3 b - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_E4, qb::BLACK) == 2);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B6, qb::WHITE) == 2);
    }

    SECTION("CountAttackers correctly counts non sliding attacks")
    {
        qb::Board board;
        board.InitWithFen("8/3pk3/4P3/2n5/4P1N1/4p3/4KP2/8 w - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_E6, qb::BLACK) == 3);

        REQUIRE(moveGen.CountAttackers(qb::SQ_E3, qb::WHITE) == 3);
    }
}

TEST_CASE("CountAttackers correctly counts attacks", "[movegen][attackers][sliding][count]")
{
    SECTION("CountAttackers correctly counts bishop attacks")
    {
        qb::Board board;
        board.InitWithFen("3bk3/8/1P6/8/2p5/8/8/4KB2 w - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B6, qb::BLACK) == 1);

        REQUIRE(moveGen.CountAttackers(qb::SQ_C4, qb::WHITE) == 1);
    }

    SECTION("CountAttackers correctly counts queen attacks")
    {
        qb::Board board;
        board.InitWithFen("3qk3/8/1P1P4/8/2p5/5p2/8/4KQ2 w - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B6, qb::BLACK) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_D6, qb::BLACK) == 1);

        REQUIRE(moveGen.CountAttackers(qb::SQ_C4, qb::WHITE) == 1);
        REQUIRE(moveGen.CountAttackers(qb::SQ_F3, qb::WHITE) == 1);
    }

    SECTION("CountAttackers correctly counts rook attacks")
    {
        qb::Board board;
        board.InitWithFen("4k3/8/rP6/8/8/5p1R/8/1r2KR2 w - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B6, qb::BLACK) == 2);

        REQUIRE(moveGen.CountAttackers(qb::SQ_F3, qb::WHITE) == 2);
    }

    SECTION("CountAttackers correctly counts sliding attacks")
    {
        qb::Board board;
        board.InitWithFen("3qk3/8/rP6/7B/3b1Q2/5p1R/8/1r2KR2 w - - 0 1");

        const qb::MoveGenerator moveGen(&board);

        REQUIRE(moveGen.CountAttackers(qb::SQ_B6, qb::BLACK) == 4);

        REQUIRE(moveGen.CountAttackers(qb::SQ_F3, qb::WHITE) == 4);
    }
}
