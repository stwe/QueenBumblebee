// This file is part of the QueenBumblebee project.
//
// Copyright (c) 2026. stwe <https://github.com/stwe/QueenBumblebee>
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

#include "Bitboard.h"

namespace qb
{
    //-------------------------------------------------
    // Piece string
    //-------------------------------------------------

    /**
     * @brief Creates a short string representation of a piece.
     *
     * @param t_pieceType The type of the piece.
     * @param t_color The color of the piece (defaults to NO_COLOR).
     *
     * @return A string representing the piece; empty if invalid.
     */
    [[nodiscard]] inline std::string_view create_short_piece_string(const PieceType t_pieceType, const Color t_color = NO_COLOR)
    {
        if (t_color == WHITE && t_pieceType != NO_PIECE_TYPE)
        {
            QB_ASSERT(is_valid_piece_type(t_pieceType), "[create_short_piece_string()] Invalid piece type for WHITE.");
            return { PIECE_SHORT_NAMES[t_pieceType] };
        }

        if (t_color == BLACK && t_pieceType != NO_PIECE_TYPE)
        {
            QB_ASSERT(is_valid_piece_type(t_pieceType), "[create_short_piece_string()] Invalid piece type for BLACK.");
            return { PIECE_SHORT_NAMES[t_pieceType + 6] };
        }

        return {};
    }

    /**
     * @brief Returns the string representation and color of the piece on the given square.
     *
     * @param t_file The file (column) of the square to check.
     * @param t_rank The rank (row) of the square to check.
     * @param t_bitboards The bitboard structure containing all piece positions.
     *
     * @return A pair consisting of:
     *         - A formatted string representation of the piece (or empty if no piece is present).
     *         - The color of the piece (`WHITE`, `BLACK`, or `NO_COLOR` if empty).
     */
    [[nodiscard]] inline std::pair<std::string, Color> get_piece_string(
        const File t_file,
        const Rank t_rank,
        const Bitboards& t_bitboards
    )
    {
        QB_ASSERT(is_valid_file(t_file), "[get_piece_string()] Invalid file.");
        QB_ASSERT(is_valid_rank(t_rank), "[get_piece_string()] Invalid rank.");

        auto formatPiece = [](const PieceType t_type, const Color t_color) -> std::pair<std::string, Color> {
            return { " " + std::string(create_short_piece_string(t_type, t_color)) + " ", t_color };
        };

        if (is_bit_set(t_bitboards.whitePawns, t_file, t_rank)) return formatPiece(PAWN, WHITE);
        if (is_bit_set(t_bitboards.whiteKnights, t_file, t_rank)) return formatPiece(KNIGHT, WHITE);
        if (is_bit_set(t_bitboards.whiteBishops, t_file, t_rank)) return formatPiece(BISHOP, WHITE);
        if (is_bit_set(t_bitboards.whiteRooks, t_file, t_rank)) return formatPiece(ROOK, WHITE);
        if (is_bit_set(t_bitboards.whiteQueens, t_file, t_rank)) return formatPiece(QUEEN, WHITE);
        if (is_bit_set(t_bitboards.whiteKing, t_file, t_rank)) return formatPiece(KING, WHITE);

        if (is_bit_set(t_bitboards.blackPawns, t_file, t_rank)) return formatPiece(PAWN, BLACK);
        if (is_bit_set(t_bitboards.blackKnights, t_file, t_rank)) return formatPiece(KNIGHT, BLACK);
        if (is_bit_set(t_bitboards.blackBishops, t_file, t_rank)) return formatPiece(BISHOP, BLACK);
        if (is_bit_set(t_bitboards.blackRooks, t_file, t_rank)) return formatPiece(ROOK, BLACK);
        if (is_bit_set(t_bitboards.blackQueens, t_file, t_rank)) return formatPiece(QUEEN, BLACK);
        if (is_bit_set(t_bitboards.blackKing, t_file, t_rank)) return formatPiece(KING, BLACK);

        return { "   ", NO_COLOR };
    }

    //-------------------------------------------------
    // Bitboard string
    //-------------------------------------------------

    /**
     * @brief Converts a bitboard to its string representation, showing the board layout.
     *
     * @param t_bitboard The bitboard to convert.
     *
     * @return A string representation of the bitboard in chessboard format.
     */
    [[nodiscard]] inline std::string bitboard_to_string(const U64 t_bitboard)
    {
        std::string result;
        result.reserve(120);

        for (int rank{ RANK_8 }; rank >= RANK_1; --rank)
        {
            result += static_cast<char>('1' + rank);
            result += '|';

            for (int file{ FILE_A }; file <= FILE_H; ++file)
            {
                result += is_bit_set(t_bitboard, static_cast<File>(file), static_cast<Rank>(rank)) ? '1' : '0';
            }

            result += '\n';
        }

        result += "  --------\n";
        result += "  ABCDEFGH\n";

        return result;
    }
}
