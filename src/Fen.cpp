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

#include "Fen.h"
#include "Assert.h"
#include <charconv>

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::Fen::Fen()
    : Fen{ std::string(FEN_START) }
{
    QB_LOG_DEBUG("[Fen::Fen()] Create Fen object.");
}

qb::Fen::Fen(std::string t_fenString)
    : m_fenString{ std::move(t_fenString) }
{
    QB_LOG_DEBUG("[Fen::Fen()] Create Fen object.");
}

qb::Fen::~Fen() noexcept
{
    QB_LOG_DEBUG("[Fen::~Fen()] Destruct Fen object.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const std::vector<std::string>& qb::Fen::GetPiecePlacement() const
{
    return m_piecePlacement;
}

qb::Color qb::Fen::GetSideToMove() const
{
    return m_sideToMove;
}

qb::CastlingRights qb::Fen::GetCastlingRights() const
{
    return m_castlingRights;
}

qb::Square qb::Fen::GetEnPassantSquare() const
{
    return m_enPassantSquare;
}

int qb::Fen::GetHalfmoveClock() const
{
    return m_halfmoveClock;
}

int qb::Fen::GetFullmoveNumber() const
{
    return m_fullmoveNumber;
}

//-------------------------------------------------
// Parsing Logic
//-------------------------------------------------

bool qb::Fen::Parse()
{
    // Use string_view for the main split to avoid heap allocations
    const auto tokens{ Split(m_fenString, ' ') };
    if (tokens.size() < 4)
    {
        QB_LOG_ERROR("[Fen::Parse()] FEN string must have at least 4 fields.");
        return false;
    }

    // 1. Piece placement
    const auto rankTokens{ Split(tokens[0], '/') };
    if (rankTokens.size() != 8)
    {
        QB_LOG_ERROR("[Fen::Parse()] Invalid piece placement in FEN.");
        return false;
    }

    m_piecePlacement.clear();
    m_piecePlacement.reserve(8);
    for (const auto& rankStr : rankTokens)
    {
        m_piecePlacement.emplace_back(rankStr);
    }

    // 2. Side to move
    if (tokens[1] == "w")
    {
        m_sideToMove = WHITE;
    }
    else if (tokens[1] == "b")
    {
        m_sideToMove = BLACK;
    }
    else
    {
        QB_LOG_ERROR("[Fen::Parse()] Invalid side to move in FEN.");
        return false;
    }

    // 3. Castling rights
    m_castlingRights = NO_CASTLING;
    if (const auto& castling{ tokens[2] }; castling != "-")
    {
        for (const char c : castling)
        {
            switch (c)
            {
            case 'K':
                m_castlingRights |= WHITE_OO;
                break;
            case 'Q':
                m_castlingRights |= WHITE_OOO;
                break;
            case 'k':
                m_castlingRights |= BLACK_OO;
                break;
            case 'q':
                m_castlingRights |= BLACK_OOO;
                break;
            default:
                QB_LOG_ERROR("[Fen::Parse()] Invalid castling rights in FEN.");
                return false;
            }
        }
    }

    // 4. En passant square
    if (const auto ep{ tokens[3] }; ep == "-")
    {
        m_enPassantSquare = SQ_NONE;
    }
    else if (ep.length() == 2 && ep[0] >= 'a' && ep[0] <= 'h' && (ep[1] == '3' || ep[1] == '6'))
    {
        // Calculate square index directly (assuming standard mapping: a1=0, b1=1 ... h8=63)
        const int file = ep[0] - 'a';
        const int rank = ep[1] - '1';
        m_enPassantSquare = static_cast<Square>(rank * 8 + file);
    }
    else
    {
        QB_LOG_ERROR("[Fen::Parse()] Invalid en passant square in FEN.");
        return false;
    }

    // 5. & 6. Halfmove clock and Fullmove number (Optional in some sloppy FENs, but standard)
    if (tokens.size() >= 5)
    {
        std::from_chars(tokens[4].data(), tokens[4].data() + tokens[4].size(), m_halfmoveClock);
    }

    if (tokens.size() >= 6)
    {
        std::from_chars(tokens[5].data(), tokens[5].data() + tokens[5].size(), m_fullmoveNumber);
    }

    return true;
}

//-------------------------------------------------
// Parsing Helper
//-------------------------------------------------

std::vector<std::string_view> qb::Fen::Split(std::string_view t_str, const char t_delimiter)
{
    std::vector<std::string_view> result;
    size_t start{ 0 };
    size_t end{ t_str.find(t_delimiter) };

    while (end != std::string_view::npos)
    {
        result.emplace_back(t_str.substr(start, end - start));
        start = end + 1;
        end = t_str.find(t_delimiter, start);
    }

    result.emplace_back(t_str.substr(start));

    return result;
}
