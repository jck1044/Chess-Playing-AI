#include "chess.h"
#include <cstring>
#include <iostream>

// ========== Generate Knight Moves ==========
uint64_t getKnightMoves(uint64_t knights) {
    uint64_t moves = 0;
    const uint64_t notAFile = 0xfefefefefefefefeULL;
    const uint64_t notHFile = 0x7f7f7f7f7f7f7f7fULL;
    const uint64_t notABFile = 0xfcfcfcfcfcfcfcfcULL;
    const uint64_t notGHFile = 0x3f3f3f3f3f3f3f3fULL;

    moves |= (knights << 17) & notAFile; // 2 up, 1 right
    moves |= (knights << 15) & notHFile; // 2 up, 1 left
    moves |= (knights << 10) & notABFile; // 1 up, 2 right
    moves |= (knights << 6) & notGHFile; // 1 up, 2 left
    moves |= (knights >> 17) & notHFile; // 2 down, 1 left
    moves |= (knights >> 15) & notAFile; // 2 down, 1 right
    moves |= (knights >> 10) & notGHFile; // 1 down, 2 left
    moves |= (knights >> 6) & notABFile; // 1 down, 2 right

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
    int color = (state.isWhiteToMove ? 0 : 1); // white->0, black->1
    int enemy = !color;

    // Remove enemy piece on the destination square, if any.
    for (auto &piece: newState.pieces) {
        if (piece[enemy] & (1ULL << to)) {
            piece[enemy] &= ~(1ULL << to);
            break;
        }
    }

    // Move the piece for the current player.
    for (auto &piece: newState.pieces) {
        if (piece[color] & (1ULL << from)) {
            piece[color] &= ~(1ULL << from);
            piece[color] |= (1ULL << to);
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
    for (auto piece: state.pieces) {
        occupied |= piece[0] | piece[1];
    }
    uint64_t emptySquares = ~occupied;
    int color = (state.isWhiteToMove ? 0 : 1);

    // Compute friendly and enemy occupancy.
    uint64_t friendlyOccupancy = 0;
    for (auto piece: state.pieces) {
        friendlyOccupancy |= piece[color];
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
        for (auto &bishopDir: bishopDirs) {
            int dr = bishopDir[0], dc = bishopDir[1];
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
        for (auto &rookDir: rookDirs) {
            int dr = rookDir[0], dc = rookDir[1];
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
        for (auto &queenDir: queenDirs) {
            int dr = queenDir[0], dc = queenDir[1];
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

// ---------------------------------------------------------------------
// Transposition Table Definitions and Hash Computation
// ---------------------------------------------------------------------
enum TTFlag { EXACT, LOWERBOUND, UPPERBOUND };

struct TTEntry {
    int eval; // Evaluation of the position
    int depth; // Remaining search depth at which this was computed
    TTFlag flag; // The type of bound this evaluation represents
    Move bestMove; // The best move found from this position
};

static std::unordered_map<uint64_t, TTEntry> transpositionTable;

uint64_t computeHash(const ChessState &state) {
    uint64_t hash = 1469598103934665603ULL; // FNV offset basis
    auto hashCombine = [&hash](uint64_t data) {
        hash ^= data;
        hash *= 1099511628211ULL; // FNV prime
    };

    for (int piece = 0; piece < 6; piece++) {
        hashCombine(state.pieces[piece][0]);
        hashCombine(state.pieces[piece][1]);
    }
    hashCombine(state.castlingRights);
    hashCombine(state.enPassantFile);
    hashCombine(state.halfmoveClock);
    hashCombine(state.fullmoveCount);
    hashCombine(state.isWhiteToMove ? 1ULL : 0ULL);
    return hash;
}

// ---------------------------------------------------------------------
// Updated minimax function with transposition table and alpha-beta pruning
// ---------------------------------------------------------------------
Move miniMax(const ChessState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0) {
        int evalScore = evaluate(state);
        std::cout << "Leaf reached at depth 0: eval = " << evalScore << endl;
        return Move(0, 0, evalScore);
    }

    int originalAlpha = alpha;
    int originalBeta = beta;
    uint64_t hash = computeHash(state);

    if (transpositionTable.find(hash) != transpositionTable.end()) {
        TTEntry entry = transpositionTable[hash];
        if (entry.depth >= depth) {
            if (entry.flag == EXACT) {
                return entry.bestMove;
            } else if (entry.flag == LOWERBOUND) {
                alpha = std::max(alpha, entry.eval);
            } else if (entry.flag == UPPERBOUND) {
                beta = std::min(beta, entry.eval);
            }
            if (alpha >= beta) {
                return entry.bestMove;
            }
        }
    }

    Move bestMove(0, 0);
    int bestEval = maximizingPlayer ? -999999 : 999999;

    vector<Move> moves = generateMoves(state);
    for (Move move: moves) {
        std::cout << "Depth " << depth << ": Evaluating move from " << move.getFrom() << " to " << move.getTo() << endl;
        ChessState newState = applyMove(state, move);
        int eval = miniMax(newState, depth - 1, alpha, beta, !maximizingPlayer).getSpecial();
        std::cout << "Move from " << move.getFrom() << " to " << move.getTo() << " evaluated to " << eval << endl;
        if (maximizingPlayer) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, eval);
        } else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            beta = std::min(beta, eval);
        }
        std::cout << "After move: alpha = " << alpha << ", beta = " << beta << endl;
        if (alpha >= beta) {
            std::cout << "Pruning at depth " << depth << " with alpha = " << alpha << " and beta = " << beta << endl;
            break;
        }
    }

    TTFlag flag;
    if (bestEval <= originalAlpha) {
        flag = UPPERBOUND;
    } else if (bestEval >= originalBeta) {
        flag = LOWERBOUND;
    } else {
        flag = EXACT;
    }
    transpositionTable[hash] = {bestEval, depth, flag, bestMove};

    return bestMove;
}

// ========== Iterative Deepening Search ==========
Move iterativeDeepening(const ChessState &state) {
    int alpha = INT_MIN;
    int beta = INT_MAX;
    Move bestMove(0, 0);
    int depth = 2;
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::seconds(180);

    while (std::chrono::steady_clock::now() < endTime) {
        std::cout << "Iterative deepening at depth: " << depth << endl;
        Move currentMove = miniMax(state, depth, alpha, beta, false);
        std::cout << "At depth " << depth << ", best move so far: from "
                << currentMove.getFrom() << " to " << currentMove.getTo()
                << " with eval " << currentMove.getSpecial() << endl;
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
