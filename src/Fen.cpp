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

#include "Fen.h"
#include "Assert.h"

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

//-------------------------------------------------
// Parsing Logic
//-------------------------------------------------

bool qb::Fen::Parse()
{
    const auto tokens{ Split(m_fenString, ' ') };
    if (tokens.size() < 4)
    {
        QB_LOG_ERROR("[Fen::Parse()] FEN string must have at least 4 fields.");
        return false;
    }

    // 1. Piece placement
    m_piecePlacement = Split(tokens[0], '/');
    if (m_piecePlacement.size() != 8)
    {
        QB_LOG_ERROR("[Fen::Parse()] Invalid piece placement in FEN.");
        return false;
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
    if (const auto& ep{ tokens[3] }; ep == "-")
    {
        m_enPassantSquare = SQ_NONE;
    }
    else
    {
        auto found{ false };
        for (auto i{ 0 }; i < 64; ++i)
        {
            if (SQUARE_NAMES[i] == ep)
            {
                m_enPassantSquare = static_cast<Square>(i);
                found = true;

                break;
            }
        }
        if (!found)
        {
            QB_LOG_ERROR("[Fen::Parse()] Invalid en passant square in FEN.");
            return false;
        }
    }

    return true;
}

//-------------------------------------------------
// Parsing Helper
//-------------------------------------------------

std::vector<std::string> qb::Fen::Split(const std::string& t_str, const char t_delimiter)
{
    std::vector<std::string> result;
    size_t start{ 0 };
    while (start < t_str.size())
    {
        const auto end{ t_str.find(t_delimiter, start) };
        if (end == std::string_view::npos)
        {
            result.emplace_back(t_str.substr(start));
            break;
        }

        result.emplace_back(t_str.substr(start, end - start));
        start = end + 1;
    }

    return result;
}
