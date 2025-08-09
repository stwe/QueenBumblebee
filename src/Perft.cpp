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

#include "Perft.h"
#include "Board.h"
#include "MoveGenerator.h"

uint64_t qb::Perft::Run(Board* t_board, MoveGenerator* t_moveGenerator, const int t_depth) // NOLINT(*-no-recursion)
{
    if (t_depth == 0)
    {
        return 1;
    }

    const auto moves{ t_moveGenerator->GenerateLegalMoves() };

    uint64_t nodes{ 0 };
    for (const auto& move : moves)
    {
        t_board->MakeMove(move);
        nodes += Run(t_board, t_moveGenerator, t_depth - 1);
        t_board->UnmakeMove();
    }

    return nodes;
}

uint64_t qb::Perft::RunPerft(Board* t_board, MoveGenerator* t_moveGenerator, const int t_depth)
{
    const auto moves{ t_moveGenerator->GenerateLegalMoves() };

    uint64_t totalNodes{ 0 };
    for (const auto& move : moves)
    {
        t_board->MakeMove(move);
        const auto nodes{ Run(t_board, t_moveGenerator, t_depth - 1) };
        t_board->UnmakeMove();

        totalNodes += nodes;
        //std::cout << move << ": " << nodes << std::endl;
    }

    return totalNodes;
}
