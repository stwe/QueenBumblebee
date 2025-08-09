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

#include <vector>
#include "Types.h"

namespace qb
{
    //-------------------------------------------------
    // Fen
    //-------------------------------------------------

    /**
     * @brief Represents a parser for Forsyth–Edwards Notation (FEN).
     *
     * The Fen class is responsible for parsing a FEN string and extracting chess board state:
     * piece placement, side to move, castling rights, and en passant square.
     */
    class Fen
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * @brief The FEN string representing the standard initial chess position.
         */
        static constexpr std::string_view FEN_START{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Fen();

        /**
         * @brief Constructs a Fen parser with the given FEN string.
         *
         * @param t_fenString The FEN string to be parsed.
         */
        explicit Fen(std::string t_fenString);

        Fen(const Fen& t_other) = delete;
        Fen(Fen&& t_other) noexcept = delete;
        Fen& operator=(const Fen& t_other) = delete;
        Fen& operator=(Fen&& t_other) noexcept = delete;

        ~Fen() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Returns the piece placement field as a vector of ranks.
         *
         * @return A vector of 8 strings (1 per rank), each describing the piece placement.
         */
        [[nodiscard]] const std::vector<std::string>& GetPiecePlacement() const;

        /**
         * @brief Returns which side is to move.
         *
         * @return WHITE or BLACK depending on the FEN string.
         */
        [[nodiscard]] Color GetSideToMove() const;

        /**
         * @brief Returns the castling rights extracted from the FEN string.
         *
         * @return A bitmask of castling rights.
         */
        [[nodiscard]] CastlingRights GetCastlingRights() const;

        /**
         * @brief Returns the en passant square if one exists.
         *
         * @return The en passant square, or SQ_NONE if not set.
         */
        [[nodiscard]] Square GetEnPassantSquare() const;

        //-------------------------------------------------
        // Parsing Logic
        //-------------------------------------------------

        /**
         * @brief Parses the stored FEN string and populates internal members.
         *
         * @return true if parsing was successful, false if the FEN string is invalid.
         */
        [[nodiscard]] bool Parse();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief The FEN string to be parsed.
         */
        std::string m_fenString;

        /**
         * @brief Piece placement field parsed into ranks (8 rows).
         */
        std::vector<std::string> m_piecePlacement;

        /**
         * @brief The side to move (WHITE or BLACK).
         */
        Color m_sideToMove{ NO_COLOR };

        /**
         * @brief Bitmask representing castling rights.
         */
        CastlingRights m_castlingRights{ NO_CASTLING };

        /**
         * @brief En passant target square.
         */
        Square m_enPassantSquare{ SQ_NONE };

        //-------------------------------------------------
        // Parsing Helper
        //-------------------------------------------------

        /**
         * @brief Helper function to split a string by a delimiter.
         *
         * @param t_str The input string to split.
         * @param t_delimiter The delimiter character.
         *
         * @return A vector of strings after splitting.
         */
        static std::vector<std::string> Split(const std::string& t_str, char t_delimiter);
    };
}
