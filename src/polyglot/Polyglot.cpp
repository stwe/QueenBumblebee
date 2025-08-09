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

// ReSharper disable CppDFANullDereference

#include <fstream>
#include "Types.h"
#include "Polyglot.h"
#include "Board.h"
#include "PolyglotKeys.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::polyglot::Book::Book()
{
    QB_LOG_DEBUG("[Book::Book()] Create Book object.");
}

qb::polyglot::Book::~Book() noexcept
{
    QB_LOG_DEBUG("[Book::~Book()] Destruct Book object.");
}

//-------------------------------------------------
// Loading
//-------------------------------------------------

bool qb::polyglot::Book::Open(const std::string& t_fileName)
{
    QB_LOG_DEBUG("[Book::Open()] Trying to load {}.", t_fileName);

    std::ifstream fil(t_fileName, std::ios::binary | std::ios::ate);
    if (!fil)
    {
        QB_LOG_ERROR("[Book::Open()] Could not open {}.", t_fileName);
        return false;
    }

    const auto sizePos{ fil.tellg() };
    if (sizePos == std::streampos(-1))
    {
        QB_LOG_ERROR("[Book::Open()] tellg() failed.");
        return false;
    }

    const auto size{ static_cast<std::size_t>(sizePos) };
    if (size % sizeof(Entry) != 0)
    {
        QB_LOG_ERROR("[Book::Open()] File size not multiple of 16 bytes – corrupt book.");
        return false;
    }

    const auto count{ size / sizeof(Entry) };
    if (count == 0)
    {
        QB_LOG_ERROR("[Book::Open()] No entries in {}.", t_fileName);
        return false;
    }

    QB_LOG_DEBUG("[Book::Open()] {} entries found in {}.", count, t_fileName);

    m_entries.clear();
    m_entries.resize(count);

    fil.seekg(0);
    for (auto i{ 0 }; i < count; ++i)
    {
        Entry entry{};
        if (!fil.read(reinterpret_cast<char*>(&entry), sizeof entry))
        {
            QB_LOG_ERROR("[Book::Open()] Read error at record {}.", i);
            m_entries.clear();
            return false;
        }

        entry.key = Be64(entry.key);
        entry.move = Be16(entry.move);
        entry.weight = Be16(entry.weight);
        entry.learn = Be32(entry.learn);

        m_entries[i] = entry;
    }

    QB_LOG_DEBUG("[Book::Open()] Successfully loaded {}.", t_fileName);

    return true;
}

//-------------------------------------------------
// Query helpers
//-------------------------------------------------

std::string qb::polyglot::Book::PolyMoveToUci(const uint16_t t_move)
{
    const auto to{ t_move & 0x3F };
    const auto from{ (t_move >> 6) & 0x3F };
    const auto prom{ (t_move >> 12) & 0x7 };

    auto move{ std::string(SQUARE_NAMES[from]).append(SQUARE_NAMES[to]) };
    if (prom)
    {
        static constexpr char PROM_CHAR[5]{ ' ', 'n', 'b', 'r', 'q' };
        move += PROM_CHAR[prom];
    }

    return move;
}

uint64_t qb::polyglot::Book::PolyglotHash(const Board* t_board)
{
    QB_ASSERT(t_board, "[Book::PolyglotHash()] Null pointer.")

    uint64_t key{ 0 };

    // piece
    for(auto sq{ 0 }; sq < 64; ++sq)
    {
        auto piece{ t_board->positionState.pieceBoard[sq] };
        if(piece.IsEmpty())
        {
            continue;
        }

        const auto pieceIdx{ PolyPieceIndex(piece.type, piece.color) };
        key ^= POLYGLOT_KEYS[pieceIdx * 64 + sq];
    }

    // castle
    const auto rights{ t_board->positionState.castlingRights };
    if (rights & WHITE_OO) key ^= POLYGLOT_KEYS[768];
    if (rights & WHITE_OOO) key ^= POLYGLOT_KEYS[769];
    if (rights & BLACK_OO) key ^= POLYGLOT_KEYS[770];
    if (rights & BLACK_OOO) key ^= POLYGLOT_KEYS[771];

    // en‑passant
    if(t_board->positionState.enPassantSquare != SQ_NONE)
    {
        const auto file{ file_of(t_board->positionState.enPassantSquare) };
        const auto rank{ rank_of(t_board->positionState.enPassantSquare) };

        if ((rank == RANK_3 && PawnOn(t_board, file, RANK_4, BLACK)) || // White was on the move
            (rank == RANK_6 && PawnOn(t_board, file, RANK_5, WHITE)))   // Black was on the move
        {
            key ^= POLYGLOT_KEYS[772 + file];
        }
    }

    // side
    if(t_board->positionState.side == WHITE) key ^= POLYGLOT_KEYS[780];

    return key;
}

std::pair<std::vector<qb::polyglot::Entry>::const_iterator, std::vector<qb::polyglot::Entry>::const_iterator> qb::
polyglot::Book::Probe(const uint64_t t_key) const
{
    QB_ASSERT(!m_entries.empty(), "[Book::Probe()] Missing book entries.")

    auto lo = std::lower_bound(
        m_entries.begin(), m_entries.end(),
        t_key,
        [](const Entry& t_e, const uint64_t t_k){ return t_e.key < t_k; }
    );

    if (lo == m_entries.end() || lo->key != t_key)
    {
        return { m_entries.end(), m_entries.end() };
    }

    auto hi = std::upper_bound(
        lo, m_entries.end(),
        t_key,
        [](const uint64_t t_k, const Entry& t_e){ return t_k < t_e.key; });

    return { lo, hi };
}

std::string qb::polyglot::Book::NormalizeCastlingToSquare(const std::string& t_moveStr)
{
    if (t_moveStr.length() < 4)
    {
        return "";
    }

    const std::string key{ t_moveStr.substr(0, 4) };
    if (const auto it{ castling_map.find(key) }; it != castling_map.end())
    {
        return it->second;
    }

    return t_moveStr.substr(2, 2);
}

//-------------------------------------------------
// Internal helper
//-------------------------------------------------

bool qb::polyglot::Book::PawnOn(const Board* t_board, const File t_file, const Rank t_rank, const Color t_color) noexcept
{
    QB_ASSERT(t_board, "[Book::PawnOn()] Null pointer.")

    const U64 bb{ (t_color == WHITE) ? t_board->bitboards.whitePawns : t_board->bitboards.blackPawns };
    return is_bit_set(bb, make_square(t_file, t_rank));
}
