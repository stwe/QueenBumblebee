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

// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppUseStructuredBinding

#include <iostream>
#include "UciClient.h"
#include "Board.h"
#include "Search.h"
#include "MoveGenerator.h"
#include "Perft.h"
#include "polyglot/Polyglot.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::uci::UciClient::UciClient(Board* t_board)
    : m_board{ t_board }
{
    QB_ASSERT(m_board, "[UciClient::UciClient()] Null pointer.")

    QB_LOG_DEBUG("[UciClient::UciClient()] Create UciClient object.");
}

qb::uci::UciClient::UciClient(const std::string& t_bookName, Board* t_board)
    : UciClient(t_board)
{
    QB_LOG_DEBUG("[UciClient::UciClient()] Create UciClient object.");

    if (m_book.Open(t_bookName))
    {
        m_bookLoaded = true;
        QB_LOG_DEBUG("[UciClient::UciClient()] Loaded {} successfully.", t_bookName);
    }
    else
    {
        QB_LOG_ERROR("[UciClient::UciClient()] Failed to load {}.", t_bookName);
    }
}

qb::uci::UciClient::~UciClient() noexcept
{
    QB_LOG_DEBUG("[UciClient::~UciClient()] Destruct UciClient object.");
}

//-------------------------------------------------
// Main UCI Loop
//-------------------------------------------------

void qb::uci::UciClient::Run()
{
    std::string line;
    std::cout.setf(std::ios::unitbuf);

    SendUciInfo();

    while (std::getline(std::cin, line))
    {
        if (line.empty()) continue;

        QB_LOG_DEBUG("[UCI IN] {}", line);
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        HandleCommand(command, iss);
    }
}

//-------------------------------------------------
// Command Handling
//-------------------------------------------------

void qb::uci::UciClient::SendUciInfo()
{
    SendCommand("id name QueenBumblebee");
    SendCommand("id author stwe");
    SendCommand("option name UseBook type check default true");
    SendCommand("uciok");
}

void qb::uci::UciClient::HandleCommand(const std::string& t_command, std::istringstream& t_stream)
{
    if (t_command == "uci")
    {
        SendUciInfo();
    }
    else if (t_command == "isready")
    {
        SendCommand("readyok");
    }
    else if (t_command == "ucinewgame")
    {
        HandleNewGame();
    }
    else if (t_command == "position")
    {
        HandlePosition(t_stream);
    }
    else if (t_command == "setoption")
    {
        HandleSetOption(t_stream);
    }
    else if (t_command == "go")
    {
        HandleGo();
    }
    else if (t_command == "stop")
    {
        //search->Stop();
    }
    else if (t_command == "ponderhit")
    {
    }
    else if (t_command == "quit")
    {
        std::exit(0);
    }
    else if (t_command == "perft")
    {
        HandlePerft(t_stream);
    }
    else if (t_command == "display")
    {
        m_board->ColorPrintBoard();
    }
    else
    {
        QB_LOG_DEBUG("[UCI IN] Unknown command: {}", t_command);
    }
}

void qb::uci::UciClient::HandleNewGame() const
{
    m_board->Reset();
}

void qb::uci::UciClient::HandlePerft(std::istringstream& t_stream) const
{
    int depth{ 0 };
    t_stream >> depth;

    if (depth <= 0)
    {
        SendCommand("info string Invalid perft depth.");
        return;
    }

    MoveGenerator moveGenerator(m_board);

    const auto start{ std::chrono::high_resolution_clock::now() };
    const auto nodes{ Perft::RunPerft(m_board, &moveGenerator, depth) };
    const auto end{ std::chrono::high_resolution_clock::now() };

    const std::chrono::duration<double> elapsed{ end - start };
    const auto nps{ static_cast<uint64_t>(static_cast<double>(nodes) / elapsed.count()) };

    SendCommand("info string Perft(" + std::to_string(depth) + ") = " + std::to_string(nodes) +
        " runtime: " + std::to_string(elapsed.count()) + "s" +
        " NPS: " + std::to_string(nps));
}

void qb::uci::UciClient::HandleSetOption(std::istringstream& t_stream)
{
    std::string token;
    std::string name;
    std::string value;

    while (t_stream >> token)
    {
        if (token == "name")
        {
            name.clear();
            while (t_stream >> token && token != "value")
            {
                name += token + " ";
            }

            name = name.substr(0, name.size() - 1);
        }

        if (token == "value")
        {
            value.clear();
            while (t_stream >> token)
            {
                value += token + " ";
            }

            value = value.substr(0, value.size() - 1);
        }
    }

    QB_LOG_DEBUG("[UCI] SetOption name: {} value: {}", name, value);

    if (name == "UseBook")
    {
        std::string val{ value };
        std::ranges::transform(val, val.begin(),
            [](const unsigned char t_c) { return std::tolower(t_c); }
        );

        m_useOpeningBook = val == "true" || val == "1" || val == "yes" || val == "on";
    }
}

//-------------------------------------------------
// Position Handling
//-------------------------------------------------

void qb::uci::UciClient::HandlePosition(std::istringstream& t_stream) const
{
    m_board->Reset();

    std::string token;
    t_stream >> token;

    if (token == "startpos")
    {
        if (!m_board->SetStartPosition())
        {
            QB_LOG_ERROR("[UciClient::HandlePosition()] Error setting start position.");
            return;
        }
    }
    else if (token == "fen")
    {
        std::string fen, word;
        for (auto i{ 0 }; i < 6 && t_stream >> word; ++i)
        {
            fen += word + " ";
        }

        if (!m_board->InitWithFen(fen))
        {
            QB_LOG_ERROR("[UciClient::HandlePosition()] Error parsing FEN: {}", fen);
            return;
        }
    }

    if (t_stream >> token && token == "moves")
    {
        ApplyMoves(t_stream);
    }
}

void qb::uci::UciClient::ApplyMoves(std::istringstream& t_stream) const
{
    std::string moveStr;
    while (t_stream >> moveStr)
    {
        const Move move{ ParseUciMove(moveStr) };
        if (move.move == 0)
        {
            QB_LOG_ERROR("[UciClient::ApplyMoves()] Invalid move: {}", moveStr);
            break;
        }

        m_board->MakeMove(move);
    }
}

//-------------------------------------------------
// Move Decision
//-------------------------------------------------

void qb::uci::UciClient::HandleGo() const
{
    if (ShouldUseBook() && TryBookMove())
    {
        return;
    }

    MoveGenerator moveGenerator(m_board);
    Search search(m_board, &moveGenerator);

    const Move bestMove{ search.SearchBestMove(4) }; // todo: depth inc

    if (bestMove.FromSquare() != bestMove.ToSquare()) // todo: IsValid()
    {
        SendCommand("bestmove " + bestMove.ToString());
    }
    else
    {
        SendCommand("bestmove 0000"); // todo: no valid move
    }
}

bool qb::uci::UciClient::TryBookMove() const
{
    if (!ShouldUseBook())
    {
        return false;
    }

    const auto range{ m_book.Probe(polyglot::Book::PolyglotHash(m_board)) };
    if (range.first == range.second)
    {
        QB_LOG_DEBUG("[UciClient::TryBookMove()] No book move found.");
        return false;
    }

    const auto best = std::max_element(range.first, range.second,
        [](const auto& t_a, const auto& t_b) {
        return t_a.weight < t_b.weight;
    });

    const auto moveStr{ polyglot::Book::PolyMoveToUci(best->move) };
    QB_LOG_DEBUG("[UciClient::TryBookMove()] Sending book move {}.", moveStr);
    SendCommand("bestmove " + moveStr);

    return true;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void qb::uci::UciClient::SendCommand(const std::string& t_command)
{
    QB_LOG_DEBUG("[UCI OUT] {}", t_command);
    std::cout << t_command << std::endl;
}

qb::Move qb::uci::UciClient::ParseUciMove(const std::string& t_moveStr) const
{
    if (t_moveStr.length() < 4)
    {
        QB_LOG_ERROR("[Board::ParseUciMove()] Invalid UCI move: {}", t_moveStr);
        return {};
    }

    const std::string fromStr{ t_moveStr.substr(0, 2) };
    const std::string toStr{ polyglot::Book::NormalizeCastlingToSquare(t_moveStr) };

    const auto from{ square_from_string(fromStr) };
    const auto to{ square_from_string(toStr) };

    const MoveGenerator moveGen(m_board);
    const auto legalMoves{ moveGen.GenerateLegalMoves() };

    for (const Move& move : legalMoves)
    {
        if (move.FromSquare() != from || move.ToSquare() != to)
        {
            continue;
        }

        if (t_moveStr.length() == 5)
        {
            const auto expectedPromo{ piece_type_from_char(t_moveStr[4]) };
            if (expectedPromo == NO_PIECE_TYPE || expectedPromo == KING || expectedPromo == PAWN)
            {
                QB_LOG_ERROR("[Board::ParseUciMove()] Invalid promotion piece: {}.", t_moveStr[4]);
                return {};
            }

            if (move.TypeOfMove() == PROMOTION && move.TypeOfPromotionPiece() == expectedPromo)
            {
                return move;
            }
        }
        else if (move.TypeOfMove() != PROMOTION)
        {
            return move;
        }
    }

    QB_LOG_ERROR("[Board::ParseUciMove()] No legal move found for: {}", t_moveStr);

    return {};
}

bool qb::uci::UciClient::ShouldUseBook() const
{
    return m_useOpeningBook && m_bookLoaded;
}
