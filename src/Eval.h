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

// ReSharper disable CppTooWideScopeInitStatement

#pragma once

#include "Board.h"
#include "MoveGenerator.h"

//-------------------------------------------------
// Eval
//-------------------------------------------------

namespace qb
{
    using PieceSquareTable = std::array<int, 64>;
    using PieceSquareTables = std::array<PieceSquareTable, 12>;

    inline constexpr PieceSquareTable MG_PAWN_TABLE
    {
        0,   0,   0,   0,   0,   0,  0,   0,
       98, 134,  61,  95,  68, 126, 34, -11,
       -6,   7,  26,  31,  65,  56, 25, -20,
      -14,  13,   6,  21,  23,  12, 17, -23,
      -27,  -2,  -5,  12,  17,   6, 10, -25,
      -26,  -4,  -4, -10,   3,   3, 33, -12,
      -35,  -1, -20, -23, -15,  24, 38, -22,
        0,   0,   0,   0,   0,   0,  0,   0,
    };

    inline constexpr PieceSquareTable EG_PAWN_TABLE
    {
        0,   0,   0,   0,   0,   0,   0,   0,
      178, 173, 158, 134, 147, 132, 165, 187,
       94, 100,  85,  67,  56,  53,  82,  84,
       32,  24,  13,   5,  -2,   4,  17,  17,
       13,   9,  -3,  -7,  -7,  -8,   3,  -1,
        4,   7,  -6,   1,   0,  -5,  -1,  -8,
       13,   8,   8,  10,  13,   0,   2,  -7,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    inline constexpr PieceSquareTable MG_KNIGHT_TABLE
    {
        -167, -89, -34, -49,  61, -97, -15, -107,
         -73, -41,  72,  36,  23,  62,   7,  -17,
         -47,  60,  37,  65,  84, 129,  73,   44,
          -9,  17,  19,  53,  37,  69,  18,   22,
         -13,   4,  16,  13,  28,  19,  21,   -8,
         -23,  -9,  12,  10,  19,  17,  25,  -16,
         -29, -53, -12,  -3,  -1,  18, -14,  -19,
        -105, -21, -58, -33, -17, -28, -19,  -23,
    };

    inline constexpr PieceSquareTable EG_KNIGHT_TABLE
    {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64,
    };

    inline constexpr PieceSquareTable MG_BISHOP_TABLE
    {
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -16,  37,  43,  40,  35,  50,  37,  -2,
         -4,   5,  19,  50,  37,  37,   7,  -2,
         -6,  13,  13,  26,  34,  12,  10,   4,
          0,  15,  15,  15,  14,  27,  18,  10,
          4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21,
    };

    inline constexpr PieceSquareTable EG_BISHOP_TABLE
    {
        -14, -21, -11,  -8, -7,  -9, -17, -24,
         -8,  -4,   7, -12, -3, -13,  -4, -14,
          2,  -8,   0,  -1, -2,   6,   0,   4,
         -3,   9,  12,   9, 14,  10,   3,   2,
         -6,   3,  13,  19,  7,  10,  -3,  -9,
        -12,  -3,   8,  10, 13,   3,  -7, -15,
        -14, -18,  -7,  -1,  4,  -9, -15, -27,
        -23,  -9, -23,  -5, -9, -16,  -5, -17,
    };

    inline constexpr PieceSquareTable MG_ROOK_TABLE
    {
        32,  42,  32,  51, 63,  9,  31,  43,
        27,  32,  58,  62, 80, 67,  26,  44,
        -5,  19,  26,  36, 17, 45,  61,  16,
       -24, -11,   7,  26, 24, 35,  -8, -20,
       -36, -26, -12,  -1,  9, -7,   6, -23,
       -45, -25, -16, -17,  3,  0,  -5, -33,
       -44, -16, -20,  -9, -1, 11,  -6, -71,
       -19, -13,   1,  17, 16,  7, -37, -26,
    };

    inline constexpr PieceSquareTable EG_ROOK_TABLE
    {
        13, 10, 18, 15, 12,  12,   8,   5,
        11, 13, 13, 11, -3,   3,   8,   3,
         7,  7,  7,  5,  4,  -3,  -5,  -3,
         4,  3, 13,  1,  2,   1,  -1,   2,
         3,  5,  8,  4, -5,  -6,  -8, -11,
        -4,  0, -5, -1, -7, -12,  -8, -16,
        -6, -6,  0,  2, -9,  -9, -11,  -3,
        -9,  2,  3, -1, -5, -13,   4, -20,
    };

    inline constexpr PieceSquareTable MG_QUEEN_TABLE
    {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
         -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
         -1, -18,  -9,  10, -15, -25, -31, -50,
    };

    inline constexpr PieceSquareTable EG_QUEEN_TABLE
    {
        -9,  22,  22,  27,  27,  19,  10,  20,
       -17,  20,  32,  41,  58,  25,  30,   0,
       -20,   6,   9,  49,  47,  35,  19,   9,
         3,  22,  24,  45,  57,  40,  57,  36,
       -18,  28,  19,  47,  31,  34,  39,  23,
       -16, -27,  15,   6,   9,  17,  10,   5,
       -22, -23, -30, -16, -16, -23, -36, -32,
       -33, -28, -22, -43,  -5, -32, -20, -41,
    };

    inline constexpr PieceSquareTable MG_KING_TABLE
    {
        -65,  23,  16, -15, -56, -34,   2,  13,
         29,  -1, -20,  -7,  -8,  -4, -38, -29,
         -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
          1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14,
    };

    inline constexpr PieceSquareTable EG_KING_TABLE
    {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
         10,  17,  23,  15,  20,  45,  44,  13,
         -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  27,  23,   9, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
    };

    inline constexpr std::array MG_PIECE_VALUE{ 82, 337, 365, 477, 1025, 0 };
    inline constexpr std::array EG_PIECE_VALUE{ 94, 281, 297, 512,  936, 0 };

    constexpr std::array MG_PST{ MG_PAWN_TABLE, MG_KNIGHT_TABLE, MG_BISHOP_TABLE, MG_ROOK_TABLE, MG_QUEEN_TABLE, MG_KING_TABLE };
    constexpr std::array EG_PST{ EG_PAWN_TABLE, EG_KNIGHT_TABLE, EG_BISHOP_TABLE, EG_ROOK_TABLE, EG_QUEEN_TABLE, EG_KING_TABLE };

    constexpr std::array PIECE_PHASE_WEIGHT
    {
        /* WP */ 0, /* BP */ 0,
        /* WN */ 1, /* BN */ 1, // 4x1
        /* WB */ 1, /* BB */ 1, // 4x1
        /* WR */ 2, /* BR */ 2, // 4x2
        /* WQ */ 4, /* BQ */ 4, // 2x4
        /* WK */ 0, /* BK */ 0
    };

    constexpr auto MAX_PHASE_WEIGHT{ 24 };

    inline PieceSquareTables g_mg_table{};
    inline PieceSquareTables g_eg_table{};

    /**
     * @brief Penalty values (in centipawns) for hanging pieces.
     *
     * A piece is considered *hanging* if it is currently attacked by an opponent’s piece
     * and not defended by any friendly piece.
     *
     * Indexed by PieceType:
     * - NO_PIECE  → 0
     * - PAWN      → 30
     * - KNIGHT    → 90
     * - BISHOP    → 90
     * - ROOK      → 150
     * - QUEEN     → 300
     * - KING      → 0 (the king is never penalized)
     */
    constexpr std::array HANGING_PENALTY_TABLE
    {
        0,   // NO_PIECE
        30,  // PAWN
        90,  // KNIGHT
        90,  // BISHOP
        150, // ROOK
        300, // QUEEN
        0    // KING
    };

    /**
     * @brief Initialise middlegame and endgame piece‑square tables.
     *
     * Combines the base material value (`MG_PIECE_VALUE` / `EG_PIECE_VALUE`)
     * with the PeSTO piece‑square offsets, mirrored for Black.
     *
     * @note Must be called exactly once before using qb::eval().
     */
    constexpr void init_tables() noexcept
    {
        for (const PieceType pieceType : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING })
        {
            const auto typeIdx{ static_cast<size_t>(pieceType) };

            for (const Color color : { WHITE, BLACK })
            {
                const auto encPiece{ encode_piece(pieceType, color) };

                for (auto square{ 0 }; square < 64; ++square)
                {
                    const auto finalSq{ (color == WHITE) ? square : flip_square(static_cast<Square>(square)) };

                    g_mg_table[encPiece][square] = MG_PIECE_VALUE[typeIdx] + MG_PST[typeIdx][finalSq];
                    g_eg_table[encPiece][square] = EG_PIECE_VALUE[typeIdx] + EG_PST[typeIdx][finalSq];
                }
            }
        }
    }

    /**
     * @brief Evaluates a position using tapered evaluation, including hanging piece penalties.
     *
     * The score is positive if the position favors the side to move, and negative otherwise.
     * Evaluation consists of:
     * 1. Accumulating middlegame and endgame scores from precomputed piece-square tables.
     * 2. Calculating the game phase (0 to MAX_PHASE_WEIGHT) based on remaining material.
     * 3. Interpolating between middlegame and endgame scores (“tapered eval”).
     * 4. Applying penalties for hanging pieces (attacked but undefended).
     *
     * @param t_board The current board.
     * @param t_moveGenerator A move generator instance.
     *
     * @return Evaluation score in centipawns from the perspective of the side to move.
     */
    [[nodiscard]] inline int eval(const Board& t_board, const MoveGenerator& t_moveGenerator) noexcept
    {
        std::array<int, 2> mg{}; // mg[WHITE], mg[BLACK]
        std::array<int, 2> eg{}; // eg[WHITE], eg[BLACK]
        auto weights{ 0 };

        for (auto square{ 0 }; square < 64; ++square)
        {
            const auto [type, color]{ t_board.positionState.pieceBoard[square] };
            if (type == NO_PIECE_TYPE)
            {
                continue;
            }

            const auto encPiece{ encode_piece(type, color) };

            mg[color] += g_mg_table[encPiece][square];
            eg[color] += g_eg_table[encPiece][square];

            weights += PIECE_PHASE_WEIGHT[encPiece];

            // Hanging piece penalty
            if (color == t_board.positionState.side)
            {
                const auto attackerSide{ opposite_color(color) };
                const auto attackers{ t_moveGenerator.CountAttackers(static_cast<Square>(square), attackerSide) };
                const auto defenders{ t_moveGenerator.CountAttackers(static_cast<Square>(square), color) };

                if (attackers > 0 && defenders == 0)
                {
                    const auto penalty{ HANGING_PENALTY_TABLE[type] };
                    mg[color] -= penalty;
                    eg[color] -= penalty / 2;
                }
            }
        }

        // tapered eval
        const auto mgScore{ mg[t_board.positionState.side] - mg[opposite_color(t_board.positionState.side)] };
        const auto egScore{ eg[t_board.positionState.side] - eg[opposite_color(t_board.positionState.side)] };
        const auto mgPhase{ std::min(weights, MAX_PHASE_WEIGHT) };
        const auto egPhase{ MAX_PHASE_WEIGHT - mgPhase };

        return (mgScore * mgPhase + egScore * egPhase) / MAX_PHASE_WEIGHT;
    }
}
