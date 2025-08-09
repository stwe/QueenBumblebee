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

#include <catch2/catch_test_macros.hpp>
#include "Board.h"
#include "Fen.h"

TEST_CASE("Promotion without capture", "[move][promotion]")
{
    qb::Board board;
    board.InitWithFen("4k3/P7/8/8/8/8/8/4K3 w - - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_A7,
        qb::SQ_A8,
        qb::PAWN,
        qb::QUEEN,
        qb::PROMOTION,
        qb::NO_PIECE_TYPE
    );
    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::QUEEN);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A7].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::NO_PIECE_TYPE);
}

TEST_CASE("Promotion with capture", "[move][promotion][capture]")
{
    qb::Board board;
    board.InitWithFen("r3k3/1P6/8/8/8/8/8/4K3 w - - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_B7,
        qb::SQ_A8,
        qb::PAWN,
        qb::QUEEN,
        qb::PROMOTION,
        qb::ROOK
    );
    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::QUEEN);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_B7].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::ROOK);
}

TEST_CASE("Normal capture", "[move][capture]")
{
    qb::Board board;
    board.InitWithFen("4k3/8/8/4p3/5P2/8/8/4K3 w - - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_F4,
        qb::SQ_E5,
        qb::PAWN,
        qb::NO_PIECE_TYPE,
        qb::NORMAL,
        qb::PAWN
    );
    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E5].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F4].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E5].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F4].type == qb::PAWN);
}

TEST_CASE("En passant", "[move][enpassant]")
{
    qb::Board board;
    board.InitWithFen("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_E5,
        qb::SQ_D6,
        qb::PAWN,
        qb::NO_PIECE_TYPE,
        qb::EN_PASSANT,
        qb::PAWN
    );

    REQUIRE(board.positionState.enPassantSquare == qb::SQ_D6);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D6].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E5].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D5].type == qb::PAWN);

    board.MakeMove(MOVE);
    REQUIRE(board.positionState.enPassantSquare == qb::SQ_NONE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D6].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E5].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D5].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.enPassantSquare == qb::SQ_D6);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D6].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E5].type == qb::PAWN);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D5].type == qb::PAWN);
}

TEST_CASE("White king‑side castling", "[move][castling][white][OO]")
{
    qb::Board board;
    board.InitWithFen("4k3/8/8/8/8/8/8/R3K2R w K - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_E1,
        qb::SQ_G1,
        qb::KING,
        qb::NO_PIECE_TYPE,
        qb::CASTLING,
        qb::NO_PIECE_TYPE
    );

    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H1].type == qb::ROOK);

    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_G1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F1].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H1].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H1].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F1].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_G1].type == qb::NO_PIECE_TYPE);
}

TEST_CASE("White queen‑side castling", "[move][castling][white][OOO]")
{
    qb::Board board;
    board.InitWithFen("4k3/8/8/8/8/8/8/R3K2R w Q - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_E1,
        qb::SQ_C1,
        qb::KING,
        qb::NO_PIECE_TYPE,
        qb::CASTLING,
        qb::NO_PIECE_TYPE
    );

    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A1].type == qb::ROOK);

    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_C1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D1].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A1].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E1].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A1].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D1].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_C1].type == qb::NO_PIECE_TYPE);
}

TEST_CASE("Black king‑side castling", "[move][castling][black][OO]")
{
    qb::Board board;
    board.InitWithFen("r3k2r/8/8/8/8/8/8/4K3 b k - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_E8,
        qb::SQ_G8,
        qb::KING,
        qb::NO_PIECE_TYPE,
        qb::CASTLING,
        qb::NO_PIECE_TYPE
    );

    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H8].type == qb::ROOK);

    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_G8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F8].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H8].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_H8].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_F8].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_G8].type == qb::NO_PIECE_TYPE);
}

TEST_CASE("Black queen‑side castling", "[move][castling][black][OOO]")
{
    qb::Board board;
    board.InitWithFen("r3k2r/8/8/8/8/8/8/4K3 b q - 0 1");

    constexpr qb::Move MOVE(
        qb::SQ_E8,
        qb::SQ_C8,
        qb::KING,
        qb::NO_PIECE_TYPE,
        qb::CASTLING,
        qb::NO_PIECE_TYPE
    );

    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::ROOK);

    board.MakeMove(MOVE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_C8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D8].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::NO_PIECE_TYPE);

    board.UnmakeMove();
    REQUIRE(board.positionState.pieceBoard[qb::SQ_E8].type == qb::KING);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_A8].type == qb::ROOK);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_D8].type == qb::NO_PIECE_TYPE);
    REQUIRE(board.positionState.pieceBoard[qb::SQ_C8].type == qb::NO_PIECE_TYPE);
}

TEST_CASE("Halfmove clock resets on pawn move or capture, increments otherwise", "[board][halfmove]")
{
    qb::Board board;
    board.InitWithFen(std::string(qb::Fen::FEN_START));

    // 1. Initially: halfmoveClock = 0
    REQUIRE(board.positionState.halfmoveClock == 0);

    // 2. Normal pawn move -> resets halfmoveClock
    qb::Move pawnMove(qb::SQ_E2, qb::SQ_E4, qb::PAWN);
    board.MakeMove(pawnMove);
    REQUIRE(board.positionState.halfmoveClock == 0);

    // 3. Normal non-capture move (knight) -> increments halfmoveClock
    qb::Move knightMove(qb::SQ_G8, qb::SQ_F6, qb::KNIGHT);
    board.MakeMove(knightMove);
    REQUIRE(board.positionState.halfmoveClock == 1);

    // 4. Capture move -> resets halfmoveClock
    // Note: This move is not checked for legality, so it's represented as a capture here.
    qb::Move captureMove(qb::SQ_E4, qb::SQ_F6, qb::PAWN, qb::NO_PIECE_TYPE, qb::NORMAL, qb::KNIGHT);
    board.MakeMove(captureMove);
    REQUIRE(board.positionState.halfmoveClock == 0);

    // 5. Other normal non-capture, non-pawn move -> increments halfmoveClock
    qb::Move bishopMove(qb::SQ_F1, qb::SQ_C4, qb::BISHOP);
    board.MakeMove(bishopMove);
    REQUIRE(board.positionState.halfmoveClock == 1);
}

TEST_CASE("Fullmove number increments after Black's move", "[board][fullmove]")
{
    qb::Board board;
    board.InitWithFen(std::string(qb::Fen::FEN_START));

    auto initialFullmove{ board.positionState.fullmoveNumber };

    // White moves (first move of the game)
    qb::Move whiteMove(qb::SQ_E2, qb::SQ_E4, qb::PAWN);
    board.MakeMove(whiteMove);
    REQUIRE(board.positionState.fullmoveNumber == initialFullmove);

    // Black moves -> fullmoveNumber increments
    qb::Move blackMove(qb::SQ_E7, qb::SQ_E5, qb::PAWN);
    board.MakeMove(blackMove);
    REQUIRE(board.positionState.fullmoveNumber == initialFullmove + 1);

    // White moves again -> fullmoveNumber remains the same
    qb::Move whiteMove2(qb::SQ_G1, qb::SQ_F3, qb::KNIGHT);
    board.MakeMove(whiteMove2);
    REQUIRE(board.positionState.fullmoveNumber == initialFullmove + 1);

    // Black moves again -> fullmoveNumber increments again
    qb::Move blackMove2(qb::SQ_B8, qb::SQ_C6, qb::KNIGHT);
    board.MakeMove(blackMove2);
    REQUIRE(board.positionState.fullmoveNumber == initialFullmove + 2);
}

TEST_CASE("UnmakeMove restores halfmove and fullmove clocks correctly", "[board][counter][undo]")
{
    qb::Board board;
    board.InitWithFen(std::string(qb::Fen::FEN_START));

    auto initialFullmove{ board.positionState.fullmoveNumber };

    // First move (White)
    qb::Move move1(qb::SQ_E2, qb::SQ_E4, qb::PAWN);
    board.MakeMove(move1);

    // Second move (Black)
    qb::Move move2(qb::SQ_E7, qb::SQ_E5, qb::PAWN);
    board.MakeMove(move2);

    // Unmake Black's move
    board.UnmakeMove();

    REQUIRE(board.positionState.fullmoveNumber == initialFullmove);
    REQUIRE(board.positionState.halfmoveClock == 0);  // halfmove clock resets after pawn move

    // Unmake White's move
    board.UnmakeMove();

    REQUIRE(board.positionState.fullmoveNumber == initialFullmove);
    REQUIRE(board.positionState.halfmoveClock == 0);
}
