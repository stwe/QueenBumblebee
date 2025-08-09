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

#include <string>
#include <vector>
#include <unordered_map>
#include "Entry.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace qb
{
    /** @brief Forward declaration class Board. */
    class Board;
}

namespace qb::polyglot
{
    //-------------------------------------------------
    // Book
    //-------------------------------------------------

    /**
     * @brief Lightweight reader and query helper for Polyglot *.bin* opening books.
     */
    class Book
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Book();

        Book(const Book& t_other) = delete;
        Book(Book&& t_other) noexcept = delete;
        Book& operator=(const Book& t_other) = delete;
        Book& operator=(Book&& t_other) noexcept = delete;

        ~Book() noexcept;

        //-------------------------------------------------
        // Loading
        //-------------------------------------------------

        /**
         * @brief Load and parse a Polyglot book file.
         *
         * @param t_fileName Path to the *.bin* book file.
         *
         * @return True if the book was loaded successfully; false on any I/O or format error.
         */
        [[nodiscard]] bool Open(const std::string& t_fileName);

        //-------------------------------------------------
        // Query helpers
        //-------------------------------------------------

        /**
         * @brief Converts a 16-bit Polyglot move to a UCI (Universal Chess Interface) string.
         *
         * @param t_move The 16-bit Polyglot move.
         *
         * @return The corresponding move in UCI format (e.g., "e2e4").
         */
        [[nodiscard]] static std::string PolyMoveToUci(uint16_t t_move);

        /**
         * @brief Computes the Polyglot hash key for a given board position.
         *
         * This hash is used to look up entries in a Polyglot opening book.
         *
         * @param t_board Pointer to the board object representing the current position.
         *
         * @return The 64-bit Polyglot hash key for the position.
         */
        [[nodiscard]] static uint64_t PolyglotHash(const Board* t_board);

        /**
         * @brief Probe the book for a given hash.
         *
         * @param t_key The Polyglot hash value.
         *
         * @return A pair of constant iterators delimiting the range of matching entries.
         */
        [[nodiscard]] std::pair<std::vector<Entry>::const_iterator, std::vector<Entry>::const_iterator> Probe(uint64_t t_key) const;

        /**
         * @brief Converts the Polyglot castling move notation into the resulting king destination square.
         *
         * @param t_moveStr The move string representing the castling move (e.g., "e1h1").
         *
         * @return std::string The square the king moves to (e.g., "g1"), or an empty string.
         */
        [[nodiscard]] static std::string NormalizeCastlingToSquare(const std::string& t_moveStr);

    protected:

    private:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * @brief Unconventially Polyglot Castling moves.
         */
        inline static const std::unordered_map<std::string, std::string> castling_map
        {
            { "e1h1", "g1" },
            { "e1a1", "c1" },
            { "e8h8", "g8" },
            { "e8a8", "c8" }
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Complete entry table (host endianness).
         */
        std::vector<Entry> m_entries;

        //-------------------------------------------------
        // Internal helper
        //-------------------------------------------------

        /**
         * @brief Checks if a pawn of the given color is on the specified file and rank.
         *
         * Used for determining en passant eligibility when computing Polyglot hash keys.
         *
         * @param t_board Pointer to the board object.
         * @param t_file The file to check.
         * @param t_rank The rank to check.
         * @param t_color The color of the pawn (WHITE or BLACK).
         *
         * @return True if a pawn of the given color is on the specified square, false otherwise.
         */
        [[nodiscard]] static bool PawnOn(const Board* t_board, File t_file, Rank t_rank, Color t_color) noexcept;

        /**
         * @brief Map engine piece‑type and color to the Polyglot index 0–11.
         *
         * @note This engine uses a *black‑white interleaved* table that matches
         *       the accompanying Polyglot key set (Komodo book).
         */
        [[nodiscard]] static constexpr auto PolyPieceIndex(const PieceType t_pieceType, const Color t_color) noexcept
        {
            switch (t_pieceType)
            {
            case PAWN:
                return t_color == WHITE ? 1 : 0;
            case KNIGHT:
                return t_color == WHITE ? 3 : 2;
            case BISHOP:
                return t_color == WHITE ? 5 : 4;
            case ROOK:
                return t_color == WHITE ? 7 : 6;
            case QUEEN:
                return t_color == WHITE ? 9 : 8;
            case KING:
                return t_color == WHITE ? 11 : 10;
            default:
                return -1;
            }
        }
    };
}
