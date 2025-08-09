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

// ReSharper disable CppUseStructuredBinding

#define CATCH_CONFIG_MAIN

#include <regex>
#include <catch2/catch_test_macros.hpp>
#include "Bitboard.h"

//-------------------------------------------------
// Diagonals
//-------------------------------------------------

constexpr auto REFERENCE_DIAG_MASK = [](const int t_diagIdx) {
    U64 mask{ 0 };
    for (auto sq{ 0 }; sq < 64; ++sq)
    {
        if (qb::diagonal_index(static_cast<qb::Square>(sq)) == t_diagIdx)
        {
            mask |= qb::SQUARE_BITBOARDS[sq];
        }
    }

    return mask;
};

constexpr auto REFERENCE_ANTI_DIAG_MASK = [](const int t_adiagIdx) {
    U64 mask{ 0 };
    for (auto sq{ 0 }; sq < 64; ++sq)
    {
        if (qb::antidiagonal_index(static_cast<qb::Square>(sq)) == t_adiagIdx)
        {
            mask |= qb::SQUARE_BITBOARDS[sq];
        }
    }

    return mask;
};

TEST_CASE("generate_diagonal_mask produces correct bit pattern", "[generate_diagonal_mask]")
{
    for (auto idx{ 0 }; idx <= 14; ++idx)
    {
        REQUIRE(qb::generate_diagonal_mask(idx) == REFERENCE_DIAG_MASK(idx));
    }
}

TEST_CASE("generate_anti_diagonal_mask produces correct bit pattern", "[generate_anti_diagonal_mask]")
{
    for (auto idx{ 0 }; idx <= 14; ++idx)
    {
        REQUIRE(qb::generate_anti_diagonal_mask(idx) == REFERENCE_ANTI_DIAG_MASK(idx));
    }
}

//-------------------------------------------------
// Bitboard utils
//-------------------------------------------------

TEST_CASE("reverse_bits returns bitboard with bits mirrored", "[reverse_bits]")
{
    struct VectorTest
    {
        U64 in;
        U64 out;
    };

    constexpr VectorTest VECTORS[]
    {
        {0x0000000000000000ULL, 0x0000000000000000ULL},
        {0x0000000000000001ULL, 0x8000000000000000ULL},
        {0x8000000000000000ULL, 0x0000000000000001ULL},
        {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL},
        {0x0123456789ABCDEFULL, 0xF7B3D591E6A2C480ULL},
        {0x00FF00FF00FF00FFULL, 0xFF00FF00FF00FF00ULL},
        {0xAA55AA55AA55AA55ULL, 0xAA55AA55AA55AA55ULL}
    };

    SECTION("known test vectors")
    {
        for (const auto &v : VECTORS)
        {
            REQUIRE(qb::reverse_bits(v.in) == v.out);
        }
    }

    SECTION("involution property (reverse twice → original)")
    {
        for (const auto &v : VECTORS)
        {
            REQUIRE(qb::reverse_bits(qb::reverse_bits(v.in)) == v.in);
        }
    }
}

TEST_CASE("square_bitboard generates correct bitboard", "[square_bitboard]")
{
    REQUIRE(qb::square_bitboard(qb::SQ_A1) == qb::A1);
    REQUIRE(qb::square_bitboard(qb::SQ_H8) == qb::H8);
    REQUIRE(qb::square_bitboard(qb::SQ_D4) == qb::D4);
}

TEST_CASE("set_bit sets correct bits", "[set_bit]")
{
    U64 board{ 0 };
    qb::set_bit(board, qb::FILE_A, qb::RANK_1);
    REQUIRE(board == qb::A1);

    qb::set_bit(board, qb::FILE_H, qb::RANK_8);
    REQUIRE(board == (qb::A1 | qb::H8));

    qb::set_bit(board, qb::SQ_B3);
    REQUIRE(board == (qb::A1 | qb::H8 | qb::B3));
}

TEST_CASE("set_bit sets correct bit using Square parameter", "[set_bit]")
{
    U64 board{ 0 };

    SECTION("Setting single bits")
    {
        qb::set_bit(board, qb::SQ_A1);
        REQUIRE(board == qb::A1);

        board = 0;
        qb::set_bit(board, qb::SQ_H8);
        REQUIRE(board == qb::H8);

        board = 0;
        qb::set_bit(board, qb::SQ_D4);
        REQUIRE(board == qb::D4);
    }

    SECTION("Setting multiple bits")
    {
        board = 0;
        qb::set_bit(board, qb::SQ_A1);
        qb::set_bit(board, qb::SQ_H8);
        REQUIRE(board == (qb::A1 | qb::H8));

        qb::set_bit(board, qb::SQ_D4);
        REQUIRE(board == (qb::A1 | qb::D4 | qb::H8));
    }

    SECTION("Setting already set bits")
    {
        board = qb::A1;
        qb::set_bit(board, qb::SQ_A1);
        REQUIRE(board == qb::A1);

        qb::set_bit(board, qb::SQ_H8);
        REQUIRE(board == (qb::A1 | qb::H8));
    }
}

TEST_CASE("clear_bit clears correct bits", "[clear_bit]")
{
    SECTION("Clearing bits using Square")
    {
        U64 board{ qb::A1 | qb::D4 | qb::H8 };

        qb::clear_bit(board, qb::SQ_A1);
        REQUIRE(board == (qb::D4 | qb::H8));

        qb::clear_bit(board, qb::SQ_D4);
        REQUIRE(board == qb::H8);

        qb::clear_bit(board, qb::SQ_H8);
        REQUIRE(board == 0);
    }

    SECTION("Clearing already cleared bits")
    {
        U64 board{ qb::A1 | qb::H8 };

        qb::clear_bit(board, qb::SQ_D4);
        REQUIRE(board == (qb::A1 | qb::H8));
    }
}

TEST_CASE("is_bit_set detects correct bits", "[is_bit_set]")
{
    constexpr U64 BOARD{ qb::A1 | qb::D4 | qb::H8 };

    REQUIRE(qb::is_bit_set(BOARD, qb::FILE_A, qb::RANK_1));
    REQUIRE(qb::is_bit_set(BOARD, qb::FILE_D, qb::RANK_4));
    REQUIRE(qb::is_bit_set(BOARD, qb::FILE_H, qb::RANK_8));

    REQUIRE(qb::is_bit_set(BOARD, qb::SQ_A1));
    REQUIRE(qb::is_bit_set(BOARD, qb::SQ_D4));
    REQUIRE(qb::is_bit_set(BOARD, qb::SQ_H8));

    REQUIRE_FALSE(qb::is_bit_set(BOARD, qb::FILE_B, qb::RANK_1));
    REQUIRE_FALSE(qb::is_bit_set(BOARD, qb::FILE_G, qb::RANK_5));
}

TEST_CASE("get_lsb finds least significant bit", "[get_lsb]")
{
    REQUIRE(qb::get_lsb(qb::A1 | qb::H8) == qb::SQ_A1);
    REQUIRE(qb::get_lsb(qb::B2) == qb::SQ_B2);
    REQUIRE(qb::get_lsb(qb::H8) == qb::SQ_H8);
}

TEST_CASE("pop_lsb pops least significant bit", "[pop_lsb]")
{
    U64 board{ qb::A1 | qb::D4 | qb::H8 };

    qb::Square first{ qb::pop_lsb(board) };
    REQUIRE(first == qb::SQ_A1);
    REQUIRE(board == (qb::D4 | qb::H8));

    qb::Square second{ qb::pop_lsb(board) };
    REQUIRE(second == qb::SQ_D4);
    REQUIRE(board == qb::H8);

    qb::Square third{ qb::pop_lsb(board) };
    REQUIRE(third == qb::SQ_H8);
    REQUIRE(board == 0);
}

TEST_CASE("popcount returns correct number of set bits", "[popcount]")
{
    REQUIRE(qb::popcount(0x0ULL) == 0);                 // No bits set
    REQUIRE(qb::popcount(0x1ULL) == 1);                 // One-bit set (LSB)
    REQUIRE(qb::popcount(0x8000000000000000ULL) == 1);  // One-bit set (MSB)
    REQUIRE(qb::popcount(0xFFFFFFFFFFFFFFFFULL) == 64); // All bits set
    REQUIRE(qb::popcount(0xAAAAAAAAAAAAAAAAULL) == 32); // Alternating bits (1010...)
    REQUIRE(qb::popcount(0x5555555555555555ULL) == 32); // Alternating bits (0101...)
    REQUIRE(qb::popcount(0xF0F0F0F0F0F0F0F0ULL) == 32); // Patterned bits
    REQUIRE(qb::popcount(0x00000000FFFFFFFFULL) == 32); // Lower half full
    REQUIRE(qb::popcount(0xFFFFFFFF00000000ULL) == 32); // Upper half full
    REQUIRE(qb::popcount(0x0123456789ABCDEFULL) == 32); // Random pattern
}

TEST_CASE("bitscan_all scans all set bits correctly", "[bitscan_all]")
{
    SECTION("Empty bitboard returns empty vector")
    {
        U64 emptyBoard{ 0 };
        const auto squares{ qb::bitscan_all(emptyBoard) };
        REQUIRE(squares.empty());
    }

    SECTION("Single bit returns vector with one square")
    {
        SECTION("LSB position")
        {
            const auto squares{ qb::bitscan_all(qb::A1) };
            REQUIRE(squares.size() == 1);
            REQUIRE(squares[0] == qb::SQ_A1);
        }

        SECTION("MSB position")
        {
            const auto squares{ qb::bitscan_all(qb::H8) };
            REQUIRE(squares.size() == 1);
            REQUIRE(squares[0] == qb::SQ_H8);
        }

        SECTION("Middle position")
        {
            const auto squares{ qb::bitscan_all(qb::D4) };
            REQUIRE(squares.size() == 1);
            REQUIRE(squares[0] == qb::SQ_D4);
        }
    }

    SECTION("Multiple bits return squares in correct order")
    {
        constexpr U64 BOARD{ qb::A1 | qb::D4 | qb::H8 };
        const auto squares{ qb::bitscan_all(BOARD) };

        REQUIRE(squares.size() == 3);
        REQUIRE(squares[0] == qb::SQ_A1);  // LSB first
        REQUIRE(squares[1] == qb::SQ_D4);  // Middle bit second
        REQUIRE(squares[2] == qb::SQ_H8);  // MSB last
    }

    SECTION("Adjacent bits are correctly identified")
    {
        constexpr U64 BOARD{ qb::A1 | qb::B1 | qb::C1 };
        const auto squares{ qb::bitscan_all(BOARD) };

        REQUIRE(squares.size() == 3);
        REQUIRE(squares[0] == qb::SQ_A1);
        REQUIRE(squares[1] == qb::SQ_B1);
        REQUIRE(squares[2] == qb::SQ_C1);
    }
}
