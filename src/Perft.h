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

#pragma once

#include <cstdint>

namespace qb
{
    //-------------------------------------------------
    // Forward declarations
    //-------------------------------------------------

    /** @brief Forward declaration class Board. */
    class Board;

    /** @brief Forward declaration class MoveGenerator. */
    class MoveGenerator;

    //-------------------------------------------------
    // Perft
    //-------------------------------------------------

    /**
     * @brief Class for performing performance tests (Perft) for chess moves.
     *
     * This class implements performance testing for the chess engine.
     * Perft (Performance Test) counts all possible legal moves to a certain depth,
     * which is used for debugging the move generator.
     */
    class Perft
    {
    public:
        /**
         * @brief Executes a Perft test and outputs the result.
         *
         * @param t_board Pointer to the current chess board.
         * @param t_moveGenerator Pointer to the move generator.
         * @param t_depth The search depth for the Perft test.
         *
         * @return The number of legal positions.
         */
        static uint64_t RunPerft(Board* t_board, MoveGenerator* t_moveGenerator, int t_depth);

    protected:

    private:
        /**
         * @brief Recursive helper function for the Perft test.
         *
         * @param t_board Pointer to the current chess board.
         * @param t_moveGenerator Pointer to the move generator.
         * @param t_depth The current search depth.
         *
         * @return The number of positions found.
         */
        static uint64_t Run(Board* t_board, MoveGenerator* t_moveGenerator, int t_depth);
    };
}
