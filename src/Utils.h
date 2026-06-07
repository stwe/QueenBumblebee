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

// ReSharper disable CppDFAUnreachableFunctionCall

#pragma once

#include "Assert.h"
#include "Types.h"

//-------------------------------------------------
// Common utils
//-------------------------------------------------

namespace qb
{
    //-------------------------------------------------
    // Validation
    //-------------------------------------------------

    /**
     * @brief Checks whether a given Square is valid.
     *
     * @param t_square The Square to validate.
     *
     * @return True if the Square is valid; otherwise, false.
     */
    [[nodiscard]] constexpr bool is_valid_square(const Square t_square)
    {
        return t_square >= SQ_A1 && t_square < SQ_NONE;
    }

    /**
     * @brief Checks whether a given File is valid.
     *
     * @param t_file The File to validate.
     *
     * @return True if the File is valid; otherwise, false.
     */
    [[nodiscard]] constexpr bool is_valid_file(const File t_file)
    {
        return t_file >= FILE_A && t_file <= FILE_H;
    }

    /**
     * @brief Checks whether a given Rank is valid.
     *
     * @param t_rank The Rank to validate.
     *
     * @return True if the Rank is valid; otherwise, false.
     */
    [[nodiscard]] constexpr bool is_valid_rank(const Rank t_rank)
    {
        return t_rank >= RANK_1 && t_rank <= RANK_8;
    }

    /**
     * @brief Checks whether a given PieceType is valid.
     *
     * @param t_pieceType The PieceType to validate.
     *
     * @return True if the PieceType is valid; otherwise, false.
     */
    [[nodiscard]] constexpr bool is_valid_piece_type(const PieceType t_pieceType)
    {
        return t_pieceType >= PAWN && t_pieceType < NO_PIECE_TYPE;
    }

    /**
     * @brief Checks whether a given Color is valid.
     *
     * @param t_color The Color to validate.
     *
     * @return True if the Color is valid; otherwise, false.
     */
    [[nodiscard]] constexpr bool is_valid_color(const Color t_color)
    {
        return t_color == WHITE || t_color == BLACK;
    }

    //-------------------------------------------------
    // Square utils
    //-------------------------------------------------

    /**
     * @brief Creates a Square from a given File and Rank.
     *
     * @param t_file The file (column) on the chessboard.
     * @param t_rank The rank (row) on the chessboard.
     *
     * @return A Square representing the corresponding board position.
     */
    [[nodiscard]] constexpr Square make_square(const File t_file, const Rank t_rank)
    {
        QB_ASSERT(is_valid_file(t_file), "[make_square()] Invalid file.");
        QB_ASSERT(is_valid_rank(t_rank), "[make_square()] Invalid rank.");
        return static_cast<Square>((static_cast<int>(t_rank) << 3) + static_cast<int>(t_file));
    }

    /**
     * @brief Vertically mirrors a square the 0‑63 board index.
     *
     * @param t_square Square to be flipped.
     *
     * @return The square on the opposite side of the board.
     */
    constexpr Square flip_square(const Square t_square)
    {
        QB_ASSERT(is_valid_square(t_square), "[flip_square()] Invalid square.");
        return static_cast<Square>(t_square ^ 56);
    }

    /**
     * @brief Gets the File (column) of a given Square.
     *
     * @param t_square The Square to extract the File from.
     *
     * @return The File corresponding to the square.
     */
    [[nodiscard]] constexpr File file_of(const Square t_square)
    {
        QB_ASSERT(is_valid_square(t_square), "[file_of()] Invalid square.");
        return static_cast<File>(t_square & 7);
    }

    /**
     * @brief Gets the Rank (row) of a given Square.
     *
     * @param t_square The Square to extract the Rank from.
     *
     * @return The Rank corresponding to the square.
     */
    [[nodiscard]] constexpr Rank rank_of(const Square t_square)
    {
        QB_ASSERT(is_valid_square(t_square), "[rank_of()] Invalid square.");
        return static_cast<Rank>(t_square >> 3);
    }

    /**
     * @brief Computes the diagonal index for a given square.
     *
     * The diagonal index is defined as the sum of the rank and file of the square.
     *
     * @param t_square The square to compute the diagonal index for.
     *
     * @return The diagonal index.
     */
    constexpr auto diagonal_index(const Square t_square)
    {
        QB_ASSERT(is_valid_square(t_square), "[diagonal_index()] Invalid square.");
        return static_cast<int>(rank_of(t_square)) + static_cast<int>(file_of(t_square));
    }

    /**
     * @brief Computes the anti-diagonal index for a given square.
     *
     * The anti-diagonal index is defined as 7 plus the file minus the rank of the square.
     *
     * @param t_square The square to compute the anti-diagonal index for.
     *
     * @return The anti-diagonal index.
     */
    constexpr auto antidiagonal_index(const Square t_square)
    {
        QB_ASSERT(is_valid_square(t_square), "[antidiagonal_index()] Invalid square.");
        return 7 + static_cast<int>(file_of(t_square)) - static_cast<int>(rank_of(t_square));
    }

    /**
     * @brief Converts a 2-character algebraic notation string (e.g., "e4") into a Square enum.
     *
     * The input string must be exactly two characters: a file ('a'-'h') and a rank ('1'-'8').
     * If the input is invalid, SQ_NONE is returned.
     *
     * @param t_string The square string in algebraic notation.
     *
     * @return The corresponding Square enum value or SQ_NONE on error.
     */
    inline Square square_from_string(const std::string& t_string)
    {
        if (t_string.length() != 2)
        {
            return SQ_NONE;
        }

        const int file{ t_string[0] - 'a' };
        const int rank{ t_string[1] - '1' };

        if (file < 0 || file > 7 || rank < 0 || rank > 7)
        {
            return SQ_NONE;
        }

        return static_cast<Square>((rank << 3) + file);
    }

    //-------------------------------------------------
    // Color
    //-------------------------------------------------

    /**
     * @brief Returns the opposite of the given color.
     *
     * @param t_color The color to invert.
     *
     * @return The opposite color.
     */
    [[nodiscard]] constexpr Color opposite_color(const Color t_color)
    {
        QB_ASSERT(is_valid_color(t_color), "[opposite_color()] Invalid color.");
        return static_cast<Color>(1 ^ t_color);
    }

    //-------------------------------------------------
    // PieceType
    //-------------------------------------------------

    /**
     * @brief Encodes a piece‑type, color pair into a compact index 0‑11.
     *
     * @param t_pieceType Piece type to encode (must not be NO_PIECE_TYPE).
     * @param t_color Color of the piece (must be WHITE or BLACK).
     *
     * @return Index in the range 0‑11.
     */
    [[nodiscard]] constexpr size_t encode_piece(const PieceType t_pieceType, const Color t_color)
    {
        QB_ASSERT(is_valid_piece_type(t_pieceType), "[encode_piece()] Invalid piece type.");
        QB_ASSERT(is_valid_color(t_color), "[encode_piece()] Invalid color.");
        return 2 * static_cast<size_t>(t_pieceType) + static_cast<size_t>(t_color);
    }

    /**
     * @brief Converts a character to the corresponding PieceType.
     *
     * This function maps standard FEN characters ('p', 'n', 'b', 'r', 'q', 'k') to their
     * respective PieceType enumeration values. The input character is case-insensitive.
     *
     * @param t_pieceChar The character representing a chess piece.
     *
     * @return The corresponding PieceType, or NO_PIECE_TYPE if the character is invalid.
     */
    [[nodiscard]] constexpr PieceType piece_type_from_char(char t_pieceChar)
    {
        if (t_pieceChar >= 'A' && t_pieceChar <= 'Z')
        {
            t_pieceChar |= 32;
        }

        switch (t_pieceChar)
        {
        case 'p':
            return PAWN;
        case 'n':
            return KNIGHT;
        case 'b':
            return BISHOP;
        case 'r':
            return ROOK;
        case 'q':
            return QUEEN;
        case 'k':
            return KING;
        default:
            return NO_PIECE_TYPE;
        }
    }
}
