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

#include "Utils.h"

// --------------------------------------------------------------------------------
// Move-Record: 26 Bit
// --------------------------------------------------------------------------------
// bit 00-05: from square (0 to 63)                                          6 Bits
// bit 06-11: to square (0 to 63)                                            6 Bits
// bit 12-14: moving piece type                                              3 Bits
// bit 15-17: promotion piece type                                           3 Bits
// bit 18-19: move type (normal, promo, en passant, castling)                2 Bits
// bit 20-22: captured piece type (0 = no capture, otherwise like PieceType) 3 Bits
// --------------------------------------------------------------------------------

namespace qb
{
    //-------------------------------------------------
    // Move
    //-------------------------------------------------

    /**
     * @brief Represents a chess move encoded as a 26-bit value.
     *
     * This struct compactly stores all information about a move, including the
     * origin and destination squares, the moving piece type, optional promotion,
     * move type, and any captured piece.
     */
    struct Move
    {
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr auto TO_SQUARE_OFFSET{ 6 };
        static constexpr auto PIECE_TYPE_OFFSET{ 12 };
        static constexpr auto PROMOTION_PIECE_TYPE_OFFSET{ 15 };
        static constexpr auto MOVE_TYPE_OFFSET{ 18 };
        static constexpr auto CAPTURED_PIECE_TYPE_OFFSET{ 20 };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Encoded move as a 32-bit unsigned integer (only lower 26 bits used).
         */
        uint32_t move{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Move() = default;

        /**
         * @brief Constructs a new Move object.
         *
         * @param t_from The origin square.
         * @param t_to The destination square.
         * @param t_pieceType The type of the moving piece.
         * @param t_promotionPieceType The type of the promotion piece (if applicable).
         * @param t_moveType The type of move (normal, promotion, en passant, castling).
         * @param t_capturedPieceType The type of captured piece (if any).
         */
        constexpr Move(
            const Square t_from,
            const Square t_to,
            const PieceType t_pieceType,
            const PieceType t_promotionPieceType = NO_PIECE_TYPE,
            const MoveType t_moveType = NORMAL,
            const PieceType t_capturedPieceType = NO_PIECE_TYPE
        )
            : move(t_from +
                (t_to << TO_SQUARE_OFFSET) +
                (t_pieceType << PIECE_TYPE_OFFSET) +
                (t_promotionPieceType << PROMOTION_PIECE_TYPE_OFFSET) +
                (t_moveType << MOVE_TYPE_OFFSET) +
                (t_capturedPieceType << CAPTURED_PIECE_TYPE_OFFSET)
            )
        {
            QB_ASSERT(is_valid_square(t_from), "[Move::Move()] From square out of bounds.");
            QB_ASSERT(is_valid_square(t_to), "[Move::Move()] To square out of bounds.");
            QB_ASSERT(is_valid_piece_type(t_pieceType), "[Move::Move()] Invalid moving piece type.");

            QB_ASSERT(
                t_moveType != PROMOTION ||
                (t_promotionPieceType != NO_PIECE_TYPE &&
                    t_promotionPieceType != PAWN &&
                    t_promotionPieceType != KING),
                "[Move::Move()] Promotion move must have a valid promotion piece type."
            );

            QB_ASSERT((move & 0xFC000000) == 0, "[Move::Move()] Move value exceeds 26-bit limit.");
        }

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Returns the origin square of the move.
         *
         * @return The from-square as Square enum.
         */
        [[nodiscard]] constexpr Square FromSquare() const
        {
            return static_cast<Square>(move & 0x3F);
        }

        /**
         * @brief Returns the destination square of the move.
         *
         * @return The to-square as Square enum.
         */
        [[nodiscard]] constexpr Square ToSquare() const
        {
            return static_cast<Square>(move >> TO_SQUARE_OFFSET & 0x3F);
        }

        /**
         * @brief Returns the type of the piece that is moving.
         *
         * @return The PieceType of the moving piece.
         */
        [[nodiscard]] constexpr PieceType TypeOfPiece() const
        {
            return static_cast<PieceType>(move >> PIECE_TYPE_OFFSET & 0x07);
        }

        /**
         * @brief Returns the promotion piece type if applicable.
         *
         * @return The PieceType of the promoted piece.
         */
        [[nodiscard]] constexpr PieceType TypeOfPromotionPiece() const
        {
            return static_cast<PieceType>(move >> PROMOTION_PIECE_TYPE_OFFSET & 0x07);
        }

        /**
         * @brief Returns the move type.
         *
         * @return The MoveType of this move.
         */
        [[nodiscard]] constexpr MoveType TypeOfMove() const
        {
            return static_cast<MoveType>(move >> MOVE_TYPE_OFFSET & 0x03);
        }

        /**
         * @brief Returns the type of captured piece if any.
         *
         * @return The PieceType of the captured piece.
         */
        [[nodiscard]] constexpr PieceType TypeOfCapturedPiece() const
        {
            return static_cast<PieceType>(move >> CAPTURED_PIECE_TYPE_OFFSET & 0x07);
        }

        /**
         * @brief Converts the Move to its string representation.
         */
        [[nodiscard]] std::string ToString() const
        {
            std::string str;

            str += SQUARE_NAMES[FromSquare()];
            str += SQUARE_NAMES[ToSquare()];

            if (TypeOfMove() == PROMOTION)
            {
                switch (TypeOfPromotionPiece())
                {
                case KNIGHT: str += "n"; break;
                case BISHOP: str += "b"; break;
                case ROOK:   str += "r"; break;
                case QUEEN:  str += "q"; break;
                default: break;
                }
            }

            return str;
        }

        constexpr bool operator==(const Move& t_other) const noexcept
        {
            return move == t_other.move;
        }
    };

    inline std::ostream& operator<<(std::ostream& t_os, const Move& t_move)
    {
        t_os << t_move.ToString();
        return t_os;
    }
}
