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

#include "Move.h"
#include "polyglot/Polyglot.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace qb
{
    /** @brief Forward declaration class Board. */
    class Board;
}

namespace qb::uci
{
    //-------------------------------------------------
    // UciClient
    //-------------------------------------------------

    /**
     * @brief The UciClient class handles the UCI (Universal Chess Interface) protocol.
     *
     * It communicates with the GUI, interprets commands, manages game state and uses an opening book
     * if provided to play moves.
     */
    class UciClient
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        UciClient() = delete;

        /**
         * @brief Constructs a UciClient with a given board.
         *
         * @param t_board Pointer to the Board object.
         */
        explicit UciClient(Board* t_board);

        /**
         * @brief Constructs a UciClient with a given opening book and board.
         *
         * @param t_bookName Name of the Polyglot book file.
         * @param t_board Pointer to the Board object.
         */
        UciClient(const std::string& t_bookName, Board* t_board);

        UciClient(const UciClient& t_other) = delete;
        UciClient(UciClient&& t_other) noexcept = delete;
        UciClient& operator=(const UciClient& t_other) = delete;
        UciClient& operator=(UciClient&& t_other) noexcept = delete;

        ~UciClient() noexcept;

        //-------------------------------------------------
        // Main UCI Loop
        //-------------------------------------------------

        /**
         * @brief Starts the main loop to process UCI commands.
         */
        void Run();

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Pointer to the game board.
         */
        Board* m_board{ nullptr };

        /**
         * @brief Polyglot opening book object.
         */
        polyglot::Book m_book;

        /**
         * @brief Flag indicating whether the book has been loaded.
         */
        bool m_bookLoaded{ false };

        /**
         * @brief Flag indicating whether the engine should use the opening book.
         */
        bool m_useOpeningBook{ true };

        //-------------------------------------------------
        // Command Handling
        //-------------------------------------------------

        /**
         * @brief Sends UCI engine identification and available options to the GUI.
         *
         * This includes engine name, author, and all configurable UCI options.
         */
        static void SendUciInfo();

        /**
         * @brief Parses and handles an incoming UCI command.
         *
         * @param t_command The command keyword.
         * @param t_stream Stream containing the remaining command input.
         */
        void HandleCommand(const std::string& t_command, std::istringstream& t_stream);

        /**
         * @brief Handles the "ucinewgame" command to reset engine state.
         */
        void HandleNewGame() const;

        /**
         * @brief Handles the custom "perft" command for debugging move generation.
         *
         * @param t_stream Stream containing the depth argument for the perft test.
         */
        void HandlePerft(std::istringstream& t_stream) const;

        /**
         * @brief Handles the "setoption" command to modify UCI options.
         *
         * Parses option name and value from the stream and applies them accordingly.
         *
         * @param t_stream Stream containing the option name and value.
         */
        void HandleSetOption(std::istringstream& t_stream);

        //-------------------------------------------------
        // Position Handling
        //-------------------------------------------------

        /**
         * @brief Handles the "position" command to set up a board state.
         *
         * @param t_stream Stream containing the position data.
         */
        void HandlePosition(std::istringstream& t_stream) const;

        /**
         * @brief Applies a list of moves to the current board state.
         *
         * @param t_stream Stream containing the move list.
         */
        void ApplyMoves(std::istringstream& t_stream) const;

        //-------------------------------------------------
        // Move Decision
        //-------------------------------------------------

        /**
         * @brief Handles the "go" command to initiate move generation.
         */
        void HandleGo() const;

        /**
         * @brief Attempts to find a move from the opening book.
         *
         * @return True if a book move was found and played; otherwise false.
         */
        [[nodiscard]] bool TryBookMove() const;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * @brief Sends a UCI-compatible command string to the GUI.
         *
         * @param t_command The command string to send.
         */
        static void SendCommand(const std::string& t_command);

        /**
         * @brief Parses a UCI move string into a Move object.
         *
         * @param t_moveStr The move string in UCI format (e.g., "e2e4").
         *
         * @return A valid Move object representing the parsed move.
         */
        [[nodiscard]] Move ParseUciMove(const std::string& t_moveStr) const;

        [[nodiscard]] bool ShouldUseBook() const;
    };
}
