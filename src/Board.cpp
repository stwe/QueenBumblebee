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

// ReSharper disable CppTooWideScopeInitStatement

#include <fmt/color.h>
#include "Board.h"
#include "Fen.h"
#include "Eval.h"
#include "MoveGenerator.h"
#include "ToString.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::Board::Board()
{
    QB_LOG_DEBUG("[Board::Board()] Create Board object.");

    InitLookupArray();
    [[maybe_unused]] static bool tablesReady = (init_tables(), true);
}

qb::Board::~Board() noexcept
{
    QB_LOG_DEBUG("[Board::~Board()] Destruct Board object.");
}

//-------------------------------------------------
// Init
//-------------------------------------------------

bool qb::Board::InitWithFen(const std::string& t_fenString)
{
    QB_LOG_DEBUG("[Board::InitWithFen()] Initialize FEN {}.", t_fenString);

    Fen fen(t_fenString);
    if (!fen.Parse())
    {
        QB_LOG_ERROR("[Board::InitWithFen()] Invalid FEN string passed.");
        return false;
    }

    auto r{ fen.GetPiecePlacement() };

    auto rank{ RANK_8 };
    for (const auto& pieces : fen.GetPiecePlacement())
    {
        if (!SetBitboards(pieces, rank))
        {
            QB_LOG_ERROR("[Board::InitWithFen()] Failed to parse piece positions.");
            return false;
        }

        --rank;
    }

    positionState.side = fen.GetSideToMove();
    positionState.castlingRights = fen.GetCastlingRights();
    positionState.enPassantSquare = fen.GetEnPassantSquare();

    UpdateCommonBitboards();

    return true;
}

bool qb::Board::InitWithBitboards(const Bitboards& t_bitboards, const PositionState& t_positionState)
{
    QB_LOG_DEBUG("[Board::InitWithBitboards()] Initialize with bitboards.");

    bitboards = t_bitboards;
    positionState = t_positionState;

    UpdateCommonBitboards();

    return true;
}

bool qb::Board::SetStartPosition()
{
    return InitWithFen(std::string(Fen::FEN_START));
}

//-------------------------------------------------
// Reset
//-------------------------------------------------

bool qb::Board::Reset()
{
    bitboards = {};
    positionState = {};
    m_moveHistory.clear();

    return SetStartPosition();
}

//-------------------------------------------------
// Move
//-------------------------------------------------

void qb::Board::MakeMove(const Move& t_move)
{
    MoveState moveState;
    moveState.enPassantSquare = positionState.enPassantSquare;
    moveState.side = positionState.side;
    moveState.castlingRights = positionState.castlingRights;
    moveState.move = t_move;
    moveState.zobristKey = positionState.zobristKey;
    moveState.halfmoveClock = positionState.halfmoveClock;
    moveState.fullmoveNumber = positionState.fullmoveNumber;

    const Color us{ positionState.side };
    const Color them{ opposite_color(us) };

    // 1. Remove old En-Passant from the Hash
    if (positionState.enPassantSquare != SQ_NONE)
    {
        positionState.zobristKey ^= m_zobristKeys.GetEnPassantKey(file_of(positionState.enPassantSquare));
    }

    // 2. Remove Castling Rights from the Hash
    positionState.zobristKey ^= m_zobristKeys.GetCastlingRightsKey(positionState.castlingRights);

    // 3. Remove the piece from the source square from the Hash
    positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(us, t_move.TypeOfPiece(), t_move.FromSquare());
    RemovePiece(t_move.FromSquare(), { t_move.TypeOfPiece(), us });

    // 4. If a piece is captured on the target square, remove it as well
    if (t_move.TypeOfCapturedPiece() != NO_PIECE_TYPE)
    {
        moveState.capturedPiece = Piece{ t_move.TypeOfCapturedPiece(), them };
        positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(them, t_move.TypeOfCapturedPiece(), t_move.ToSquare());
        RemovePiece(t_move.ToSquare(), { t_move.TypeOfCapturedPiece(), them });
    }

    // 5. Special case: En Passant capture
    if (t_move.TypeOfMove() == EN_PASSANT)
    {
        const U64 toBb{ square_bitboard(t_move.ToSquare()) };
        const U64 capturedBb{ us == WHITE ? (toBb >> 8) : (toBb << 8) };
        const Square capturedPawnSq{ get_lsb(capturedBb) };
        moveState.capturedPiece = Piece{ PAWN, them };
        positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(them, PAWN, capturedPawnSq);
        RemovePiece(capturedPawnSq, Piece{ PAWN, them });
    }

    // 6. Special case: Castling
    if (t_move.TypeOfMove() == CASTLING)
    {
        if (t_move.ToSquare() == SQ_G1)
        {
            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(WHITE, ROOK, SQ_H1);
            RemovePiece(SQ_H1, Piece{ ROOK, WHITE });

            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(WHITE, ROOK, SQ_F1);
            SetPiece(SQ_F1, Piece{ ROOK, WHITE });
        }
        else if (t_move.ToSquare() == SQ_C1)
        {
            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(WHITE, ROOK, SQ_A1);
            RemovePiece(SQ_A1, Piece{ ROOK, WHITE });

            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(WHITE, ROOK, SQ_D1);
            SetPiece(SQ_D1, Piece{ ROOK, WHITE });
        }
        else if (t_move.ToSquare() == SQ_G8)
        {
            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(BLACK, ROOK, SQ_H8);
            RemovePiece(SQ_H8, Piece{ ROOK, BLACK });

            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(BLACK, ROOK, SQ_F8);
            SetPiece(SQ_F8, Piece{ ROOK, BLACK });
        }
        else if (t_move.ToSquare() == SQ_C8)
        {
            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(BLACK, ROOK, SQ_A8);
            RemovePiece(SQ_A8, Piece{ ROOK, BLACK });

            positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(BLACK, ROOK, SQ_D8);
            SetPiece(SQ_D8, Piece{ ROOK, BLACK });
        }
    }

    // 7. Place the piece on the target square
    if (t_move.TypeOfMove() == PROMOTION)
    {
        SetPiece(t_move.ToSquare(), { t_move.TypeOfPromotionPiece(), us });
        positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(us, t_move.TypeOfPromotionPiece(), t_move.ToSquare());
    }
    else
    {
        SetPiece(t_move.ToSquare(), { t_move.TypeOfPiece(), us });
        positionState.zobristKey ^= m_zobristKeys.GetPieceSquareKey(us, t_move.TypeOfPiece(), t_move.ToSquare());
    }

    // 8. Update castling rights and set new castling hash
    UpdateCastlingRights(t_move);
    positionState.zobristKey ^= m_zobristKeys.GetCastlingRightsKey(positionState.castlingRights);

    // 9. Update en passant square and set new en passant hash
    if (t_move.TypeOfPiece() == PAWN && abs(t_move.ToSquare() - t_move.FromSquare()) == 16)
    {
        positionState.enPassantSquare = static_cast<Square>((t_move.FromSquare() + t_move.ToSquare()) / 2);
        positionState.zobristKey ^= m_zobristKeys.GetEnPassantKey(file_of(positionState.enPassantSquare));
    }
    else
    {
        positionState.enPassantSquare = SQ_NONE;
    }

    // 10. Update 50-move rule clock
    if (t_move.TypeOfPiece() == PAWN || t_move.TypeOfCapturedPiece() != NO_PIECE_TYPE)
    {
        positionState.halfmoveClock = 0;
    }
    else
    {
        ++positionState.halfmoveClock;
    }

    // 11. Advance fullmove number after Black
    if (positionState.side == BLACK)
    {
        ++positionState.fullmoveNumber;
    }

    // 12. Switch side (side to move)
    positionState.zobristKey ^= m_zobristKeys.GetSideToMoveKey();
    positionState.side = them;

    // 13. Store move
    m_moveHistory.push_back(moveState);
}

void qb::Board::UnmakeMove()
{
    if (m_moveHistory.empty())
    {
        QB_LOG_ERROR("[Board::UnmakeMove()] Called with empty move history.");
        return;
    }

    // Get last move
    const MoveState lastMove{ m_moveHistory.back() };
    m_moveHistory.pop_back();

    const Move move{ lastMove.move };
    const Color us{ opposite_color(positionState.side) };

    // 1. Switch back the side.
    positionState.side = us;

    // 2. Reset Zobrist Key (simply from the stored MoveState)
    positionState.zobristKey = lastMove.zobristKey;

    // 3. Restore castling rights and en-passant square
    positionState.castlingRights = lastMove.castlingRights;
    positionState.enPassantSquare = lastMove.enPassantSquare;

    // 4. Reset the piece (promotion / normal moves)
    if (move.TypeOfMove() == PROMOTION)
    {
        // Undo promotion: remove promoted piece
        RemovePiece(move.ToSquare(), Piece{ move.TypeOfPromotionPiece(), us });
        // Reset original pawn
        SetPiece(move.FromSquare(), Piece{ PAWN, us });
    }
    else
    {
        // Restore the piece
        RemovePiece(move.ToSquare(), Piece{ move.TypeOfPiece(), us });
        SetPiece(move.FromSquare(), Piece{ move.TypeOfPiece(), us });
    }

    // 5. Restore the captured piece if necessary
    if (!lastMove.capturedPiece.IsEmpty())
    {
        if (move.TypeOfMove() == EN_PASSANT)
        {
            // Restore en-passant captured pawn
            const U64 toBb{ square_bitboard(move.ToSquare()) };
            const U64 capturedBb{ us == WHITE ? (toBb >> 8) : (toBb << 8) };
            const Square capturedPawnSq{ get_lsb(capturedBb) };
            SetPiece(capturedPawnSq, lastMove.capturedPiece);
        }
        else
        {
            // Normal captures
            SetPiece(move.ToSquare(), lastMove.capturedPiece);
        }
    }

    // 6. Undo castling
    if (move.TypeOfMove() == CASTLING)
    {
        if (move.ToSquare() == SQ_G1)
        {
            RemovePiece(SQ_F1, Piece{ ROOK, WHITE });
            SetPiece(SQ_H1, Piece{ ROOK, WHITE });
        }
        else if (move.ToSquare() == SQ_C1)
        {
            RemovePiece(SQ_D1, Piece{ ROOK, WHITE });
            SetPiece(SQ_A1, Piece{ ROOK, WHITE });
        }
        else if (move.ToSquare() == SQ_G8)
        {
            RemovePiece(SQ_F8, Piece{ ROOK, BLACK });
            SetPiece(SQ_H8, Piece{ ROOK, BLACK });
        }
        else if (move.ToSquare() == SQ_C8)
        {
            RemovePiece(SQ_D8, Piece{ ROOK, BLACK });
            SetPiece(SQ_A8, Piece{ ROOK, BLACK });
        }
    }

    // 7. Restore halfmove and fullmove clocks
    positionState.halfmoveClock = lastMove.halfmoveClock;
    positionState.fullmoveNumber = lastMove.fullmoveNumber;
}

void qb::Board::SetPiece(const Square t_square, const Piece& t_piece)
{
    UpdatePieceAtSquare(t_square, t_piece, true);
}

void qb::Board::RemovePiece(const Square t_square, const Piece& t_piece)
{
    UpdatePieceAtSquare(t_square, t_piece, false);
}

void qb::Board::UpdateCastlingRights(const Move t_move)
{
    // King moves → remove all castling rights on this side
    if (t_move.TypeOfPiece() == KING)
    {
        if (positionState.side == WHITE)
        {
            positionState.castlingRights &= ~(WHITE_OO | WHITE_OOO);
        }
        else
        {
            positionState.castlingRights &= ~(BLACK_OO | BLACK_OOO);
        }
    }

    // Rook moves → remove corresponding castling rights
    if (t_move.TypeOfPiece() == ROOK)
    {
        if (positionState.side == WHITE)
        {
            if (t_move.FromSquare() == SQ_H1)
            {
                positionState.castlingRights &= ~WHITE_OO;
            }
            else if (t_move.FromSquare() == SQ_A1)
            {
                positionState.castlingRights &= ~WHITE_OOO;
            }
        }
        else
        {
            if (t_move.FromSquare() == SQ_H8)
            {
                positionState.castlingRights &= ~BLACK_OO;
            }
            else if (t_move.FromSquare() == SQ_A8)
            {
                positionState.castlingRights &= ~BLACK_OOO;
            }
        }
    }

    // Rook is captured → adjust castling rights accordingly
    if (t_move.TypeOfCapturedPiece() == ROOK)
    {

        if (const auto capturedColor{ positionState.side == WHITE ? BLACK : WHITE }; capturedColor == WHITE)
        {
            if (t_move.ToSquare() == SQ_H1)
            {
                positionState.castlingRights &= ~WHITE_OO;
            }
            else if (t_move.ToSquare() == SQ_A1)
            {
                positionState.castlingRights &= ~WHITE_OOO;
            }
        }
        else
        {
            if (t_move.ToSquare() == SQ_H8)
            {
                positionState.castlingRights &= ~BLACK_OO;
            }
            else if (t_move.ToSquare() == SQ_A8)
            {
                positionState.castlingRights &= ~BLACK_OOO;
            }
        }
    }
}

//-------------------------------------------------
// King Safety & Attack Detection
//-------------------------------------------------

bool qb::Board::IsKingInCheck(const Color t_bySide) const
{
    const Square kingSq = t_bySide == WHITE
        ? get_lsb(bitboards.whiteKing)
        : get_lsb(bitboards.blackKing);

    return IsSquareAttacked(opposite_color(t_bySide), kingSq);
}

bool qb::Board::IsSquareAttacked(const Color t_bySide, const Square t_targetSq) const
{
    const U64 targetBb{ square_bitboard(t_targetSq) };
    const U64 occupied{ bitboards.allPieces };

    const U64 pawnsBb = t_bySide == WHITE
        ? bitboards.whitePawns
        : bitboards.blackPawns;
    for (const Square fromSq : bitscan_all(pawnsBb))
    {
        if (MoveGenerator::PAWN_ATTACKS_BB[t_bySide][fromSq] & targetBb)
        {
            return true;
        }
    }

    const U64 knightsBb = t_bySide == WHITE
        ? bitboards.whiteKnights
        : bitboards.blackKnights;
    for (const Square fromSq : bitscan_all(knightsBb))
    {
        if (MoveGenerator::KNIGHT_ATTACKS_BB[fromSq] & targetBb)
        {
            return true;
        }
    }

    const U64 kingBb = t_bySide == WHITE
        ? bitboards.whiteKing
        : bitboards.blackKing;
    for (const Square fromSq : bitscan_all(kingBb))
    {
        if (MoveGenerator::KING_ATTACKS_BB[fromSq] & targetBb)
        {
            return true;
        }
    }

    // Bishops and Queens (diagonal sliders)
    const U64 bishopsBb = t_bySide == WHITE
        ? bitboards.whiteBishops
        : bitboards.blackBishops;
    const U64 queensBb = t_bySide == WHITE
        ? bitboards.whiteQueens
        : bitboards.blackQueens;
    const U64 diagSliders{ bishopsBb | queensBb };
    for (const Square fromSq : bitscan_all(diagSliders))
    {
        const U64 diagMask = DIAGONAL_MASKS[diagonal_index(fromSq)];
        const U64 adiagMask = ANTIDIAGONAL_MASKS[antidiagonal_index(fromSq)];

        const U64 diagBlockers = occupied & diagMask;
        const U64 adiagBlockers = occupied & adiagMask;

        if (MoveGenerator::SlidingAttacks(diagBlockers, square_bitboard(fromSq), diagMask) & targetBb)
        {
            return true;
        }

        if (MoveGenerator::SlidingAttacks(adiagBlockers, square_bitboard(fromSq), adiagMask) & targetBb)
        {
            return true;
        }
    }

    // Rooks and Queens (rank/file sliders)
    const U64 rooksBb = t_bySide == WHITE
        ? bitboards.whiteRooks
        : bitboards.blackRooks;
    const U64 rookSliders{ rooksBb | queensBb };
    for (const Square fromSq : bitscan_all(rookSliders)) // NOLINT(*-use-anyofallof)
    {
        const U64 fileMask = FILE_MASKS[file_of(fromSq)];
        const U64 rankMask = RANK_MASKS[rank_of(fromSq)];

        const U64 fileBlockers = occupied & fileMask;
        const U64 rankBlockers = occupied & rankMask;

        if (MoveGenerator::SlidingAttacks(fileBlockers, square_bitboard(fromSq), fileMask) & targetBb)
        {
            return true;
        }

        if (MoveGenerator::SlidingAttacks(rankBlockers, square_bitboard(fromSq), rankMask) & targetBb)
        {
            return true;
        }
    }

    return false;
}

//-------------------------------------------------
// Castling Rights
//-------------------------------------------------

bool qb::Board::CanCastle(
    const Square t_kingSq,
    const Square t_transit1,
    const Square t_transit2,
    const Color t_bySide
) const
{
    const Color enemyColor{ opposite_color(t_bySide) };
    if (IsSquareAttacked(enemyColor, t_kingSq) ||
        IsSquareAttacked(enemyColor, t_transit1) ||
        IsSquareAttacked(enemyColor, t_transit2)
    )
    {
        return false;
    }

    return true;
}

//-------------------------------------------------
// Print
//-------------------------------------------------

void qb::Board::ColorPrintBoard() const
{
    fmt::print(fg(fmt::color::green), "\n          Queen Bumblebee\n\n");
    fmt::print(fg(fmt::color::green), " +---+---+---+---+---+---+---+---+\n");

    for (int rank{ RANK_8 }; rank >= RANK_1; --rank)
    {
        fmt::print(fg(fmt::color::green), "{}|", rank + 1);

        for (int file{ FILE_A }; file <= FILE_H; ++file)
        {
            const auto piece{ get_piece_string(static_cast<File>(file), static_cast<Rank>(rank), bitboards) };
            if (piece.second == WHITE)
            {
                fmt::print(fg(fmt::color::white), "{}", piece.first);
            }
            else
            {
                fmt::print(fg(fmt::color::yellow), "{}", piece.first);
            }
            fmt::print(fg(fmt::color::green), "|");
        }

        fmt::print("\n");
        fmt::print(fg(fmt::color::green), " +---+---+---+---+---+---+---+---+\n");
    }

    fmt::print(fg(fmt::color::green), "   A   B   C   D   E   F   G   H\n\n");

    fmt::print(fg(fmt::color::green), "{}: {}\n", "Side to move", (positionState.side == WHITE ? "White" : "Black"));
    fmt::print(fg(fmt::color::green), "{}: {}\n", "En Passant target square", positionState.enPassantSquare != SQ_NONE ? SQUARE_NAMES[positionState.enPassantSquare] : "-");
    fmt::print(fg(fmt::color::green), "{}: {}\n\n", "Castling rights", GetCastlingRightsString());
    fmt::print(fg(fmt::color::green), "{}: {}\n", "Halfmove clock", positionState.halfmoveClock);
    fmt::print(fg(fmt::color::green), "{}: {}\n\n", "Fullmove number", positionState.fullmoveNumber);
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void qb::Board::InitLookupArray()
{
    m_pieceBitboards[WHITE][PAWN] = &bitboards.whitePawns;
    m_pieceBitboards[WHITE][ROOK] = &bitboards.whiteRooks;
    m_pieceBitboards[WHITE][KNIGHT] = &bitboards.whiteKnights;
    m_pieceBitboards[WHITE][BISHOP] = &bitboards.whiteBishops;
    m_pieceBitboards[WHITE][QUEEN] = &bitboards.whiteQueens;
    m_pieceBitboards[WHITE][KING] = &bitboards.whiteKing;

    m_pieceBitboards[BLACK][PAWN] = &bitboards.blackPawns;
    m_pieceBitboards[BLACK][ROOK] = &bitboards.blackRooks;
    m_pieceBitboards[BLACK][KNIGHT] = &bitboards.blackKnights;
    m_pieceBitboards[BLACK][BISHOP] = &bitboards.blackBishops;
    m_pieceBitboards[BLACK][QUEEN] = &bitboards.blackQueens;
    m_pieceBitboards[BLACK][KING] = &bitboards.blackKing;
}

bool qb::Board::SetBitboards(const std::string& t_pieces, const Rank t_rank)
{
    auto file{ 0 };

    for (const auto piece : t_pieces)
    {
        if (std::isdigit(piece))
        {
            file += piece - '0';
            continue;
        }

        if (file >= 8)
        {
            QB_LOG_ERROR("[Board::SetBitboards()] Too many pieces on rank.");
            return false;
        }

        const Color color{ std::isupper(piece) ? WHITE : BLACK };
        const auto pieceType{ piece_type_from_char(piece) };

        const auto square{ make_square(static_cast<File>(file), t_rank) };
        if (auto bb{ GetBitboard({ pieceType, color }) }; bb.has_value())
        {
            set_bit(*bb, square);
            positionState.pieceBoard[square] = Piece{ pieceType, color };
            file += 1;
        }
        else
        {
            QB_LOG_ERROR("[Board::SetBitboards()] Invalid piece type.");
            return false;
        }
    }

    return true;
}

void qb::Board::UpdatePieceAtSquare(const Square t_square, const Piece& t_piece, const bool t_set)
{
    if (t_set)
    {
        set_bit(*m_pieceBitboards[t_piece.color][t_piece.type], t_square);
        positionState.pieceBoard[t_square] = t_piece;
    }
    else
    {
        clear_bit(*m_pieceBitboards[t_piece.color][t_piece.type], t_square);
        positionState.pieceBoard[t_square] = {};
    }

    UpdateCommonBitboards();
}

void qb::Board::UpdateCommonBitboards()
{
    bitboards.whitePieces = bitboards.whitePawns |
        bitboards.whiteKnights |
        bitboards.whiteBishops |
        bitboards.whiteRooks |
        bitboards.whiteQueens |
        bitboards.whiteKing;

    bitboards.blackPieces = bitboards.blackPawns |
        bitboards.blackKnights |
        bitboards.blackBishops |
        bitboards.blackRooks |
        bitboards.blackQueens |
        bitboards.blackKing;

    bitboards.allPieces = bitboards.whitePieces | bitboards.blackPieces;

    bitboards.noPieces = ~bitboards.allPieces;
}

std::optional<std::reference_wrapper<U64>> qb::Board::GetBitboard(const Piece t_piece)
{
    switch (t_piece.type)
    {
    case PAWN:
        return std::ref(t_piece.color == WHITE ? bitboards.whitePawns : bitboards.blackPawns);
    case KNIGHT:
        return std::ref(t_piece.color == WHITE ? bitboards.whiteKnights : bitboards.blackKnights);
    case BISHOP:
        return std::ref(t_piece.color == WHITE ? bitboards.whiteBishops : bitboards.blackBishops);
    case ROOK:
        return std::ref(t_piece.color == WHITE ? bitboards.whiteRooks : bitboards.blackRooks);
    case QUEEN:
        return std::ref(t_piece.color == WHITE ? bitboards.whiteQueens : bitboards.blackQueens);
    case KING:
        return std::ref(t_piece.color == WHITE ? bitboards.whiteKing : bitboards.blackKing);
    default:
        QB_LOG_ERROR("[Board::GetBitboard()] Invalid piece type");
        break;
    }

    return std::nullopt;
}

std::string qb::Board::GetCastlingRightsString() const
{
    if (positionState.castlingRights == NO_CASTLING)
    {
        return "-";
    }

    std::string cr{};
    if (positionState.castlingRights & WHITE_OO)
    {
        cr.append("K");
    }
    if (positionState.castlingRights & WHITE_OOO)
    {
        cr.append("Q");
    }
    if (positionState.castlingRights & BLACK_OO)
    {
        cr.append("k");
    }
    if (positionState.castlingRights & BLACK_OOO)
    {
        cr.append("q");
    }

    return cr;
}
