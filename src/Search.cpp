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

#include "Search.h"
#include "Eval.h"
#include "Board.h"
#include "MoveGenerator.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::Search::Search(Board* t_board, MoveGenerator* t_moveGenerator)
    : m_board{ t_board }
    , m_moveGenerator{ t_moveGenerator }
{
    QB_LOG_DEBUG("[Search::Search()] Create Search object.");
}

qb::Search::~Search() noexcept
{
    QB_LOG_DEBUG("[Search::~Search()] Destruct Search object.");
}

//-------------------------------------------------
// Search best move
//-------------------------------------------------

qb::Move qb::Search::SearchBestMove(const int t_depth)
{
    m_rootDepth = t_depth;
    m_nodes = 0;
    m_rootBestMove = Move{};
    m_rootScore = AlphaBeta(t_depth, -30000, 30000);

    return m_rootBestMove;
}

//-------------------------------------------------
// AlphaBeta
//-------------------------------------------------

int qb::Search::AlphaBeta(const int t_depth, int t_alpha, const int t_beta) // NOLINT(*-no-recursion)
{
    ++m_nodes;

    if (t_depth == 0)
    {
        return eval(*m_board, *m_moveGenerator);
    }

    const auto moves{ m_moveGenerator->GenerateLegalMoves() };
    if (moves.empty())
    {
        return m_board->IsKingInCheck(m_board->positionState.side)
            ? -30000 + m_ply // mate
            : 0;             // patt/remis
    }

    Move bestMove{};
    for (const auto& move : moves)
    {
        m_board->MakeMove(move);
        ++m_ply;
        const auto score{ -AlphaBeta(t_depth - 1, -t_beta, -t_alpha) };
        --m_ply;
        m_board->UnmakeMove();

        if (score >= t_beta)
        {
            return t_beta;
        }

        if (score > t_alpha)
        {
            t_alpha = score;
            bestMove = move;

            if (t_depth == m_rootDepth)
            {
                m_rootBestMove = bestMove;
            }
        }
    }

    return t_alpha;
}
