#ifndef CHESS_H
#define CHESS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <unordered_map>

using namespace std;

// ========== Chess Constants ==========
constexpr int boardSize = 8;
constexpr int squareSize = 150;
// Declare global variables for GUI and game state
extern int curTurn;
extern bool isGameOver;
extern sf::RenderWindow window;
extern sf::Texture pieceTextures[15];
extern sf::Texture highlightTextures[12];
extern sf::Sprite pieceSprites[boardSize + 1][boardSize];

// Piece Identifiers
enum PieceType {
    ROOK = 0, KNIGHT = 1, BISHOP = 2, QUEEN = 3, KING = 4, PAWN = 5
};

// Bitboards are indexed by [PieceType][Color] (0 = White, 1 = Black)
struct ChessState {
    uint64_t pieces[6][2]{}; // 6 piece types, 2 colors
    uint8_t castlingRights; // 4 bits (KQkq)
    uint8_t enPassantFile; // 6 bits (0-7 if available, 255 if none)
    uint16_t halfmoveClock; // 50-move rule
    uint16_t fullmoveCount; // Move counter
    bool isWhiteToMove; // 1 bit

    ChessState() {
        for (auto &piece: pieces) {
            piece[0] = piece[1] = 0;
        }
        castlingRights = 0b1111;
        enPassantFile = 255;
        halfmoveClock = 0;
        fullmoveCount = 1;
        isWhiteToMove = true;
    }
};

// ========== Compact Move Representation (16-bit) ==========
struct Move {
    uint16_t moveData;

    explicit Move(int from = 0, int to = 0, int special = 0) {
        moveData = (from & 0x3F) | ((to & 0x3F) << 6) | ((special & 0xF) << 12);
    }

    [[nodiscard]] int getFrom() const { return moveData & 0x3F; }
    [[nodiscard]] int getTo() const { return (moveData >> 6) & 0x3F; }
    [[nodiscard]] int getSpecial() const { return (moveData >> 12) & 0xF; }
};

// ========== Global Variables ==========

inline sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));

// ========== Function Declarations (No Definitions Here) ==========
void loadTextures();

void createSprites();

void render(int board[8][8]);

void bitboardToArray(const ChessState &state, int board[8][8]);

int evaluate(const ChessState &state);

Move miniMax(const ChessState &state, int depth, int alpha, int beta, bool maximizingPlayer);

Move iterativeDeepening(const ChessState &state);

ChessState applyMove(const ChessState &state, Move move);

vector<Move> generateMoves(const ChessState &state);

void handleEvents();

#endif // CHESS_H
