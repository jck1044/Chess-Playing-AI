#include "chess.h"
#include <iostream>

// Global game state variables
int curTurn = 1; // 1 = White, 2 = Black
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
                    std::cout << "Mouse click at pixel (" << mousePosition.x << ", " << mousePosition.y
                            << ") mapped to board (" << row << ", " << col << ")\n";

                    static sf::Vector2i selectedPiecePosition;
                    static bool isPieceSelected = false;

                    if (!isPieceSelected) {
                        if ((board[row][col] > 0 && curTurn == 1) || (board[row][col] < 0 && curTurn == 2)) {
                            isPieceSelected = true;
                            selectedPiecePosition = sf::Vector2i(col, row);
                            std::cout << "Piece selected at board (" << row << ", " << col << ")\n";
                        }
                    } else {
                        int selectedCol = selectedPiecePosition.x;
                        int selectedRow = selectedPiecePosition.y;
                        // Convert board coordinates to bitboard indices using the flipped row:
                        int from = (7 - selectedRow) * 8 + selectedCol;
                        int to = (7 - row) * 8 + col;
                        std::cout << "Attempting move from board (" << selectedRow << ", " << selectedCol
                                << ") [bit index " << from << "] to board (" << row << ", " << col
                                << ") [bit index " << to << "]\n";

                        Move move(from, to);

                        // Validate the move against generated legal moves.
                        vector<Move> legalMoves = generateMoves(boardState);
                        bool legal = false;
                        for (const auto &m: legalMoves) {
                            if (m.getFrom() == move.getFrom() && m.getTo() == move.getTo()) {
                                legal = true;
                                break;
                            }
                        }
                        if (!legal) {
                            std::cout << "Illegal move attempted: from " << move.getFrom()
                                    << " to " << move.getTo() << "\n";
                            isPieceSelected = false; // Reset selection
                            continue; // Ignore the move
                        }

                        boardState = applyMove(boardState, move);
                        std::cout << "Player move applied.\n";

                        isPieceSelected = false;
                        curTurn = (curTurn % 2) + 1;
                        if (curTurn == 2) {
                            std::cout << "AI's turn. Starting iterative deepening search...\n";
                            Move aiMove = iterativeDeepening(boardState);
                            std::cout << "AI move: from " << aiMove.getFrom() << " to "
                                    << aiMove.getTo() << " with special " << aiMove.getSpecial() << "\n";
                            boardState = applyMove(boardState, aiMove);
                            std::cout << "AI move applied. Resetting turn to white.\n";
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
