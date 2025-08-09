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

#include "Move.h"

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
    // Search
    //-------------------------------------------------

    class Search
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Search() = delete;

        Search(Board* t_board, MoveGenerator* t_moveGenerator);

        Search(const Search& t_other) = delete;
        Search(Search&& t_other) noexcept = delete;
        Search& operator=(const Search& t_other) = delete;
        Search& operator=(Search&& t_other) noexcept = delete;

        ~Search() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] int GetScore() const { return m_rootScore; }
        [[nodiscard]] uint64_t GetNodesSearched() const { return m_nodes; }

        //-------------------------------------------------
        // Search best move
        //-------------------------------------------------

        [[nodiscard]] Move SearchBestMove(int t_depth);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        Board* m_board{ nullptr };
        MoveGenerator* m_moveGenerator{ nullptr };
        int m_rootDepth{ 0 };
        Move m_rootBestMove{};
        int m_rootScore{ 0 };
        uint64_t m_nodes{ 0 };
        int m_ply{ 0 };

        //-------------------------------------------------
        // AlphaBeta
        //-------------------------------------------------

        [[nodiscard]] int AlphaBeta(int t_depth, int t_alpha, int t_beta);
    };
}
