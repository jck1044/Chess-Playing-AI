#include "chess.h"
#include "gui.cpp"
#include "gameplay.cpp"

// Global game state variables
int curTurn = 1;  // 1 = White, 2 = Black
bool isGameOver = false;

// SFML window instance
sf::RenderWindow window(sf::VideoMode(1200, 1200), "Chess Game");

ChessState boardState;

// Keep only function implementations
int evaluate(const ChessState &state) {
    const int pieceValues[6] = {100, 300, 320, 500, 900, 10000};
    int score = 0;

    for (int i = 0; i < 6; i++) {
        score += __builtin_popcountll(state.pieces[i][0]) * pieceValues[i];
        score -= __builtin_popcountll(state.pieces[i][1]) * pieceValues[i];
    }

    return state.isWhiteToMove ? score : -score;
}

void handleEvents() {
    int board[8][8];
    bitboardToArray(boardState, board);
    render(board);

    if (curTurn == 1) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && !isGameOver) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int col = mousePosition.x / squareSize;
                    int row = mousePosition.y / squareSize;

                    static sf::Vector2i selectedPiecePosition;
                    static bool isPieceSelected = false;

                    if (!isPieceSelected) {
                        if ((board[row][col] > 0 && curTurn == 1) || (board[row][col] < 0 && curTurn == 2)) {
                            isPieceSelected = true;
                            selectedPiecePosition = sf::Vector2i(col, row);
                        }
                    } else {
                        int selectedCol = selectedPiecePosition.x;
                        int selectedRow = selectedPiecePosition.y;

                        int from = selectedRow * 8 + selectedCol;
                        int to = row * 8 + col;
                        Move move(from, to);
                        boardState = applyMove(boardState, move);

                        isPieceSelected = false;
                        curTurn = (curTurn % 2) + 1;
                        if (curTurn == 2) {
                            Move aiMove = iterativeDeepening(boardState);
                            boardState = applyMove(boardState, aiMove);
                            curTurn = 1;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // Set up White pieces
    boardState.pieces[5][0] = 0x000000000000FF00; // White Pawns
    boardState.pieces[4][0] = 0x0000000000000010; // White King
    boardState.pieces[3][0] = 0x0000000000000008; // White Queen
    boardState.pieces[2][0] = 0x0000000000000024; // White Bishops
    boardState.pieces[1][0] = 0x0000000000000042; // White Knights
    boardState.pieces[0][0] = 0x0000000000000081; // White Rooks

    // Set up Black pieces
    boardState.pieces[5][1] = 0x00FF000000000000; // Black Pawns
    boardState.pieces[4][1] = 0x1000000000000000; // Black King
    boardState.pieces[3][1] = 0x0800000000000000; // Black Queen
    boardState.pieces[2][1] = 0x2400000000000000; // Black Bishops
    boardState.pieces[1][1] = 0x4200000000000000; // Black Knights
    boardState.pieces[0][1] = 0x8100000000000000; // Black Rooks

    loadTextures();
    createSprites();
    while (window.isOpen()) {
        handleEvents();
    }

    return 0;
}
