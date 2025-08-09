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

// ReSharper disable CppParameterMayBeConst

#pragma once

#include <array>
#include <string>
#include <type_traits>

//-------------------------------------------------
// Types
//-------------------------------------------------

namespace qb
{
    //-------------------------------------------------
    // File && Rank
    //-------------------------------------------------

    /**
     * @brief A vertical column on the chessboard.
     */
    enum File : int
    {
        FILE_A, FILE_B, FILE_C, FILE_D,
        FILE_E, FILE_F, FILE_G, FILE_H
    };

    /**
     * @brief A horizontal row on the chessboard.
     */
    enum Rank : int
    {
        RANK_1, RANK_2, RANK_3, RANK_4,
        RANK_5, RANK_6, RANK_7, RANK_8
    };

    //-------------------------------------------------
    // Square
    //-------------------------------------------------

    /*
      Little-endian rank-file mapping of each square.
      Position A1 is the least significant bit (LSB), bit 0, of the 64-bit number, and H8 is the
      most significant bit (MSB), bit 63. Squares are assigned in left-to-right, bottom-to-top order,
      mapping each square to one bit index in the 64-bit number, from LSB to MSB.

      8 | 56 57 58 59 60 61 62 63
      7 | 48 49 50 51 52 53 54 55
      6 | 40 41 42 43 44 45 46 47
      5 | 32 33 34 35 36 37 38 39
      4 | 24 25 26 27 28 29 30 31
      3 | 16 17 18 19 20 21 22 23
      2 |  8  9 10 11 12 13 14 15
      1 |  0  1  2  3  4  5  6  7
        -------------------------
           A  B  C  D  E  F  G  H
    */

    /**
     * @brief Enum representing each square on a chessboard.
     */
    enum Square : int
    {
        SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
        SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
        SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
        SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
        SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
        SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
        SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
        SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
        SQ_NONE
    };

    /**
     * @brief Array of square names on the chessboard.
     */
    constexpr std::array<std::string_view, 64> SQUARE_NAMES
    {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
    };

    //-------------------------------------------------
    // Color
    //-------------------------------------------------

    /**
     * @brief Enum representing the colors used in a chess game.
     */
    enum Color : int
    {
        WHITE, BLACK,
        NO_COLOR
    };

    /**
     * @brief Array of color names.
     */
    constexpr std::array<std::string_view, 2> COLOR_NAMES
    {
        "White", "Black"
    };

    //-------------------------------------------------
    // Piece
    //-------------------------------------------------

    /**
     * @brief Enum representing the different types of chess pieces.
     */
    enum PieceType : int
    {
        PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
        NO_PIECE_TYPE
    };

    /**
     * @brief Represents a single piece, such as a pawn, knight, bishop, etc.
     */
    struct Piece
    {
        PieceType type{ NO_PIECE_TYPE };
        Color color{ NO_COLOR };

        [[nodiscard]] auto IsEmpty() const { return type == NO_PIECE_TYPE; }
    };

    /**
     * @brief Array of short single-character piece identifiers.
     */
    constexpr std::array<std::string_view, 12> PIECE_SHORT_NAMES
    {
        "P" ,"N", "B", "R", "Q", "K",
        "p", "n", "b", "r", "q", "k"
    };

    /**
     * @brief Array of Unicode symbols for each piece type.
     */
    constexpr std::array<std::string_view, 12> PIECE_UNICODE_NAMES
    {
        "♙", "♘", "♗", "♖", "♕", "♔",
        "♟", "♞", "♝", "♜", "♛", "♚"
    };

    //-------------------------------------------------
    // Move type
    //-------------------------------------------------

    /**
     * @brief Enum representing the types of moves in chess.
     */
    enum MoveType : int
    {
        NORMAL, PROMOTION, EN_PASSANT, CASTLING
    };

    /**
     * @brief Array of move type descriptions.
     *
     * - Normal: A standard move.
     * - Promotion: A pawn reaches the last rank and promotes.
     * - En Passant: A special pawn capture.
     * - Castling: A king and rook move simultaneously.
     */
    constexpr std::array<std::string_view, 4> MOVE_TYPE_NAMES
    {
        "Normal", "Promotion", "En Passant", "Castling"
    };

    //-------------------------------------------------
    // Castling rights
    //-------------------------------------------------

    /**
     * @brief Enum representing castling rights in a chess game.
     */
    enum CastlingRights : int
    {
        NO_CASTLING,
        WHITE_OO  = 1 << 0,
        WHITE_OOO = WHITE_OO << 1,
        BLACK_OO  = WHITE_OO << 2,
        BLACK_OOO = WHITE_OO << 3,

        KING_SIDE      = WHITE_OO | BLACK_OO,
        QUEEN_SIDE     = WHITE_OOO | BLACK_OOO,
        WHITE_CASTLING = WHITE_OO | WHITE_OOO,
        BLACK_CASTLING = BLACK_OO | BLACK_OOO,
        ANY_CASTLING   = WHITE_CASTLING | BLACK_CASTLING
    };

    /**
     * @brief Bitwise OR operator overload for CastlingRights.
     *
     * @param t_lhs Left-hand operand.
     * @param t_rhs Right-hand operand.
     *
     * @return Combined castling rights.
     */
    inline CastlingRights operator|(CastlingRights t_lhs, CastlingRights t_rhs)
    {
        return static_cast<CastlingRights>(
            static_cast<std::underlying_type_t<CastlingRights>>(t_lhs) |
            static_cast<std::underlying_type_t<CastlingRights>>(t_rhs));
    }

    /**
     * @brief Bitwise OR-assignment operator overload for CastlingRights.
     *
     * @param t_lhs Left-hand operand (modified).
     * @param t_rhs Right-hand operand.
     *
     * @return Reference to the modified left-hand operand.
     */
    inline CastlingRights& operator|=(CastlingRights& t_lhs, CastlingRights t_rhs)
    {
        t_lhs = t_lhs | t_rhs;

        return t_lhs;
    }

    /**
     * @brief Bitwise AND operator overload for CastlingRights.
     *
     * @param t_lhs The left-hand side operand.
     * @param t_rhs The right-hand side operand.
     *
     * @return The result of the operation based on operands provided.
     */
    inline CastlingRights operator&(CastlingRights t_lhs, CastlingRights t_rhs)
    {
        return static_cast<CastlingRights>(
            static_cast<std::underlying_type_t<CastlingRights>>(t_lhs) &
            static_cast<std::underlying_type_t<CastlingRights>>(t_rhs)
        );
    }

    /**
     * @brief Bitwise AND-assignment operator overload for CastlingRights.
     *
     * @param t_lhs Reference to the left-hand side CastlingRights object, which will be updated with the result.
     * @param t_rhs The right-hand side CastlingRights object to be included in the bitwise AND operation.
     *
     * @return A reference to the updated left-hand side CastlingRights object.
     */
    inline CastlingRights& operator&=(CastlingRights& t_lhs, CastlingRights t_rhs)
    {
        t_lhs = t_lhs & t_rhs;

        return t_lhs;
    }

    /**
     * @brief Bitwise NOT operator overload for CastlingRights.
     *
     * @param t_rhs The castling rights to invert.
     *
     * @return The inverted castling rights.
     */
    inline CastlingRights operator~(CastlingRights t_rhs)
    {
        return static_cast<CastlingRights>(~static_cast<std::underlying_type_t<CastlingRights>>(t_rhs));
    }

    //-------------------------------------------------
    // Operator
    //-------------------------------------------------

#define ENABLE_INCR_OPERATORS_ON(T) \
    constexpr T& operator++(T& t_val) { return t_val = T(static_cast<std::underlying_type_t<T>>(t_val) + 1); } \
    constexpr T& operator--(T& t_val) { return t_val = T(static_cast<std::underlying_type_t<T>>(t_val) - 1); }

    ENABLE_INCR_OPERATORS_ON(Square)
    ENABLE_INCR_OPERATORS_ON(File)
    ENABLE_INCR_OPERATORS_ON(Rank)
#undef ENABLE_INCR_OPERATORS_ON
}
