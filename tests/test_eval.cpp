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

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "Eval.h"

TEST_CASE("Eval penalizes hanging queen", "[eval][hanging][pesto]")
{
    qb::Board board;
    board.InitWithFen("4k3/8/8/8/4q3/8/8/4Q3 w - - 0 1");
    const qb::MoveGenerator moveGenerator(&board);

    const auto evalScore{ eval(board, moveGenerator) };
    INFO("Eval score: " << evalScore);
    REQUIRE(evalScore < -40);
}
