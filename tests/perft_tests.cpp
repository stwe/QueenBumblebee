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
#include "MoveGenerator.h"
#include "Perft.h"

/**
 * @brief Convenience POD to bundle a FEN string, search depth and the expected perft node count.
 */
struct PerftCase
{
    std::string fen;
    int depth;
    uint64_t expectedNodes;
};

/**
 * @brief Perft positions.
 */
const std::array cases
{
    PerftCase{
        // Initial Position
        .fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        .depth = 4,
        .expectedNodes = 197281,
    },
    PerftCase{
        // Kiwipete
        .fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        .depth = 4,
        .expectedNodes = 4085603,
    },
    PerftCase{
        // discover promotion bugs
        .fen = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1",
        .depth = 4,
        .expectedNodes = 182838
    },
    PerftCase{
        .fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        .depth = 5,
        .expectedNodes = 674624,
    },
    PerftCase{
        .fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        .depth = 5,
        .expectedNodes = 15833292,
    },
    PerftCase{
        .fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        .depth = 4,
        .expectedNodes = 2103487,
    },
    PerftCase{
        .fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        .depth = 4,
        .expectedNodes = 3894594,
    }
};

TEST_CASE("Perft Tests", "[perft]")
{
    for (const auto& [fen, depth, expectedNodes] : cases)
    {
        qb::Board board;
        REQUIRE(board.InitWithFen(fen));

        qb::MoveGenerator moveGenerator(&board);
        const auto nodes{ qb::Perft::RunPerft(&board, &moveGenerator, depth) };

        INFO("FEN:   " << fen);
        INFO("Depth: " << depth);
        REQUIRE(nodes == expectedNodes);
    }
}
