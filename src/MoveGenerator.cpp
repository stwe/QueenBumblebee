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

// ReSharper disable once CppTooWideScopeInitStatement
// ReSharper disable CppTooWideScopeInitStatement

#include <fmt/color.h>
#include "MoveGenerator.h"
#include "Board.h"
#include "ToString.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

qb::MoveGenerator::MoveGenerator(Board* t_board)
    : m_board{ t_board }
{
    QB_ASSERT(m_board, "[MoveGenerator::MoveGenerator()] Null pointer.")

    QB_LOG_DEBUG("[MoveGenerator::MoveGenerator()] Create MoveGenerator object.");
}

qb::MoveGenerator::~MoveGenerator() noexcept
{
    QB_LOG_DEBUG("[MoveGenerator::~MoveGenerator()] Destruct MoveGenerator object.");
}

//-------------------------------------------------
// Move Generation
//-------------------------------------------------

std::vector<qb::Move> qb::MoveGenerator::GeneratePseudoLegalMoves() const
{
    std::vector<Move> moves;

    GeneratePawnMoves(moves, m_board->positionState.side);
    GenerateKnightMoves(moves, m_board->positionState.side);
    GenerateKingMoves(moves, m_board->positionState.side);
    if (m_board->positionState.castlingRights != NO_CASTLING)
    {
        GenerateCastlingMoves(moves, m_board->positionState.side);
    }

    GenerateRookMoves(moves, m_board->positionState.side);
    GenerateBishopMoves(moves, m_board->positionState.side);
    GenerateQueenMoves(moves, m_board->positionState.side);

    return moves;
}

std::vector<qb::Move> qb::MoveGenerator::GenerateLegalMoves() const
{
    const auto pseudoMoves{ GeneratePseudoLegalMoves() };
    std::vector<Move> legalMoves;

    for (const auto& move : pseudoMoves)
    {
        const Color us{ m_board->positionState.side };

        m_board->MakeMove(move);

        if (!m_board->IsKingInCheck(us))
        {
            legalMoves.push_back(move);
        }

        m_board->UnmakeMove();
    }

    return legalMoves;
}

U64 qb::MoveGenerator::GenerateAttackedSquares(const Color t_bySide) const
{
    U64 attackedBb{ 0 };
    const U64 occupied{ m_board->bitboards.allPieces };

    const U64 pawnsBb = t_bySide == WHITE
        ? m_board->bitboards.whitePawns
        : m_board->bitboards.blackPawns;
    for (const Square fromSq : bitscan_all(pawnsBb))
    {
        attackedBb |= PAWN_ATTACKS_BB[t_bySide][fromSq];
    }

    const U64 knightsBb = t_bySide == WHITE
        ? m_board->bitboards.whiteKnights
        : m_board->bitboards.blackKnights;
    for (const Square fromSq : bitscan_all(knightsBb))
    {
        attackedBb |= KNIGHT_ATTACKS_BB[fromSq];
    }

    const U64 kingBb = t_bySide == WHITE
        ? m_board->bitboards.whiteKing
        : m_board->bitboards.blackKing;
    for (const Square fromSq : bitscan_all(kingBb))
    {
        attackedBb |= KING_ATTACKS_BB[fromSq];
    }

    // Bishop and Queen (diagonals)
    const U64 bishopsBb = t_bySide == WHITE
        ? m_board->bitboards.whiteBishops
        : m_board->bitboards.blackBishops;
    const U64 queensBb = t_bySide == WHITE
        ? m_board->bitboards.whiteQueens
        : m_board->bitboards.blackQueens;
    const U64 diagSliders{ bishopsBb | queensBb };
    for (const Square fromSq : bitscan_all(diagSliders))
    {
        const U64 diagMask{ DIAGONAL_MASKS[diagonal_index(fromSq)] };
        const U64 adiagMask{ ANTIDIAGONAL_MASKS[antidiagonal_index(fromSq)] };

        attackedBb |= SlidingAttacks(occupied & diagMask, SQUARE_BITBOARDS[fromSq], diagMask);
        attackedBb |= SlidingAttacks(occupied & adiagMask, SQUARE_BITBOARDS[fromSq], adiagMask);
    }

    // Rook and Queen (ranks and files)
    const U64 rooksBb = t_bySide == WHITE
        ? m_board->bitboards.whiteRooks
        : m_board->bitboards.blackRooks;
    const U64 rookSliders{ rooksBb | queensBb };
    for (const Square fromSq : bitscan_all(rookSliders))
    {
        const U64 fileMask{ FILE_MASKS[file_of(fromSq)] };
        const U64 rankMask{ RANK_MASKS[rank_of(fromSq)] };

        attackedBb |= SlidingAttacks(occupied & fileMask, SQUARE_BITBOARDS[fromSq], fileMask);
        attackedBb |= SlidingAttacks(occupied & rankMask, SQUARE_BITBOARDS[fromSq], rankMask);
    }

    return attackedBb;
}

//-------------------------------------------------
// Debug / Output
//-------------------------------------------------

void qb::MoveGenerator::ColorPrintMoves(const std::vector<Move>& t_moves) const
{
    fmt::print(fg(fmt::color::green), "+-----------------------------------------------------------------+\n");
    fmt::print(fg(fmt::color::green), "| Number of moves: {:<47}|\n", t_moves.size());
    fmt::print(fg(fmt::color::green), "+-----------------------------------------------------------------+\n");
    fmt::print(fg(fmt::color::green), "| Piece   From  To    Type        Promotion   Capture             |\n");
    fmt::print(fg(fmt::color::green), "+-----------------------------------------------------------------+\n");

    for (const auto& move : t_moves)
    {
        auto moveColor{ fmt::color::white };

        switch (move.TypeOfMove())
        {
        case NORMAL:
            moveColor = (move.TypeOfCapturedPiece() != NO_PIECE_TYPE) ? fmt::color::red : fmt::color::white;
            break;
        case PROMOTION:
            moveColor = fmt::color::yellow;
            break;
        case EN_PASSANT:
            moveColor = fmt::color::magenta;
            break;
        case CASTLING:
            moveColor = fmt::color::cyan;
            break;
        }

        const auto pieceTypeStr = create_short_piece_string(move.TypeOfPiece(), m_board->positionState.side);
        const auto fromStr = SQUARE_NAMES[move.FromSquare()];
        const auto toStr = SQUARE_NAMES[move.ToSquare()];
        const auto moveTypeStr = MOVE_TYPE_NAMES[move.TypeOfMove()];
        const auto promotionStr = (move.TypeOfPromotionPiece() != NO_PIECE_TYPE)
            ? create_short_piece_string(move.TypeOfPromotionPiece(), m_board->positionState.side)
            : "-";
        const auto capturedStr = (move.TypeOfCapturedPiece() != NO_PIECE_TYPE)
            ? create_short_piece_string(move.TypeOfCapturedPiece(), m_board->positionState.side)
            : "-";

        fmt::print(fg(moveColor), "| {:<7} {:<5} {:<5} {:<11} {:<11} {:<20}|\n",
            pieceTypeStr, fromStr, toStr, moveTypeStr, promotionStr, capturedStr);
    }

    fmt::print(fg(fmt::color::green), "+-----------------------------------------------------------------+\n");
}

//-------------------------------------------------
// Attack Utilities
//-------------------------------------------------

U64 qb::MoveGenerator::SlidingAttacks(const U64 t_occupied, const U64 t_rook, const U64 t_lineMask)
{
    const U64 forward = t_occupied - 2 * t_rook;
    const U64 reverse = reverse_bits(reverse_bits(t_occupied) - 2 * reverse_bits(t_rook));
    const U64 attacks = (forward ^ reverse) & t_lineMask;

    return attacks;
}

int qb::MoveGenerator::CountAttackers(const Square t_square, const Color t_attackerSide) const
{
    auto attackers{ 0 };
    const bool isWhiteAttacker{ t_attackerSide == WHITE };
    const U64 allPiecesBb{ m_board->bitboards.allPieces };

    // Pawn
    const U64 attackerPawnsBb{ isWhiteAttacker ? m_board->bitboards.whitePawns : m_board->bitboards.blackPawns };
    attackers += popcount(PAWN_ATTACKS_BB[isWhiteAttacker ? BLACK : WHITE][t_square] & attackerPawnsBb);

    // King
    const U64 attackerKingBb{ isWhiteAttacker ? m_board->bitboards.whiteKing : m_board->bitboards.blackKing };
    attackers += popcount(KING_ATTACKS_BB[t_square] & attackerKingBb);

    // Knight
    const U64 attackerKnightsBb{ isWhiteAttacker ? m_board->bitboards.whiteKnights : m_board->bitboards.blackKnights };
    attackers += popcount(KNIGHT_ATTACKS_BB[t_square] & attackerKnightsBb);

    // Bishop & Queen (diagonals)
    const U64 bishopsBb{ isWhiteAttacker ? m_board->bitboards.whiteBishops : m_board->bitboards.blackBishops };
    const U64 queensBb{ isWhiteAttacker ? m_board->bitboards.whiteQueens : m_board->bitboards.blackQueens };

    for (const Square fromSq : bitscan_all(bishopsBb | queensBb))
    {
        const U64 diagMaskBb{ DIAGONAL_MASKS[diagonal_index(fromSq)] };
        const U64 antiDiagMaskBb{ ANTIDIAGONAL_MASKS[antidiagonal_index(fromSq)] };

        U64 attacksBb{ SlidingAttacks(allPiecesBb & diagMaskBb, SQUARE_BITBOARDS[fromSq], diagMaskBb) };
        attacksBb |= SlidingAttacks(allPiecesBb & antiDiagMaskBb, SQUARE_BITBOARDS[fromSq], antiDiagMaskBb);

        if (attacksBb & SQUARE_BITBOARDS[t_square])
        {
            attackers++;
        }
    }

    // Rook & Queen (ranks/files)
    const U64 rooksBb{ isWhiteAttacker ? m_board->bitboards.whiteRooks : m_board->bitboards.blackRooks };

    for (const Square fromSq : bitscan_all(rooksBb | queensBb))
    {
        const U64 rankMaskBb{ RANK_MASKS[rank_of(fromSq)] };
        const U64 fileMaskBb{ FILE_MASKS[file_of(fromSq)] };

        U64 attacksBb{ SlidingAttacks(allPiecesBb & rankMaskBb, SQUARE_BITBOARDS[fromSq], rankMaskBb) };
        attacksBb |= SlidingAttacks(allPiecesBb & fileMaskBb, SQUARE_BITBOARDS[fromSq], fileMaskBb);

        if (attacksBb & SQUARE_BITBOARDS[t_square])
        {
            attackers++;
        }
    }

    return attackers;
}

//-------------------------------------------------
// Nonsliding Pieces - Generators
//-------------------------------------------------

void qb::MoveGenerator::GeneratePawnMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto isWhite{ t_bySide == WHITE };
    const U64 pawnsBb{ isWhite ? m_board->bitboards.whitePawns : m_board->bitboards.blackPawns };
    const U64 enemyPiecesBb{ isWhite ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };
    const U64 promotionRankBb{ isWhite ? MASK_RANK_8 : MASK_RANK_1 };
    const U64 doublePushStartingRankBb{ isWhite ? MASK_RANK_3 : MASK_RANK_6 };

    const auto shiftPawn = [isWhite](const U64 t_bitboard) -> U64
    {
        return isWhite ? t_bitboard << 8 : t_bitboard >> 8;
    };

    for (Square fromSq : bitscan_all(pawnsBb))
    {
        const U64 fromBb{ SQUARE_BITBOARDS[fromSq] };

        // quiet push (non-promotion)
        // quiet double push
        // quiet promotion

        // single push
        const U64 oneStepBb{ shiftPawn(fromBb) & m_board->bitboards.noPieces };

        // double push (only from the starting rank)
        const U64 twoStepsBb{ (shiftPawn(oneStepBb & doublePushStartingRankBb) & m_board->bitboards.noPieces) };

        // quiet non-promotion (all normal single-step moves without promotion, example: e2->e3)
        const U64 quietBb{ oneStepBb & ~promotionRankBb };

        // quiet promotion (quiet move that results in a promotion without capture, example: e7->e8)
        const U64 promotionQuietBb{ oneStepBb & promotionRankBb };

        // add quiet moves
        for (Square toSq : bitscan_all(quietBb))
        {
            t_moves.emplace_back(fromSq, toSq, PAWN);
        }

        // add double pushs
        for (Square toSq : bitscan_all(twoStepsBb))
        {
            t_moves.emplace_back(fromSq, toSq, PAWN);
        }

        // add quiet promotions (no capture)
        for (Square toSq : bitscan_all(promotionQuietBb))
        {
            t_moves.emplace_back(fromSq, toSq, PAWN, QUEEN, PROMOTION);
            t_moves.emplace_back(fromSq, toSq, PAWN, ROOK, PROMOTION);
            t_moves.emplace_back(fromSq, toSq, PAWN, BISHOP, PROMOTION);
            t_moves.emplace_back(fromSq, toSq, PAWN, KNIGHT, PROMOTION);
        }

        // attack left
        // attack right
        // promotion attack

        // pawn attacks (diagonal left and right)
        const U64 attacksBb{ PAWN_ATTACKS_BB[t_bySide][fromSq] };

        // captures
        const U64 capturesBb{ attacksBb & enemyPiecesBb };

        // normal captures
        const U64 normalCapturesBb{ capturesBb & ~promotionRankBb };

        // promotion captures (capturing moves that result in a promotion)
        const U64 promotionCapturesBb{ capturesBb & promotionRankBb };

        // add normal captures
        for (Square toSq : bitscan_all(normalCapturesBb))
        {
            const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
            t_moves.emplace_back(fromSq, toSq, PAWN, NO_PIECE_TYPE, NORMAL, capPieceType);
        }

        // add promotion captures
        for (Square toSq : bitscan_all(promotionCapturesBb))
        {
            const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
            t_moves.emplace_back(fromSq, toSq, PAWN, QUEEN, PROMOTION, capPieceType);
            t_moves.emplace_back(fromSq, toSq, PAWN, ROOK, PROMOTION, capPieceType);
            t_moves.emplace_back(fromSq, toSq, PAWN, BISHOP, PROMOTION, capPieceType);
            t_moves.emplace_back(fromSq, toSq, PAWN, KNIGHT, PROMOTION, capPieceType);
        }

        // en passant
        const Square enPassantSq{ m_board->positionState.enPassantSquare };
        if (enPassantSq != SQ_NONE)
        {
            if (attacksBb & SQUARE_BITBOARDS[enPassantSq])
            {
                t_moves.emplace_back(fromSq, enPassantSq, PAWN, NO_PIECE_TYPE, EN_PASSANT, PAWN);
            }
        }
    }
}

void qb::MoveGenerator::GenerateKnightMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto whiteToMove{ t_bySide == WHITE };
    const U64 knightsBb{ whiteToMove ? m_board->bitboards.whiteKnights : m_board->bitboards.blackKnights };
    const U64 enemyPiecesBb{ whiteToMove ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };

    for (Square fromSq : bitscan_all(knightsBb))
    {
        // get all potential knight moves
        const U64 knightAttackBb{ KNIGHT_ATTACKS_BB[fromSq] };

        // quiet moves
        const U64 quietMovesBb{ knightAttackBb & m_board->bitboards.noPieces };
        for (Square toSq : bitscan_all(quietMovesBb))
        {
            t_moves.emplace_back(fromSq, toSq, KNIGHT);
        }

        // captures
        const U64 captureMovesBb{ knightAttackBb & enemyPiecesBb };
        for (Square toSq : bitscan_all(captureMovesBb))
        {
            const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
            t_moves.emplace_back(fromSq, toSq, KNIGHT, NO_PIECE_TYPE, NORMAL, capPieceType);
        }
    }
}

void qb::MoveGenerator::GenerateKingMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto whiteToMove{ t_bySide == WHITE };
    U64 kingBb{ whiteToMove ? m_board->bitboards.whiteKing : m_board->bitboards.blackKing };
    const U64 ownPiecesBb{ whiteToMove ? m_board->bitboards.whitePieces : m_board->bitboards.blackPieces };
    const U64 enemyPiecesBb{ whiteToMove ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };
    const Color enemyColor{ opposite_color(t_bySide) };

    // get the king position
    const Square kingSq{ pop_lsb(kingBb) };

    // get all potential king moves
    const U64 kingAttackBb{ KING_ATTACKS_BB[kingSq] };

    // get all enemy-attacked squares
    const U64 enemyAttacksBb{ GenerateAttackedSquares(enemyColor) };

    // filter out all squares the king cannot step on (attacked or own)
    const U64 legalKingMovesBb{ kingAttackBb & ~ownPiecesBb & ~enemyAttacksBb };

    // quiet moves: legal king moves to empty squares
    const U64 quietMovesBb{ legalKingMovesBb & ~enemyPiecesBb };
    for (const Square toSq : bitscan_all(quietMovesBb))
    {
        t_moves.emplace_back(kingSq, toSq, KING);
    }

    // captures
    const U64 captureMovesBb{ legalKingMovesBb & enemyPiecesBb };
    for (const Square toSq : bitscan_all(captureMovesBb))
    {
        const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
        t_moves.emplace_back(kingSq, toSq, KING, NO_PIECE_TYPE, NORMAL, capPieceType);
    }
}

//-------------------------------------------------
// Castling Generator
//-------------------------------------------------

void qb::MoveGenerator::GenerateCastlingMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const CastlingRights castlingRights{ m_board->positionState.castlingRights };

    if (t_bySide == WHITE)
    {
        // White King Side
        if (castlingRights & WHITE_OO && !(WHITE_CASTLING_MASK_OO & m_board->bitboards.allPieces))
        {
            if (m_board->CanCastle(SQ_E1, SQ_F1, SQ_G1, WHITE))
            {
                t_moves.emplace_back(WHITE_CASTLING_MOVE_OO);
            }
        }

        // White Queen Side
        if (castlingRights & WHITE_OOO && !(WHITE_CASTLING_MASK_OOO & m_board->bitboards.allPieces))
        {
            if (m_board->CanCastle(SQ_E1, SQ_D1, SQ_C1, WHITE))
            {
                t_moves.emplace_back(WHITE_CASTLING_MOVE_OOO);
            }
        }
    }
    else
    {
        // Black King Side
        if (castlingRights & BLACK_OO && !(BLACK_CASTLING_MASK_OO & m_board->bitboards.allPieces))
        {
            if (m_board->CanCastle(SQ_E8, SQ_F8, SQ_G8, BLACK))
            {
                t_moves.emplace_back(BLACK_CASTLING_MOVE_OO);
            }
        }

        // Black Queen Side
        if (castlingRights & BLACK_OOO && !(BLACK_CASTLING_MASK_OOO & m_board->bitboards.allPieces))
        {
            if (m_board->CanCastle(SQ_E8, SQ_D8, SQ_C8, BLACK))
            {
                t_moves.emplace_back(BLACK_CASTLING_MOVE_OOO);
            }
        }
    }
}

//-------------------------------------------------
// Sliding Pieces - Generators
//-------------------------------------------------

// todo: renames Bb
void qb::MoveGenerator::GenerateRookMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto whiteToMove{ t_bySide == WHITE };
    const U64 rooksBb{ whiteToMove ? m_board->bitboards.whiteRooks : m_board->bitboards.blackRooks };
    const U64 ownPiecesBb{ whiteToMove ? m_board->bitboards.whitePieces : m_board->bitboards.blackPieces };
    const U64 enemyPiecesBb{ whiteToMove ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };
    const U64 occupied{ m_board->bitboards.allPieces };

    for (const Square fromSq : bitscan_all(rooksBb))
    {
        const U64 rookBb{ SQUARE_BITBOARDS[fromSq] };

        const U64 rankMask{ RANK_MASKS[rank_of(fromSq)] };
        const U64 rankAttacks{ SlidingAttacks(occupied & rankMask, rookBb, rankMask) };

        const U64 fileMask{ FILE_MASKS[file_of(fromSq)] };
        const U64 fileAttacks{ SlidingAttacks(occupied & fileMask, rookBb, fileMask) };

        const U64 attacks{ (rankAttacks | fileAttacks) & ~ownPiecesBb };
        for (const Square toSq : bitscan_all(attacks))
        {
            if (enemyPiecesBb & SQUARE_BITBOARDS[toSq])
            {
                const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
                t_moves.emplace_back(fromSq, toSq, ROOK, NO_PIECE_TYPE, NORMAL, capPieceType);
            }
            else
            {
                t_moves.emplace_back(fromSq, toSq, ROOK);
            }
        }
    }
}

void qb::MoveGenerator::GenerateBishopMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto whiteToMove{ t_bySide == WHITE };
    const U64 bishopsBb{ whiteToMove ? m_board->bitboards.whiteBishops : m_board->bitboards.blackBishops };
    const U64 ownPiecesBb{ whiteToMove ? m_board->bitboards.whitePieces : m_board->bitboards.blackPieces };
    const U64 enemyPiecesBb{ whiteToMove ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };
    const U64 occupied{ m_board->bitboards.allPieces };

    for (const Square fromSq : bitscan_all(bishopsBb))
    {
        const U64 bishopBb{ SQUARE_BITBOARDS[fromSq] };

        const U64 diagMask{ DIAGONAL_MASKS[diagonal_index(fromSq)] };
        const U64 diagAttacks{ SlidingAttacks(occupied & diagMask, bishopBb, diagMask) };

        const U64 antiDiagMask{ ANTIDIAGONAL_MASKS[antidiagonal_index(fromSq)] };
        const U64 antiDiagAttacks{ SlidingAttacks(occupied & antiDiagMask, bishopBb, antiDiagMask) };

        const U64 attacks{ (diagAttacks | antiDiagAttacks) & ~ownPiecesBb };
        for (const Square toSq : bitscan_all(attacks))
        {
            if (enemyPiecesBb & SQUARE_BITBOARDS[toSq])
            {
                const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
                t_moves.emplace_back(fromSq, toSq, BISHOP, NO_PIECE_TYPE, NORMAL, capPieceType);
            }
            else
            {
                t_moves.emplace_back(fromSq, toSq, BISHOP);
            }
        }
    }
}

void qb::MoveGenerator::GenerateQueenMoves(std::vector<Move>& t_moves, const Color t_bySide) const
{
    const auto whiteToMove{ t_bySide == WHITE };
    const U64 queensBb{ whiteToMove ? m_board->bitboards.whiteQueens : m_board->bitboards.blackQueens };
    const U64 ownPiecesBb{ whiteToMove ? m_board->bitboards.whitePieces : m_board->bitboards.blackPieces };
    const U64 enemyPiecesBb{ whiteToMove ? m_board->bitboards.blackPieces : m_board->bitboards.whitePieces };
    const U64 occupied{ m_board->bitboards.allPieces };

    for (const Square fromSq : bitscan_all(queensBb))
    {
        const U64 queenBb{ SQUARE_BITBOARDS[fromSq] };

        const U64 diagMask{ DIAGONAL_MASKS[diagonal_index(fromSq)] };
        const U64 diagAttacks{ SlidingAttacks(occupied & diagMask, queenBb, diagMask) };

        const U64 antiDiagMask{ ANTIDIAGONAL_MASKS[antidiagonal_index(fromSq)] };
        const U64 antiDiagAttacks{ SlidingAttacks(occupied & antiDiagMask, queenBb, antiDiagMask) };

        const U64 rankMask{ RANK_MASKS[rank_of(fromSq)] };
        const U64 rankAttacks{ SlidingAttacks(occupied & rankMask, queenBb, rankMask) };

        const U64 fileMask{ FILE_MASKS[file_of(fromSq)] };
        const U64 fileAttacks{ SlidingAttacks(occupied & fileMask, queenBb, fileMask) };

        const U64 attacks{ (rankAttacks | fileAttacks | diagAttacks | antiDiagAttacks) & ~ownPiecesBb };
        for (const Square toSq : bitscan_all(attacks))
        {
            if (enemyPiecesBb & SQUARE_BITBOARDS[toSq])
            {
                const auto [capPieceType, _]{ m_board->positionState.pieceBoard[toSq] };
                t_moves.emplace_back(fromSq, toSq, QUEEN, NO_PIECE_TYPE, NORMAL, capPieceType);
            }
            else
            {
                t_moves.emplace_back(fromSq, toSq, QUEEN);
            }
        }
    }
}
