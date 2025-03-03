#include "chess.h"

// ========== Generate Pawn Moves ==========
uint64_t getPawnMoves(uint64_t pawn, uint64_t emptySquares, bool isWhite) {
    if (isWhite) {
        uint64_t oneStep = (pawn << 8) & emptySquares;
        uint64_t twoStep = 0;
        // White pawns start on rank 2 (bits 8 to 15)
        if ((pawn & 0x000000000000FF00ULL) && oneStep) {
            twoStep = (pawn << 16) & emptySquares;
        }
        return oneStep | twoStep;
    } else {
        uint64_t oneStep = (pawn >> 8) & emptySquares;
        uint64_t twoStep = 0;
        // Black pawns start on rank 7 (bits 48 to 55)
        if ((pawn & 0x00FF000000000000ULL) && oneStep) {
            twoStep = (pawn >> 16) & emptySquares;
        }
        return oneStep | twoStep;
    }
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
    int color = (state.isWhiteToMove ? 0 : 1); // Correct mapping: white->0, black->1

    for (int i = 0; i < 6; i++) {
        if (newState.pieces[i][color] & (1ULL << from)) {
            newState.pieces[i][color] ^= (1ULL << from);
            newState.pieces[i][color] |= (1ULL << to);
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
    int color = (state.isWhiteToMove ? 0 : 1);

    // Compute friendly and enemy occupancy.
    uint64_t friendlyOccupancy = 0;
    for (int i = 0; i < 6; i++) {
        friendlyOccupancy |= state.pieces[i][color];
    }
    uint64_t enemyOccupancy = occupied & ~friendlyOccupancy;

    // ===== Pawn Moves =====
    uint64_t pawns = state.pieces[5][color];
    while (pawns) {
        int from = __builtin_ctzll(pawns);
        uint64_t pawn = 1ULL << from;
        uint64_t oneStep, twoStep, leftCapture, rightCapture;
        if (color == 0) {
            oneStep = (pawn << 8) & emptySquares;
            twoStep = 0;
            // White pawns start on rank 2 (bit indices 8..15)
            if ((pawn & 0x000000000000FF00ULL) && oneStep) {
                twoStep = (pawn << 16) & emptySquares;
            }
            // Apply file masks BEFORE shifting to prevent wrap-around.
            leftCapture = ((pawn & 0xFEFEFEFEFEFEFEFEULL) << 7) & enemyOccupancy;
            rightCapture = ((pawn & 0x7F7F7F7F7F7F7F7FULL) << 9) & enemyOccupancy;
        } else {
            oneStep = (pawn >> 8) & emptySquares;
            twoStep = 0;
            // Black pawns start on rank 7 (bit indices 48..55)
            if ((pawn & 0x00FF000000000000ULL) && oneStep) {
                twoStep = (pawn >> 16) & emptySquares;
            }
            // Apply file masks BEFORE shifting to prevent wrap-around.
            leftCapture = ((pawn & 0xFEFEFEFEFEFEFEFEULL) >> 9) & enemyOccupancy;
            rightCapture = ((pawn & 0x7F7F7F7F7F7F7F7FULL) >> 7) & enemyOccupancy;
        }
        // Add one- and two-step moves.
        uint64_t pawnMoves = oneStep | twoStep;
        while (pawnMoves) {
            int to = __builtin_ctzll(pawnMoves);
            moves.emplace_back(from, to);
            pawnMoves &= (pawnMoves - 1);
        }
        // Add capturing moves.
        if (leftCapture) {
            int to = __builtin_ctzll(leftCapture);
            moves.emplace_back(from, to);
        }
        if (rightCapture) {
            int to = __builtin_ctzll(rightCapture);
            moves.emplace_back(from, to);
        }
        pawns &= (pawns - 1);
    }

    // ===== Knight Moves =====
    uint64_t knights = state.pieces[1][color];
    while (knights) {
        int from = __builtin_ctzll(knights);
        uint64_t knight = 1ULL << from;
        uint64_t movesForKnight = getKnightMoves(knight) & ~friendlyOccupancy;
        while (movesForKnight) {
            int to = __builtin_ctzll(movesForKnight);
            moves.emplace_back(from, to);
            movesForKnight &= (movesForKnight - 1);
        }
        knights &= (knights - 1);
    }

    // ===== Bishop Moves (Diagonals) =====
    uint64_t bishops = state.pieces[2][color];
    int bishopDirs[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    while (bishops) {
        int from = __builtin_ctzll(bishops);
        int rank = from / 8;
        int file = from % 8;
        for (int d = 0; d < 4; d++) {
            int dr = bishopDirs[d][0], dc = bishopDirs[d][1];
            int r = rank, c = file;
            while (true) {
                r += dr;
                c += dc;
                if (r < 0 || r >= 8 || c < 0 || c >= 8) break;
                int to = r * 8 + c;
                if (emptySquares & (1ULL << to)) {
                    moves.emplace_back(from, to);
                } else if (enemyOccupancy & (1ULL << to)) {
                    moves.emplace_back(from, to);
                    break;
                } else {
                    break;
                }
            }
        }
        bishops &= (bishops - 1);
    }

    // ===== Rook Moves (Vertical & Horizontal) =====
    uint64_t rooks = state.pieces[0][color];
    int rookDirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (rooks) {
        int from = __builtin_ctzll(rooks);
        int rank = from / 8;
        int file = from % 8;
        for (int d = 0; d < 4; d++) {
            int dr = rookDirs[d][0], dc = rookDirs[d][1];
            int r = rank, c = file;
            while (true) {
                r += dr;
                c += dc;
                if (r < 0 || r >= 8 || c < 0 || c >= 8) break;
                int to = r * 8 + c;
                if (emptySquares & (1ULL << to)) {
                    moves.emplace_back(from, to);
                } else if (enemyOccupancy & (1ULL << to)) {
                    moves.emplace_back(from, to);
                    break;
                } else {
                    break;
                }
            }
        }
        rooks &= (rooks - 1);
    }

    // ===== Queen Moves (Rook + Bishop Directions) =====
    uint64_t queens = state.pieces[3][color];
    int queenDirs[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    while (queens) {
        int from = __builtin_ctzll(queens);
        int rank = from / 8;
        int file = from % 8;
        for (int d = 0; d < 8; d++) {
            int dr = queenDirs[d][0], dc = queenDirs[d][1];
            int r = rank, c = file;
            while (true) {
                r += dr;
                c += dc;
                if (r < 0 || r >= 8 || c < 0 || c >= 8) break;
                int to = r * 8 + c;
                if (emptySquares & (1ULL << to)) {
                    moves.emplace_back(from, to);
                } else if (enemyOccupancy & (1ULL << to)) {
                    moves.emplace_back(from, to);
                    break;
                } else {
                    break;
                }
            }
        }
        queens &= (queens - 1);
    }

    // ===== King Moves =====
    uint64_t kings = state.pieces[4][color];
    while (kings) {
        int from = __builtin_ctzll(kings);
        uint64_t king = 1ULL << from;
        uint64_t movesForKing = getKingMoves(king) & ~friendlyOccupancy;
        while (movesForKing) {
            int to = __builtin_ctzll(movesForKing);
            moves.emplace_back(from, to);
            movesForKing &= (movesForKing - 1);
        }
        kings &= (kings - 1);
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
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::seconds(180);

    while (std::chrono::steady_clock::now() < endTime) {
        Move currentMove = miniMax(state, depth, alpha, beta, false);
        if (currentMove.getFrom() != 0 || currentMove.getTo() != 0) {
            bestMove = currentMove;
        }
        depth += 2;
        if (depth > 2) break;
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
