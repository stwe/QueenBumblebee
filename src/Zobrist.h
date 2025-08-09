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

#include <random>
#include "Utils.h"

namespace qb
{
    //-------------------------------------------------
    // ZobristKeys
    //-------------------------------------------------

    /**
     * @brief Generates and stores Zobrist hash keys for chess positions.
     *
     * Zobrist hashing is used to uniquely identify chess positions based on pieces,
     * castling rights, en passant squares and side to move.
     */
    class ZobristKeys
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        /**
         * @brief Default constructor for ZobristKeys.
         */
        ZobristKeys()
            : ZobristKeys(std::random_device{}())
        {}

        /**
         * @brief Constructs Zobrist keys using a given random seed.
         *
         * @param t_seed A seed for deterministic key generation (useful for testing).
         */
        explicit ZobristKeys(const uint64_t t_seed)
        {
            std::mt19937_64 rng(t_seed);
            std::uniform_int_distribution<uint64_t> dist;

            for (auto& key : m_pieceSquareKeys)
            {
                key = dist(rng);
            }

            for (auto& key : m_castlingRightsKeys)
            {
                key = dist(rng);
            }

            for (auto& key : m_enPassantKeys)
            {
                key = dist(rng);
            }

            m_sideToMoveKey = dist(rng);
        }

        ZobristKeys(const ZobristKeys& t_other) = delete;
        ZobristKeys(ZobristKeys&& t_other) noexcept = delete;
        ZobristKeys& operator=(const ZobristKeys& t_other) = delete;
        ZobristKeys& operator=(ZobristKeys&& t_other) noexcept = delete;

        ~ZobristKeys() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Get Zobrist key for a piece on a specific square.
         *
         * @param t_color The color of the piece.
         * @param t_pieceType The type of the piece.
         * @param t_square The square index (0-63).
         *
         * @return The corresponding Zobrist key.
         */
        [[nodiscard]] uint64_t GetPieceSquareKey(const Color t_color, const PieceType t_pieceType, const Square t_square) const
        {
            QB_ASSERT(is_valid_color(t_color), "[ZobristKeys::GetPieceSquareKey()] Invalid color.");
            QB_ASSERT(is_valid_piece_type(t_pieceType), "[ZobristKeys::GetPieceSquareKey()] Invalid piece type.");
            QB_ASSERT(is_valid_square(t_square), "[ZobristKeys::GetPieceSquareKey()] Invalid square.");

            return m_pieceSquareKeys[Index(t_color, t_pieceType, t_square)];
        }

        /**
         * @brief Get Zobrist key for castling rights.
         *
         * @param t_castlingRights Castling rights.
         *
         * @return The corresponding Zobrist key.
         */
        [[nodiscard]] uint64_t GetCastlingRightsKey(const CastlingRights t_castlingRights) const
        {
            QB_ASSERT((static_cast<int>(t_castlingRights) & ~ANY_CASTLING) == 0, "[ZobristKeys::GetCastlingRightsKey()] Invalid castling rights.");

            return m_castlingRightsKeys[static_cast<size_t>(t_castlingRights)];
        }

        /**
         * @brief Get Zobrist key for en-passant-file.
         *
         * @param t_file File index (0-7).
         *
         * @return The corresponding Zobrist key.
         */
        [[nodiscard]] uint64_t GetEnPassantKey(const File t_file) const
        {
            QB_ASSERT(is_valid_file(t_file), "[ZobristKeys::GetEnPassantKey()] Invalid file.");

            return m_enPassantKeys[static_cast<size_t>(t_file)];
        }

        /**
         * @brief Get Zobrist key for the side to move.
         *
         * @return The Zobrist key for the side to move.
         */
        [[nodiscard]] uint64_t GetSideToMoveKey() const
        {
            return m_sideToMoveKey;
        }

    private:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr size_t NUM_COLORS{ 2 };
        static constexpr size_t NUM_PIECE_TYPES{ 6 };
        static constexpr size_t NUM_SQUARES{ 64 };
        static constexpr size_t TOTAL_PIECE_SQUARE_KEYS{ NUM_COLORS * NUM_PIECE_TYPES * NUM_SQUARES };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Array of Zobrist keys for piece-square combinations.
         */
        std::array<uint64_t, TOTAL_PIECE_SQUARE_KEYS> m_pieceSquareKeys{};

        /**
         * @brief Array of Zobrist keys for castling rights (16 possible combinations / 4 Bits: KQkq).
         */
        std::array<uint64_t, 16> m_castlingRightsKeys{};

        /**
         * @brief Array of Zobrist keys for en-passant-file (8 possible files).
         */
        std::array<uint64_t, 8> m_enPassantKeys{};

        /**
         * @brief Zobrist key for side to move.
         */
        uint64_t m_sideToMoveKey{ 0 };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * @brief Computes the index into the piece-square key array.
         *
         * @param t_color The color of the piece.
         * @param t_pieceType The type of the piece.
         * @param t_square The square index (0-63).
         *
         * @return The computed flat array index.
         */
        static constexpr size_t Index(const Color t_color, const PieceType t_pieceType, const Square t_square)
        {
            return static_cast<size_t>(t_color) * NUM_PIECE_TYPES * NUM_SQUARES
                + static_cast<size_t>(t_pieceType) * NUM_SQUARES
                + static_cast<size_t>(t_square);
        }
    };
}
