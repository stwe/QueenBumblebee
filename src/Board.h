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

#include <optional>
#include "Bitboard.h"
#include "Move.h"
#include "Zobrist.h"

namespace qb
{
    //-------------------------------------------------
    // PositionState
    //-------------------------------------------------

    /**
     * @brief Represents the state of a position.
     */
    struct PositionState
    {
        /** @brief The current En Passant target square. */
        Square enPassantSquare{ SQ_NONE };

        /** @brief The current side to move. */
        Color side{ NO_COLOR };

        /** @brief The castling rights. */
        CastlingRights castlingRights{ NO_CASTLING };

        /** @brief Represents the positions of pieces on the chessboard. */
        std::array<Piece, 64> pieceBoard;

        /** @brief The Zobrist hash key representing the unique state of the board. */
        uint64_t zobristKey{ 0 };

        /** @brief Number of half-moves since the last capture or pawn move (for the fifty-move rule). */
        uint32_t halfmoveClock{ 0 };

        /** @brief The current full move number (starts at 1, incremented after Black’s move). */
        uint32_t fullmoveNumber{ 1 };
    };

    //-------------------------------------------------
    // MoveState
    //-------------------------------------------------

    /**
     * @brief Represents the state of a move.
     */
    struct MoveState
    {
        /** @brief Backup of the PositionState partes that can be changed by a move. */
        Square enPassantSquare{ SQ_NONE };
        Color side{ NO_COLOR };
        CastlingRights castlingRights{ NO_CASTLING };

        /** @brief Additionally: captured piece (important for UnmakeMove). */
        Piece capturedPiece{ NO_PIECE_TYPE, NO_COLOR };

        /** @brief For complete history. */
        Move move;

        /** @brief A unique hash value representing the state of the chessboard. */
        uint64_t zobristKey{ 0 };

        /** @brief Number of half-moves since the last capture or pawn move (for the fifty-move rule). */
        uint32_t halfmoveClock{ 0 };

        /** @brief The current full move number (starts at 1, incremented after Black’s move). */
        uint32_t fullmoveNumber{ 1 };
    };

    //-------------------------------------------------
    // Board
    //-------------------------------------------------

    /**
     * @brief Represents a chessboard and provides functionality to manipulate it.
     */
    class Board
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Structure containing bitboards for all pieces and colors.
         */
        Bitboards bitboards;

        /**
         * @brief Structure holding the current position state of the board.
         */
        PositionState positionState;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Board();

        Board(const Board& t_other) = delete;
        Board(Board&& t_other) noexcept = delete;
        Board& operator=(const Board& t_other) = delete;
        Board& operator=(Board&& t_other) noexcept = delete;

        ~Board() noexcept;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * @brief Initializes the board with the provided FEN string.
         *
         * @param t_fenString The FEN string representing the board position.
         *
         * @return True if the FEN string was successfully parsed and the board initialized.
         */
        bool InitWithFen(const std::string& t_fenString);

        /**
         * @brief Initializes the board using explicit bitboards and position state.
         *
         * This function bypasses FEN parsing and sets up the board directly from the given
         * bitboard representation and associated position state (e.g., side to move,
         * castling rights, en passant square).
         *
         * @param t_bitboards Bitboard structure containing positions of all pieces.
         * @param t_positionState Current game state including side to move and other metadata.
         *
         * @return true if initialization was successful, false otherwise.
         */
        bool InitWithBitboards(const Bitboards& t_bitboards, const PositionState& t_positionState);

        /**
         * @brief Initializes the board to the standard starting position.
         *
         * This method sets up the board using the default FEN string that
         * represents the initial chess position.
         *
         * @return True if the board was successfully initialized, false otherwise.
         */
        bool SetStartPosition();

        //-------------------------------------------------
        // Reset
        //-------------------------------------------------

        /**
         * @brief Resets the board to the initial state.
         *
         * This method clears the bitboards, position state, and move history,
         * and then reinitializes the board to the standard starting position.
         *
         * @return True if the reset was successful, false otherwise.
         */
        bool Reset();

        //-------------------------------------------------
        // Move
        //-------------------------------------------------

        /**
         * @brief Applies a move.
         *
         * @param t_move The move to be made.
         */
        void MakeMove(const Move& t_move);

        /**
         * @brief Reverts the most recent move, restoring the previous board state.
         */
        void UnmakeMove();

        /**
         * @brief Places a piece on the given square and updates the board state accordingly.
         *
         * @param t_square The square where the piece will be placed.
         * @param t_piece The piece to be placed.
         */
        void SetPiece(Square t_square, const Piece& t_piece);

        /**
         * @brief Removes a piece from the given square and updates the board state accordingly.
         *
         * @param t_square The square from which the piece will be removed.
         * @param t_piece The piece to be removed.
         */
        void RemovePiece(Square t_square, const Piece& t_piece);

        /**
         * @brief Updates castling rights based on the provided move.
         *
         * @param t_move The move that may affect castling rights.
         */
        void UpdateCastlingRights(Move t_move);

        //-------------------------------------------------
        // King Safety & Attack Detection
        //-------------------------------------------------

        /**
         * @brief Checks if the king of the given side is currently in check.
         *
         * Determines whether the king of the specified color is under attack
         * by any opposing pieces.
         *
         * @param t_bySide The color of the side to check.
         *
         * @return true if the king is in check, false otherwise.
         */
        [[nodiscard]] bool IsKingInCheck(Color t_bySide) const;

        /**
         * @brief Checks if a square is attacked by the given side.
         *
         * Evaluates whether the specified square is targeted by any piece
         * of the specified color.
         *
         * @param t_bySide The color of the attacking side.
         * @param t_targetSq The square to test for attacks.
         *
         * @return True if the square is under attack, false otherwise.
         */
        [[nodiscard]] bool IsSquareAttacked(Color t_bySide, Square t_targetSq) const;

        //-------------------------------------------------
        // Castling Rights
        //-------------------------------------------------

        /**
         * @brief Checks whether castling is safe with respect to enemy attacks.
         *
         * Determines whether the king and the squares it must pass through
         * during castling are under attack by the opposing side.
         *
         * This method does not check other castling conditions such as
         * - whether the king or rook has already moved
         * - whether the path is unobstructed
         * - whether castling rights are still available
         *
         * @param t_kingSq The square the king is currently on.
         * @param t_transit1 The first square the king would move to during castling.
         * @param t_transit2 The second square the king would move through (or end on).
         * @param t_bySide The color of the side attempting to castle.
         *
         * @return True if none of the squares are under attack, false otherwise.
         */
        [[nodiscard]] bool CanCastle(Square t_kingSq, Square t_transit1, Square t_transit2, Color t_bySide) const;

        //-------------------------------------------------
        // Print
        //-------------------------------------------------

        /**
         * @brief Prints the current state of the chessboard with color formatting.
         */
        void ColorPrintBoard() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Lookup table for piece-specific bitboards.
         *
         * m_pieceBitboards is a 2D array that maps each piece type, for both colors,
         * to a pointer to the corresponding bitboard. This allows for efficient access
         * to a piece's position on the board based on its type and color.
         *
         * The first dimension represents the color (WHITE, BLACK).
         * The second dimension represents the piece type (PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING).
         */
        U64* m_pieceBitboards[2][6]{};

        /**
         * @brief The history of moves made during the game.
         */
        std::vector<MoveState> m_moveHistory;

        /**
         * @brief Precomputed Zobrist hash keys for efficient board state hashing.
         */
        ZobristKeys m_zobristKeys;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * @brief Initializes the lookup array for piece bitboards.
         *
         * Maps each piece type, differentiated by color, to its corresponding
         * bitboard for efficient access and manipulation during gameplay.
         */
        void InitLookupArray();

        /**
         * @brief Sets the bitboards for given pieces at a specified Rank.
         *
         * @param t_pieces The string representing the pieces.
         * @param t_rank The Rank to set the bitboards for.
         */
        bool SetBitboards(const std::string& t_pieces, Rank t_rank);

        /**
         * @brief Updates the internal board state for a specific piece at a given square.
         *
         * Modifies both the piece-specific bitboards and the unified piece board array (`pieceBoard`)
         * by either setting or clearing the specified square for the given piece.
         * Also triggers an update of the common bitboards to reflect the overall board state.
         *
         * @param t_square The square to update.
         * @param t_piece The piece to be placed or removed, defined by color and type.
         * @param t_set If true, the piece is placed on the square; if false, the piece is removed.
         */
        void UpdatePieceAtSquare(Square t_square, const Piece& t_piece, bool t_set);

        /**
         * @brief Updates the common bitboards for the board state.
         *
         * This method recalculates the aggregated bitboards for white pieces, black pieces,
         * occupied squares, and empty squares based on the individual piece bitboards.
         */
        void UpdateCommonBitboards();

        /**
         * @brief Returns a reference to the bitboard corresponding to the given piece.
         *
         * @param t_piece The piece for which to retrieve the bitboard.
         *
         * @return std::optional<std::reference_wrapper<U64>> A reference to the corresponding bitboard,
         *         or std::nullopt if the piece type is invalid.
         */
        std::optional<std::reference_wrapper<U64>> GetBitboard(Piece t_piece);

        /**
         * @brief Retrieves the castling rights as a string representation.
         *
         * @return A string representing the castling rights in standard chess notation.
         */
        [[nodiscard]] std::string GetCastlingRightsString() const;
    };
}
