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

#include <inplace_vector>
#include <bit>
#include "Utils.h"

//-------------------------------------------------
// Bitboard type
//-------------------------------------------------

/** @brief Represents a bitboard. */
using U64 = uint64_t;

//-------------------------------------------------
// Bitboard constants
//-------------------------------------------------

namespace qb
{
    //-------------------------------------------------
    // Bitboards
    //-------------------------------------------------

    struct Bitboards
    {
        /** @brief Positions of white pieces. */
        U64 whitePawns{ 0 };
        U64 whiteRooks{ 0 };
        U64 whiteKnights{ 0 };
        U64 whiteBishops{ 0 };
        U64 whiteQueens{ 0 };
        U64 whiteKing{ 0 };

        /** @brief Positions of black pieces. */
        U64 blackPawns{ 0 };
        U64 blackRooks{ 0 };
        U64 blackKnights{ 0 };
        U64 blackBishops{ 0 };
        U64 blackQueens{ 0 };
        U64 blackKing{ 0 };

        /** @brief Has a 1 if there is a white piece in a square. */
        U64 whitePieces{ 0 };

        /** @brief Has a 1 if there is a black piece in a square. */
        U64 blackPieces{ 0 };

        /** @brief Has a 1 for each square that is currently occupied by a piece. */
        U64 allPieces{ 0 };

        /** @brief Has a 1 for each empty square. */
        U64 noPieces{ 0 };
    };

    //-------------------------------------------------
    // Square bitboards
    //-------------------------------------------------

    inline constexpr U64 A1{ 0x0000000000000001 };
    inline constexpr U64 B1{ 0x0000000000000002 };
    inline constexpr U64 C1{ 0x0000000000000004 };
    inline constexpr U64 D1{ 0x0000000000000008 };
    inline constexpr U64 E1{ 0x0000000000000010 };
    inline constexpr U64 F1{ 0x0000000000000020 };
    inline constexpr U64 G1{ 0x0000000000000040 };
    inline constexpr U64 H1{ 0x0000000000000080 };

    inline constexpr U64 A2{ 0x0000000000000100 };
    inline constexpr U64 B2{ 0x0000000000000200 };
    inline constexpr U64 C2{ 0x0000000000000400 };
    inline constexpr U64 D2{ 0x0000000000000800 };
    inline constexpr U64 E2{ 0x0000000000001000 };
    inline constexpr U64 F2{ 0x0000000000002000 };
    inline constexpr U64 G2{ 0x0000000000004000 };
    inline constexpr U64 H2{ 0x0000000000008000 };

    inline constexpr U64 A3{ 0x0000000000010000 };
    inline constexpr U64 B3{ 0x0000000000020000 };
    inline constexpr U64 C3{ 0x0000000000040000 };
    inline constexpr U64 D3{ 0x0000000000080000 };
    inline constexpr U64 E3{ 0x0000000000100000 };
    inline constexpr U64 F3{ 0x0000000000200000 };
    inline constexpr U64 G3{ 0x0000000000400000 };
    inline constexpr U64 H3{ 0x0000000000800000 };

    inline constexpr U64 A4{ 0x0000000001000000 };
    inline constexpr U64 B4{ 0x0000000002000000 };
    inline constexpr U64 C4{ 0x0000000004000000 };
    inline constexpr U64 D4{ 0x0000000008000000 };
    inline constexpr U64 E4{ 0x0000000010000000 };
    inline constexpr U64 F4{ 0x0000000020000000 };
    inline constexpr U64 G4{ 0x0000000040000000 };
    inline constexpr U64 H4{ 0x0000000080000000 };

    inline constexpr U64 A5{ 0x0000000100000000 };
    inline constexpr U64 B5{ 0x0000000200000000 };
    inline constexpr U64 C5{ 0x0000000400000000 };
    inline constexpr U64 D5{ 0x0000000800000000 };
    inline constexpr U64 E5{ 0x0000001000000000 };
    inline constexpr U64 F5{ 0x0000002000000000 };
    inline constexpr U64 G5{ 0x0000004000000000 };
    inline constexpr U64 H5{ 0x0000008000000000 };

    inline constexpr U64 A6{ 0x0000010000000000 };
    inline constexpr U64 B6{ 0x0000020000000000 };
    inline constexpr U64 C6{ 0x0000040000000000 };
    inline constexpr U64 D6{ 0x0000080000000000 };
    inline constexpr U64 E6{ 0x0000100000000000 };
    inline constexpr U64 F6{ 0x0000200000000000 };
    inline constexpr U64 G6{ 0x0000400000000000 };
    inline constexpr U64 H6{ 0x0000800000000000 };

    inline constexpr U64 A7{ 0x0001000000000000 };
    inline constexpr U64 B7{ 0x0002000000000000 };
    inline constexpr U64 C7{ 0x0004000000000000 };
    inline constexpr U64 D7{ 0x0008000000000000 };
    inline constexpr U64 E7{ 0x0010000000000000 };
    inline constexpr U64 F7{ 0x0020000000000000 };
    inline constexpr U64 G7{ 0x0040000000000000 };
    inline constexpr U64 H7{ 0x0080000000000000 };

    inline constexpr U64 A8{ 0x0100000000000000 };
    inline constexpr U64 B8{ 0x0200000000000000 };
    inline constexpr U64 C8{ 0x0400000000000000 };
    inline constexpr U64 D8{ 0x0800000000000000 };
    inline constexpr U64 E8{ 0x1000000000000000 };
    inline constexpr U64 F8{ 0x2000000000000000 };
    inline constexpr U64 G8{ 0x4000000000000000 };
    inline constexpr U64 H8{ 0x8000000000000000 };

    /**
     * @brief Computes the bitboard for the specified square.
     *
     * @param t_square The square for which the bitboard is requested.
     *
     * @return A U64 bitboard with only the bit of the given square set.
     */
    [[nodiscard]] constexpr U64 square_bitboard(const Square t_square) noexcept
    {
        QB_ASSERT(is_valid_square(t_square), "[square_bitboard()] Invalid square.");
        return 1ULL << static_cast<unsigned int>(t_square);
    }

    //-------------------------------------------------
    // File bitboards
    //-------------------------------------------------

    inline constexpr U64 MASK_FILE_A{ 0x0101010101010101 };
    inline constexpr U64 MASK_FILE_B{ 0x0202020202020202 };
    inline constexpr U64 MASK_FILE_C{ 0x0404040404040404 };
    inline constexpr U64 MASK_FILE_D{ 0x0808080808080808 };
    inline constexpr U64 MASK_FILE_E{ 0x1010101010101010 };
    inline constexpr U64 MASK_FILE_F{ 0x2020202020202020 };
    inline constexpr U64 MASK_FILE_G{ 0x4040404040404040 };
    inline constexpr U64 MASK_FILE_H{ 0x8080808080808080 };

    inline constexpr std::array FILE_MASKS
    {
        MASK_FILE_A, MASK_FILE_B, MASK_FILE_C, MASK_FILE_D,
        MASK_FILE_E, MASK_FILE_F, MASK_FILE_G, MASK_FILE_H
    };

    //-------------------------------------------------
    // Rank bitboards
    //-------------------------------------------------

    inline constexpr U64 MASK_RANK_1{ 0x00000000000000FF };
    inline constexpr U64 MASK_RANK_2{ 0x000000000000FF00 };
    inline constexpr U64 MASK_RANK_3{ 0x0000000000FF0000 };
    inline constexpr U64 MASK_RANK_4{ 0x00000000FF000000 };
    inline constexpr U64 MASK_RANK_5{ 0x000000FF00000000 };
    inline constexpr U64 MASK_RANK_6{ 0x0000FF0000000000 };
    inline constexpr U64 MASK_RANK_7{ 0x00FF000000000000 };
    inline constexpr U64 MASK_RANK_8{ 0xFF00000000000000 };

    inline constexpr std::array RANK_MASKS
    {
        MASK_RANK_1, MASK_RANK_2, MASK_RANK_3, MASK_RANK_4,
        MASK_RANK_5, MASK_RANK_6, MASK_RANK_7, MASK_RANK_8
    };

    //-------------------------------------------------
    // Diagonals
    //-------------------------------------------------

    /**
     * @brief Generates a bitmask for all squares on a given diagonal index.
     *
     * Iterates over all squares and sets bits corresponding to squares on the specified diagonal.
     *
     * @param t_diagIndex The diagonal index (0 to 14).
     *
     * @return Bitmask representing the diagonal.
     */
    constexpr U64 generate_diagonal_mask(const int t_diagIndex)
    {
        U64 mask{ 0 };
        for (int sq{ 0 }; sq < 64; ++sq)
        {
            if (diagonal_index(static_cast<Square>(sq)) == t_diagIndex)
            {
                mask |= square_bitboard(static_cast<Square>(sq));
            }
        }

        return mask;
    }

    /**
     * @brief Generates a bitmask for all squares on a given anti-diagonal index.
     *
     * Iterates over all squares and sets bits corresponding to squares on the specified anti-diagonal.
     *
     * @param t_adiagIndex The anti-diagonal index (0 to 14).
     *
     * @return Bitmask representing the anti-diagonal.
     */
    constexpr U64 generate_anti_diagonal_mask(const int t_adiagIndex)
    {
        U64 mask{ 0 };
        for (int sq{ 0 }; sq < 64; ++sq)
        {
            if (antidiagonal_index(static_cast<Square>(sq)) == t_adiagIndex)
            {
                mask |= square_bitboard(static_cast<Square>(sq));
            }
        }

        return mask;
    }

    /**
     * @brief Precomputed bitmasks for all diagonals on the chessboard.
     */
    inline constexpr std::array DIAGONAL_MASKS
    {
        generate_diagonal_mask(0), generate_diagonal_mask(1), generate_diagonal_mask(2), generate_diagonal_mask(3),
        generate_diagonal_mask(4), generate_diagonal_mask(5), generate_diagonal_mask(6), generate_diagonal_mask(7),
        generate_diagonal_mask(8), generate_diagonal_mask(9), generate_diagonal_mask(10), generate_diagonal_mask(11),
        generate_diagonal_mask(12), generate_diagonal_mask(13), generate_diagonal_mask(14)
    };

    /**
     * @brief Precomputed bitmasks for all anti-diagonals on the chessboard.
     */
    inline constexpr std::array ANTIDIAGONAL_MASKS
    {
        generate_anti_diagonal_mask(0), generate_anti_diagonal_mask(1), generate_anti_diagonal_mask(2), generate_anti_diagonal_mask(3),
        generate_anti_diagonal_mask(4), generate_anti_diagonal_mask(5), generate_anti_diagonal_mask(6), generate_anti_diagonal_mask(7),
        generate_anti_diagonal_mask(8), generate_anti_diagonal_mask(9), generate_anti_diagonal_mask(10), generate_anti_diagonal_mask(11),
        generate_anti_diagonal_mask(12), generate_anti_diagonal_mask(13), generate_anti_diagonal_mask(14)
    };

    //-------------------------------------------------
    // Compass
    //-------------------------------------------------

    /*
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

      Left shift '<<' means +1 on the chessboard
      Right shift '>>' means -1 on the chessboard

      +7  +8  +9
      -1   0  +1
      -9  -8  -7
    */

    //-------------------------------------------------
    // Castling bitboards
    //-------------------------------------------------

    inline constexpr U64 WHITE_CASTLING_MASK_OO{ F1 | G1 };
    inline constexpr U64 WHITE_CASTLING_MASK_OOO{ B1 | C1 | D1 };
    inline constexpr U64 BLACK_CASTLING_MASK_OO{ F8 | G8 };
    inline constexpr U64 BLACK_CASTLING_MASK_OOO{ B8 | C8 | D8 };

    //-------------------------------------------------
    // Bitboard utils
    //-------------------------------------------------

    /**
     * @brief Reverse the bit order of a 64‑bit bitboard.
     *
     * @param t_bitboard 64‑bit value whose bits are to be reversed.
     *
     * @return The input with all 64 bits in the opposite order.
     */
    [[nodiscard]] constexpr U64 reverse_bits(U64 t_bitboard) noexcept
    {
        t_bitboard = std::byteswap(t_bitboard);

        t_bitboard = ((t_bitboard >> 1) & 0x5555555555555555ULL) | ((t_bitboard & 0x5555555555555555ULL) << 1);
        t_bitboard = ((t_bitboard >> 2) & 0x3333333333333333ULL) | ((t_bitboard & 0x3333333333333333ULL) << 2);
        t_bitboard = ((t_bitboard >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((t_bitboard & 0x0F0F0F0F0F0F0F0FULL) << 4);

        return t_bitboard;
    }

    /**
     * @brief Sets the bit corresponding to the specified File and Rank on the given bitboard.
     *
     * @param t_bitboard Reference to the bitboard to be updated.
     * @param t_file The File of the square to set.
     * @param t_rank The Rank of the square to set.
     */
    constexpr void set_bit(U64& t_bitboard, const File t_file, const Rank t_rank) noexcept
    {
        QB_ASSERT(is_valid_file(t_file), "[set_bit()] Invalid file.");
        QB_ASSERT(is_valid_rank(t_rank), "[set_bit()] Invalid rank.");
        t_bitboard |= square_bitboard(make_square(t_file, t_rank));
    }

    /**
     * @brief Sets a specific bit on the bitboard corresponding to the given square.
     *
     * @param t_bitboard Reference to the bitboard to be updated.
     * @param t_square The square whose corresponding bit is to be set.
     */
    constexpr void set_bit(U64& t_bitboard, const Square t_square) noexcept
    {
        t_bitboard |= square_bitboard(t_square);
    }

    /**
     * @brief Clears a specific bit on the bitboard.
     *
     * @param t_bitboard Reference to the bitboard to modify.
     * @param t_square The square whose corresponding bit will be cleared.
     */
    constexpr void clear_bit(U64& t_bitboard, const Square t_square) noexcept
    {
        t_bitboard &= ~square_bitboard(t_square);
    }

    /**
     * @brief Checks if the bit corresponding to the specified File and Rank is set on the given bitboard.
     *
     * @param t_bitboard The target bitboard to check.
     * @param t_file The File of the square to check.
     * @param t_rank The Rank of the square to check.
     *
     * @return True if the bit is set, false otherwise.
     */
    [[nodiscard]] constexpr bool is_bit_set(const U64 t_bitboard, const File t_file, const Rank t_rank) noexcept
    {
        QB_ASSERT(is_valid_file(t_file), "[is_bit_set()] Invalid file.");
        QB_ASSERT(is_valid_rank(t_rank), "[is_bit_set()] Invalid rank.");
        return (t_bitboard & square_bitboard(make_square(t_file, t_rank))) != 0;
    }

    /**
     * @brief Checks whether the bit corresponding to the specified square is set in the given bitboard.
     *
     * @param t_bitboard The bitboard to check.
     * @param t_square The square whose corresponding bit is to be tested.
     *
     * @return True if the bit is set; false otherwise.
     */
    [[nodiscard]] constexpr bool is_bit_set(const U64 t_bitboard, const Square t_square) noexcept
    {
        return (t_bitboard & square_bitboard(t_square)) != 0;
    }

    /**
     * @brief Returns the square corresponding to the least significant set bit (LSB) in a non-zero bitboard.
     *
     * This function assumes that the input bitboard is non-zero.
     * The returned value is a Square enum corresponding to the index (0-63) of the least significant set bit.
     *
     * @param t_bitboard The input bitboard (must not be zero).
     *
     * @return The Square corresponding to the least significant set bit.
     */
    inline Square get_lsb(const U64 t_bitboard) noexcept
    {
        QB_ASSERT(t_bitboard, "[get_lsb()] Invalid bitboard.");
        return static_cast<Square>(__builtin_ctzll(t_bitboard));
    }

    /**
     * @brief Finds and clears the least significant bit (LSB) in a non-zero bitboard.
     *
     * @param t_bitboard The input bitboard from which the LSB will be found and cleared.
     *
     * @return The Square of the cleared least-significant-bit.
     */
    inline Square pop_lsb(U64& t_bitboard) noexcept
    {
        QB_ASSERT(t_bitboard, "[pop_lsb()] Invalid bitboard.");

        const Square square{ get_lsb(t_bitboard) };
        t_bitboard &= t_bitboard - 1;

        return square;
    }

    /**
     * @brief Counts the number of set bits in a 64-bit bitboard.
     *
     * This function uses the compiler intrinsic __builtin_popcountll(),
     * which typically compiles down to a single POPCNT instruction on
     * supported CPUs. It efficiently returns the Hamming weight (number
     * of set bits) of the given bitboard.
     *
     * @param t_bitboard The bitboard whose bits are to be counted.
     *
     * @return int The number of set bits in the bitboard.
     */
    inline int popcount(const U64 t_bitboard) noexcept
    {
        return __builtin_popcountll(t_bitboard);
    }

    /**
     * @brief Extracts all set bits from a bitboard and returns them as a stack-allocated list of squares.
     *
     * Uses C++26 std::inplace_vector to completely avoid slow heap allocations.
     *
     * @param t_bitboard The bitboard to scan for set bits.
     *
     * @return std::inplace_vector<Square, 64> A stack-allocated vector containing all set square indices.
     */
    inline std::inplace_vector<Square, 64> bitscan_all(U64 t_bitboard) noexcept
    {
        std::inplace_vector<Square, 64> sq;
        while (t_bitboard)
        {
            sq.push_back(pop_lsb(t_bitboard));
        }

        return sq;
    }
}
