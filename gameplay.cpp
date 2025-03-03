#include "chess.h"

// ========== Generate Pawn Moves ==========
uint64_t getPawnMoves(uint64_t pawns, uint64_t emptySquares, bool isWhite) {
    return isWhite ? (pawns << 8) & emptySquares : (pawns >> 8) & emptySquares;
}

// ========== Generate Knight Moves ==========
uint64_t getKnightMoves(uint64_t knights) {
    uint64_t moves = 0;
    uint64_t temp;

    // Knight movement bitmasks
    temp = (knights << 15) & 0xFEFEFEFEFEFEFEFE;
    moves |= temp;
    temp = (knights << 17) & 0x7F7F7F7F7F7F7F7F;
    moves |= temp;
    temp = (knights << 6) & 0xFCFCFCFCFCFCFCFC;
    moves |= temp;
    temp = (knights << 10) & 0x3F3F3F3F3F3F3F3F;
    moves |= temp;
    temp = (knights >> 15) & 0x7F7F7F7F7F7F7F7F;
    moves |= temp;
    temp = (knights >> 17) & 0xFEFEFEFEFEFEFEFE;
    moves |= temp;
    temp = (knights >> 6) & 0x3F3F3F3F3F3F3F3F;
    moves |= temp;
    temp = (knights >> 10) & 0xFCFCFCFCFCFCFCFC;
    moves |= temp;

    return moves;
}

// ========== Generate King Moves ==========
uint64_t getKingMoves(uint64_t king) {
    uint64_t moves = 0;
    moves |= (king << 1) & 0xFEFEFEFEFEFEFEFE;
    moves |= (king >> 1) & 0x7F7F7F7F7F7F7F7F;
    moves |= (king << 8);
    moves |= (king >> 8);
    moves |= (king << 9) & 0xFEFEFEFEFEFEFEFE;
    moves |= (king << 7) & 0x7F7F7F7F7F7F7F7F;
    moves |= (king >> 9) & 0x7F7F7F7F7F7F7F7F;
    moves |= (king >> 7) & 0xFEFEFEFEFEFEFEFE;
    return moves;
}

// ========== Apply Move ==========
ChessState applyMove(const ChessState &state, Move move) {
    ChessState newState = state;
    int from = move.getFrom(), to = move.getTo();

    for (int i = 0; i < 6; i++) {
        if (newState.pieces[i][state.isWhiteToMove] & (1ULL << from)) {
            newState.pieces[i][state.isWhiteToMove] ^= (1ULL << from);
            newState.pieces[i][state.isWhiteToMove] |= (1ULL << to);
            break;
        }
    }

    newState.isWhiteToMove = !state.isWhiteToMove;
    return newState;
}

// ========== Generate Legal Moves ==========
vector<Move> generateMoves(const ChessState &state) {
    vector<Move> moves;
    uint64_t occupied = 0;

    for (int i = 0; i < 6; i++) {
        occupied |= state.pieces[i][0] | state.pieces[i][1];
    }

    uint64_t emptySquares = ~occupied;

    // Generate pawn moves
    uint64_t pawnMoves = getPawnMoves(state.pieces[5][state.isWhiteToMove], emptySquares, state.isWhiteToMove);
    while (pawnMoves) {
        int to = __builtin_ctzll(pawnMoves);
        int from = to + (state.isWhiteToMove ? -8 : 8);
        moves.emplace_back(from, to);
        pawnMoves &= (pawnMoves - 1);
    }

    // Generate knight moves
    uint64_t knightMoves = getKnightMoves(state.pieces[1][state.isWhiteToMove]);
    while (knightMoves) {
        int to = __builtin_ctzll(knightMoves);
        moves.emplace_back(0, to);
        knightMoves &= (knightMoves - 1);
    }

    // Generate king moves
    uint64_t kingMoves = getKingMoves(state.pieces[4][state.isWhiteToMove]);
    while (kingMoves) {
        int to = __builtin_ctzll(kingMoves);
        moves.emplace_back(0, to);
        kingMoves &= (kingMoves - 1);
    }

    return moves;
}

// ========== Minimax Algorithm ==========
Move miniMax(ChessState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0) {
        return Move(0, 0, evaluate(state));
    }

    Move bestMove(0, 0);
    if (maximizingPlayer) {
        int maxEval = -999999;
        for (Move move: generateMoves(state)) {
            ChessState newState = applyMove(state, move);
            int eval = miniMax(newState, depth - 1, alpha, beta, false).getSpecial();
            if (eval > maxEval) {
                maxEval = eval;
                bestMove = move;
            }
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
    } else {
        int minEval = 999999;
        for (Move move: generateMoves(state)) {
            ChessState newState = applyMove(state, move);
            int eval = miniMax(newState, depth - 1, alpha, beta, true).getSpecial();
            if (eval < minEval) {
                minEval = eval;
                bestMove = move;
            }
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
    }
    return bestMove;
}

// ========== Iterative Deepening Search ==========
Move iterativeDeepening(ChessState &state) {
    int alpha = INT_MIN;
    int beta = INT_MAX;
    Move bestMove(0, 0);
    int depth = 2;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    startTime = chrono::steady_clock::now();
    endTime = startTime + chrono::seconds(180);

    while (chrono::steady_clock::now() < endTime) {
        Move currentMove = miniMax(state, depth, alpha, beta, false);
        if (currentMove.getFrom() != 0 || currentMove.getTo() != 0) {
            bestMove = currentMove;
        }
        depth += 2;
        if (depth > 6) break;
    }
    return bestMove;
}

// ========== King in Check Detection ==========
bool isKingInCheck(const ChessState &state) {
    uint64_t kingPos = state.pieces[4][state.isWhiteToMove];
    uint64_t enemyKnights = state.pieces[1][!state.isWhiteToMove];
    uint64_t knightAttacks = getKnightMoves(enemyKnights);

    return (knightAttacks & kingPos) != 0;
}

// ========== Convert Bitboard to Board Array ==========
void bitboardToArray(const ChessState &state, int board[8][8]) {
    memset(board, 0, sizeof(int) * 64);
    for (int piece = 0; piece < 6; piece++) {
        for (int color = 0; color < 2; color++) {
            uint64_t bitboard = state.pieces[piece][color];
            while (bitboard) {
                int index = __builtin_ctzll(bitboard); // Find LSB
                int row = 7 - (index / 8); // Flip board vertically
                int col = index % 8;
                board[row][col] = (color == 0 ? 1 : -1) * (piece + 1);
                bitboard &= (bitboard - 1); // Remove LSB
            }
        }
    }
}

