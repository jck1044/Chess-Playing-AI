#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;


struct MovePiece {
    int initRow = -1;
    int initCol = -1;
    int newRow = -1;
    int newCol = -1;
    int score{};
};

struct Node {
    int chessboard[8][8]{};
//    int score{};
    MovePiece moveToGetHere;
    MovePiece bestChild;
    bool doesWhiteMoveFromHere{};
//    int depth;
};


class Stack {
public:
    Stack() = default;

    ~Stack() = default;

    void push(Node value) {
        stack.push_back(value);
    }

    Node pop() {
        Node value = stack.back();
        stack.pop_back();
        return value;
    }

    bool empty() {
        return stack.empty();
    }

private:
    vector<Node> stack;
};

sf::RenderWindow window(sf::VideoMode(1500, 1500), "White's Turn");
const int boardSize = 8;
const int squareSize = 150;
const int BLACKPAWN = -6;
const int BLACKKING = -5;
const int BLACKQUEEN = -4;
const int BLACKBISHOP = -3;
const int BLACKKNIGHT = -2;
const int BLACKROOK = -1;
const int WHITEPAWN = 6;
const int WHITEKING = 5;
const int WHITEQUEEN = 4;
const int WHITEBISHOP = 3;
const int WHITEKNIGHT = 2;
const int WHITEROOK = 1;
int curTurn = 1;
int hundredMovesNoPawnMoveCounter = 0;
std::vector<int> favorCenter = {4, 3, 5, 2, 6, 1, 7, 0};

// Load chess piece textures
sf::Texture pieceTextures[15];
sf::Texture highlightTextures[12];
sf::Sprite pieceSprites[boardSize + 1][boardSize];
sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));

void loadTextures() {
    pieceTextures[0].loadFromFile("piecePics/whiteRook.png");
    pieceTextures[1].loadFromFile("piecePics/whiteKnight.png");
    pieceTextures[2].loadFromFile("piecePics/whiteBishop.png");
    pieceTextures[3].loadFromFile("piecePics/whiteQueen.png");
    pieceTextures[4].loadFromFile("piecePics/whiteKing.png");
    pieceTextures[5].loadFromFile("piecePics/whitePawn.png");
    pieceTextures[6].loadFromFile("piecePics/blackRook.png");
    pieceTextures[7].loadFromFile("piecePics/blackKnight.png");
    pieceTextures[8].loadFromFile("piecePics/blackBishop.png");
    pieceTextures[9].loadFromFile("piecePics/blackQueen.png");
    pieceTextures[10].loadFromFile("piecePics/blackKing.png");
    pieceTextures[11].loadFromFile("piecePics/blackPawn.png");
    pieceTextures[12].loadFromFile("piecePics/inCheck.png");
    pieceTextures[13].loadFromFile("piecePics/inCheckMate.png");
    pieceTextures[14].loadFromFile("piecePics/draw.png");

    highlightTextures[0].loadFromFile("piecePics/whiteRookHighlight.png");
    highlightTextures[1].loadFromFile("piecePics/whiteKnightHighlight.png");
    highlightTextures[2].loadFromFile("piecePics/whiteBishopHighlight.png");
    highlightTextures[3].loadFromFile("piecePics/whiteQueenHighlight.png");
    highlightTextures[4].loadFromFile("piecePics/whiteKingHighlight.png");
    highlightTextures[5].loadFromFile("piecePics/whitePawnHighlight.png");
    highlightTextures[6].loadFromFile("piecePics/blackRookHighlight.png");
    highlightTextures[7].loadFromFile("piecePics/blackKnightHighlight.png");
    highlightTextures[8].loadFromFile("piecePics/blackBishopHighlight.png");
    highlightTextures[9].loadFromFile("piecePics/blackQueenHighlight.png");
    highlightTextures[10].loadFromFile("piecePics/blackKingHighlight.png");
    highlightTextures[11].loadFromFile("piecePics/blackPawnHighlight.png");
}

void createSprites(int board[8][8]) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != 0) {
                int pieceIndex = board[i][j];
                if (pieceIndex < 0) {
                    pieceIndex = ((pieceIndex + 1) * -1) + 6;
                } else {
                    pieceIndex = pieceIndex - 1;
                }
                pieceSprites[i][j].setTexture(pieceTextures[pieceIndex]);
                pieceSprites[i][j].setPosition(j * squareSize, i * squareSize);
            }
        }
    }
    pieceSprites[boardSize][0].setPosition(boardSize * squareSize, 0 * squareSize);
    pieceSprites[boardSize][1].setPosition(boardSize * squareSize, 1 * squareSize);
    pieceSprites[boardSize][2].setPosition(boardSize * squareSize, 2 * squareSize);

}

void drawBoard() {
    // Draw the background
    background.setFillColor(sf::Color(232, 173, 115));
    window.draw(background);
    sf::RectangleShape boardSquare(sf::Vector2f(squareSize, squareSize));
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if ((i + j) % 2 == 0) {
                boardSquare.setFillColor(sf::Color(255, 206, 158));
            } else {
                boardSquare.setFillColor(sf::Color(209, 139, 71));
            }
            boardSquare.setPosition(i * squareSize, j * squareSize);
            window.draw(boardSquare);
        }
    }
}

void drawPieces(int board[8][8]) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != 0) {
                window.draw(pieceSprites[i][j]);
            }
        }
    }
    window.draw(pieceSprites[boardSize][0]);
    window.draw(pieceSprites[boardSize][1]);
    window.draw(pieceSprites[boardSize][2]);
}

bool isRookMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    // Check if the move is a horizontal move
    if ((isWhite && board[secondRow][secondCol] <= 0) ||
        (!isWhite && board[secondRow][secondCol] >= 0)) {
        if (firstRow == secondRow && firstCol != secondCol) {
            // Check if there are any pieces blocking the horizontal path
            int step = (secondCol > firstCol) ? 1 : -1;
            for (int x = firstCol + step; x != secondCol; x += step) {
                if (board[firstRow][x] != 0) {
                    return false;
                }
            }
            return true;
        }
        // Check if the move is a vertical move
        if (firstCol == secondCol && firstRow != secondRow) {
            // Check if there are any pieces blocking the vertical path
            int step = (secondRow > firstRow) ? 1 : -1;
            for (int y = firstRow + step; y != secondRow; y += step) {
                if (board[y][firstCol] != 0) {
                    return false;
                }
            }
            return true;
        }
    }
    // The move is not legal for a black rook
    return false;
}

bool isBishopMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    if ((isWhite && board[secondRow][secondCol] <= 0) ||
        (!isWhite && board[secondRow][secondCol] >= 0)) {
        // Check if the move is a diagonal move
        if (std::abs(firstCol - secondCol) == std::abs(firstRow - secondRow)) {
            // Determine the direction of the diagonal move
            int xStep = (secondCol > firstCol) ? 1 : -1;
            int yStep = (secondRow > firstRow) ? 1 : -1;
            // Check if there are any pieces blocking the diagonal path
            int x = firstCol + xStep;
            int y = firstRow + yStep;
            while (x != secondCol && y != secondRow) {
                if (board[y][x] != 0) {
                    return false;
                }
                x += xStep;
                y += yStep;
            }
            return true;
        }
    }
    // The move is not legal for a white bishop
    return false;
}

bool isKnightMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    if ((isWhite && board[secondRow][secondCol] <= 0) ||
        (!isWhite && board[secondRow][secondCol] >= 0)) {
        // Calculate the absolute differences in row and column
        int diffX = std::abs(firstCol - secondCol);
        int diffY = std::abs(firstRow - secondRow);
        // Check if the move is an L-shaped knight move
        if ((diffX == 2 && diffY == 1) || (diffX == 1 && diffY == 2)) {
            return true;
        }
        // The move is not legal for a white knight
        return false;
    }
    // The move is not legal for a white bishop
    return false;
}

bool isQueenMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    if ((isWhite && board[secondRow][secondCol] <= 0) ||
        (!isWhite && board[secondRow][secondCol] >= 0)) {
        // Calculate the absolute differences in row and column
        int diffX = std::abs(firstCol - secondCol);
        int diffY = std::abs(firstRow - secondRow);
        // Check if the move is a valid queen move (horizontal, vertical, or diagonal)
        if ((firstCol == secondCol || firstRow == secondRow) || (diffX == diffY)) {
            // Horizontal move
            if (firstCol == secondCol && firstRow != secondRow) {
                int step = (secondRow > firstRow) ? 1 : -1;
                for (int y = firstRow + step; y != secondRow; y += step) {
                    if (board[y][firstCol] != 0) {
                        return false;
                    }
                }
            }
                // Vertical move
            else if (firstRow == secondRow && firstCol != secondCol) {
                int step = (secondCol > firstCol) ? 1 : -1;
                for (int x = firstCol + step; x != secondCol; x += step) {
                    if (board[firstRow][x] != 0) {
                        return false;
                    }
                }
            }
                // Diagonal move
            else if (diffX == diffY) {
                int xStep = (secondCol > firstCol) ? 1 : -1;
                int yStep = (secondRow > firstRow) ? 1 : -1;
                int x = firstCol + xStep;
                int y = firstRow + yStep;
                while (x != secondCol && y != secondRow) {
                    if (board[y][x] != 0) {
                        return false;
                    }
                    x += xStep;
                    y += yStep;
                }
            }
            return true;
        }
    }
    // The move is not legal for a white bishop
    return false;
}

bool isKingMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    if ((isWhite && board[secondRow][secondCol] <= 0) ||
        (!isWhite && board[secondRow][secondCol] >= 0)) {
        // Calculate the absolute differences in row and column
        int diffX = std::abs(firstCol - secondCol);
        int diffY = std::abs(firstRow - secondRow);
        // Check if the move is a valid king move
        if (diffX <= 1 && diffY <= 1) {
            return true;
        }
    }
    // The move is not legal for a white bishop
    return false;
}

bool isPawnMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    if (isWhite) {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow - 1 == secondRow && board[secondRow][secondCol] == 0) {
            return true;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 6 && secondRow == 4 && board[4][secondCol] == 0 &&
            board[5][secondCol] == 0) {
            return true;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == 1 && board[secondRow][secondCol] < 0) {
            return true;
        }
    } else {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow + 1 == secondRow && board[secondRow][secondCol] == 0) {
            return true;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 1 && secondRow == 3 && board[3][secondCol] == 0 &&
            board[2][secondCol] == 0) {
            return true;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == -1 && board[secondRow][secondCol] > 0) {
            return true;
        }
    }
    return false;
}

int isMoveAllowed(int firstRow, int firstCol, int secondRow, int secondCol, int board[8][8]) {
    int piece = board[firstRow][firstCol];
    if (firstRow == secondRow && firstCol == secondCol) { //unselecting piece
        return -1;
    } else if (piece == WHITEPAWN) {
        return isPawnMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKPAWN) {
        return isPawnMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    } else if (piece == WHITEROOK) {
        return isRookMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKROOK) {
        return isRookMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    } else if (piece == WHITEBISHOP) {
        return isBishopMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKBISHOP) {
        return isBishopMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    } else if (piece == WHITEKNIGHT) {
        return isKnightMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKKNIGHT) {
        return isKnightMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    } else if (piece == WHITEQUEEN) {
        return isQueenMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKQUEEN) {
        return isQueenMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    } else if (piece == WHITEKING) {
        return isKingMoveValid(firstCol, firstRow, secondCol, secondRow, true, board);
    } else if (piece == BLACKKING) {
        return isKingMoveValid(firstCol, firstRow, secondCol, secondRow, false, board);
    }
    return false;
}

bool isKingInCheck(int kingX, int kingY, int board[8][8]) {
    bool isWhiteKing = board[kingY][kingX] > 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if ((isWhiteKing && board[row][col] < 0) || (!isWhiteKing && board[row][col] > 0)) {
                if (isMoveAllowed(row, col, kingY, kingX, board)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isInStalemate(bool isWhite, int board[8][8]) {
    for (int initRow = 0; initRow < boardSize; initRow++) {
        for (int initCol = 0; initCol < boardSize; initCol++) {
            if ((isWhite && board[initRow][initCol] > 0) ||
                (!isWhite && board[initRow][initCol] < 0)) {
                if ((isWhite && board[initRow][initCol] == WHITEKING) ||
                    (!isWhite && board[initRow][initCol] == BLACKKING)) {
                    if (isKingInCheck(initCol, initRow, board)) {
                        return false;
                    }
                }
                for (int newRow = 0; newRow < boardSize; newRow++) {
                    for (int newCol = 0; newCol < boardSize; newCol++) {
                        if (initRow != newRow || initCol != newCol) {
                            if (isMoveAllowed(initRow, initCol, newRow, newCol, board)) {
                                int tempChessboard[boardSize][boardSize];
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        tempChessboard[i][j] = board[i][j];
                                    }
                                }
                                board[newRow][newCol] = board[initRow][initCol];
                                board[initRow][initCol] = 0;
                                if ((isWhite && board[newRow][newCol] == WHITEKING) ||
                                    (!isWhite && board[newRow][newCol] == BLACKKING)) {
                                    if (!isKingInCheck(newCol, newRow, board)) {
                                        for (int i = 0; i < boardSize; i++) {
                                            for (int j = 0; j < boardSize; j++) {
                                                board[i][j] = tempChessboard[i][j];
                                            }
                                        }
                                        return false;
                                    }
                                } else {
                                    for (int i = 0; i < boardSize; i++) {
                                        for (int j = 0; j < boardSize; j++) {
                                            board[i][j] = tempChessboard[i][j];
                                        }
                                    }
                                    return false;
                                }
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        board[i][j] = tempChessboard[i][j];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool isInsufficientMaterial(int board[8][8]) {
    int numWhiteBishops = 0;
    int numWhiteKnights = 0;
    int numBlackBishops = 0;
    int numBlackKnights = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            int piece = board[i][j];
            if (piece == WHITEPAWN || piece == BLACKPAWN || piece == WHITEQUEEN || piece == BLACKQUEEN ||
                piece == WHITEROOK ||
                piece == BLACKROOK) {
                return false;
            }
            if (piece == WHITEBISHOP) {
                numWhiteBishops++;
                if (numWhiteBishops > 1) {
                    return false;
                }
            } else if (piece == WHITEKNIGHT) {
                numWhiteKnights++;
                if (numWhiteKnights > 1) {
                    return false;
                }
            } else if (piece == BLACKBISHOP) {
                numBlackBishops++;
                if (numBlackBishops > 1) {
                    return false;
                }
            } else if (piece == BLACKKNIGHT) {
                numBlackKnights++;
                if (numBlackKnights > 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool canKingEscapeCheck(int kingX, int kingY, int board[8][8]) {
    // Check all possible king moves
    int kingMoves[8][2] = {{1,  0},
                           {1,  1},
                           {0,  1},
                           {-1, 1},
                           {-1, 0},
                           {-1, -1},
                           {0,  -1},
                           {1,  -1}};
    bool isKingWhite = board[kingY][kingX];

    for (auto &kingMove: kingMoves) {
        int newX = kingX + kingMove[0];
        int newY = kingY + kingMove[1];
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            // Check if the new position is not under threat
            if ((isKingWhite && board[newY][newX] <= 0) || (!isKingWhite && board[newY][newX] >= 0)) {
                int tempChessboard[boardSize][boardSize];
                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        tempChessboard[i][j] = board[i][j];
                    }
                }
                board[newY][newX] = board[kingY][kingX];
                board[kingY][kingX] = 0;
                if (!isKingInCheck(newX, newY, board)) {
                    for (int i = 0; i < boardSize; i++) {
                        for (int j = 0; j < boardSize; j++) {
                            board[i][j] = tempChessboard[i][j];
                        }
                    }
                    return true;
                }
                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        board[i][j] = tempChessboard[i][j];
                    }
                }
            }
        }
    }
    // King cannot escape check by moving
    return false;
}

bool isKingInCheckmate(int kingX, int kingY, int board[8][8]) {
    bool isKingWhite = board[kingY][kingX] > 0;
    // Check if the king is in check
    if (!isKingInCheck(kingX, kingY, board)) {
        return false;
    }
    // Check if the king can escape the check
    if (canKingEscapeCheck(kingX, kingY, board)) {
        return false;
    }
    // Check if any piece can block or capture the attacking piece
    int tempChessboard[boardSize][boardSize];
    // Copy values from chessboard to tempChessboard
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            tempChessboard[i][j] = board[i][j];
        }
    }
    for (int initRow = 0; initRow < boardSize; initRow++) {
        for (int initCol = 0; initCol < boardSize; initCol++) {
            if ((isKingWhite && board[initRow][initCol] > 0) ||
                (!isKingWhite && board[initRow][initCol] < 0)) {
                for (int newRow = 0; newRow < boardSize; newRow++) {
                    for (int newCol = 0; newCol < boardSize; newCol++) {
                        if ((initRow != newRow || initCol != newCol) &&
                            isMoveAllowed(initRow, initCol, newRow, newCol, board)) {
                            board[newRow][newCol] = board[initRow][initCol];
                            board[initRow][initCol] = 0;
                            if (!isKingInCheck(kingX, kingY, board)) {
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        board[i][j] = tempChessboard[i][j];
                                    }
                                }
                                return false;
                            }
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    board[i][j] = tempChessboard[i][j];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
// The king is in checkmate
    return true;
}

int getBoardEvaluation(int board[8][8]) {
    int score = 0;

    // Piece values
    int pawnValue = 1;
    int knightValue = 3;
    int bishopValue = 3;
    int rookValue = 5;
    int queenValue = 9;
    // Evaluate king position
    int whiteKingRow = -1;
    int whiteKingCol = -1;
    int blackKingRow = -1;
    int blackKingCol = -1;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int piece = board[row][col];
            if (piece == WHITEPAWN) {
                score += pawnValue;
            } else if (piece == BLACKPAWN) {
                score -= pawnValue;
            } else if (piece == WHITEKNIGHT) {
                score += knightValue;
            } else if (piece == BLACKKNIGHT) {
                score -= knightValue;
            } else if (piece == WHITEBISHOP) {
                score += bishopValue;
            } else if (piece == BLACKBISHOP) {
                score -= bishopValue;
            } else if (piece == WHITEROOK) {
                score += rookValue;
            } else if (piece == BLACKROOK) {
                score -= rookValue;
            } else if (piece == WHITEQUEEN) {
                score += queenValue;
            } else if (piece == BLACKQUEEN) {
                score -= queenValue;
            } else if (piece == WHITEKING) {
                whiteKingRow = row;
            } else if (piece == BLACKKING) {
                blackKingRow = row;
            }
        }
    }
    // King safety and position weights
    int kingSafetyWeight = 1;
    int kingPositionWeight = 1;
    // Evaluate white king position
    if (blackKingRow > 0) {
        // Add positional bonuses for the white king
        score += kingPositionWeight * blackKingRow;
        // Check king safety and penalize if it's in check or checkmate
        if (isKingInCheck(blackKingCol, blackKingRow, board)) {
            if (isKingInCheckmate(blackKingCol, blackKingRow, board)) {
                score = INT16_MAX; // Checkmate, assign the lowest score possible
            } else {
                score += kingSafetyWeight; // King is in check, penalize the score
            }
        }
    }
    // Evaluate black king position
    if (whiteKingRow < 7) {
        // Add positional bonuses for the black king
        score -= kingPositionWeight * (7 - whiteKingRow);

        // Check king safety and penalize if it's in check or checkmate
        if (isKingInCheck(whiteKingCol, whiteKingRow, board)) {
            if (isKingInCheckmate(whiteKingCol, whiteKingRow, board)) {
                score = INT16_MIN; // Checkmate, assign the highest score possible
            } else {
                score -= kingSafetyWeight; // King is in check, penalize the score
            }
        }
    }
    return score;
}

void render(int board[8][8]) {
    window.clear(sf::Color::White);
    drawBoard();
    drawPieces(board);
    window.display();
}

static bool isPieceSelected = false;
bool isGameOver = false;
std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> endTime;

MovePiece miniMax(Node currentNode, int depth, bool maximizingPlayer, int board[8][8], int alpha, int beta) {
    if (std::chrono::steady_clock::now() > endTime) {
        MovePiece outOfTime;
        outOfTime.initRow = -69;
        return outOfTime;
    }
    if (depth == 0) { // Base case: reached maximum depth, evaluate the current position
        MovePiece move;
        move.score = getBoardEvaluation(board);
        return move;
    }
    if (maximizingPlayer) {
        currentNode.bestChild.score = INT16_MIN;
    } else {
        currentNode.bestChild.score = INT16_MAX;
    }
    for (int initRow = boardSize - 1; initRow >= 0; initRow--) {
        for (int initCol: favorCenter) {
            if ((currentNode.chessboard[initRow][initCol] < 0 && !currentNode.doesWhiteMoveFromHere) ||
                (currentNode.chessboard[initRow][initCol] > 0 && currentNode.doesWhiteMoveFromHere)) {
                // Current player's piece, can make a move
                for (int selectedRow = boardSize - 1; selectedRow >= 0; selectedRow--) {
                    for (int selectedCol: favorCenter) {
                        int isLegal = isMoveAllowed(initRow, initCol, selectedRow, selectedCol, board);
                        if (isLegal == 1) { // Move is legal
                            Node childNode = currentNode;  // Create a copy of the current node
                            MovePiece move;
                            move.initCol = initCol;
                            move.initRow = initRow;
                            move.newCol = selectedCol;
                            move.newRow = selectedRow;
                            childNode.moveToGetHere = move;
                            // Make the move on the copy of the chessboard
                            if (selectedRow == 0 && currentNode.chessboard[initRow][initCol] == WHITEPAWN) {
                                childNode.chessboard[selectedRow][selectedCol] = WHITEQUEEN;  // Promotion to queen
                            } else if (selectedRow == 7 && currentNode.chessboard[initRow][initCol] == BLACKPAWN) {
                                childNode.chessboard[selectedRow][selectedCol] = BLACKQUEEN;  // Promotion to queen
                            } else {
                                childNode.chessboard[selectedRow][selectedCol] = childNode.chessboard[initRow][initCol];
                            }
                            childNode.chessboard[initRow][initCol] = 0;
                            // Check if the move puts the opponent's king in check
                            bool isMoveAllowed = true;
                            for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                    if ((childNode.chessboard[tempRow][tempCol] == BLACKKING &&
                                         !currentNode.doesWhiteMoveFromHere) ||
                                        (childNode.chessboard[tempRow][tempCol] == WHITEKING &&
                                         currentNode.doesWhiteMoveFromHere)) {// Opponent's king
                                        if (isKingInCheck(tempCol, tempRow, childNode.chessboard)) {
                                            isMoveAllowed = false;
                                            break;
                                        }
                                    }
                                }
                                if (!isMoveAllowed) {
                                    break;
                                }
                            }
                            if (isMoveAllowed) {
                                childNode.doesWhiteMoveFromHere = !currentNode.doesWhiteMoveFromHere;
                                // Recursive call to evaluate the child node
                                MovePiece childMove = miniMax(childNode, depth - 1, !maximizingPlayer,
                                                              childNode.chessboard, alpha, beta);
                                if (childMove.initRow == -69) {
                                    return childMove;
                                }
                                // Update the best move based on the child's score
                                if ((maximizingPlayer && childMove.score > currentNode.bestChild.score) ||
                                    (!maximizingPlayer && childMove.score < currentNode.bestChild.score)) {
                                    currentNode.bestChild = move;
                                    currentNode.bestChild.score = childMove.score;
                                }
                                // Update alpha and beta
                                if (maximizingPlayer) {
                                    alpha = std::max(alpha, currentNode.bestChild.score);
                                } else {
                                    beta = std::min(beta, currentNode.bestChild.score);
                                }
                                // Alpha-beta pruning
                                if (beta <= alpha) {
                                    break;  // Beta cutoff
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return currentNode.bestChild;
}

MovePiece iterativeDeepening(Node currentNode, int board[8][8]) {
    int alpha = INT_MIN;
    int beta = INT_MAX;
    MovePiece bestMove;
    int depth = 1;
    startTime = std::chrono::steady_clock::now();
    endTime = startTime + std::chrono::seconds(5);
    while (std::chrono::steady_clock::now() < endTime) {
        MovePiece currentMove = miniMax(currentNode, depth, false, board, alpha, beta);
        if (currentMove.initRow != -69) {
            bestMove = currentMove;
        } else {
            int debug = 0;
        }
        // If the search is complete, exit the loop
        if (currentMove.score == INT16_MIN || currentMove.score == INT16_MAX) {
            break;
        }
        depth = depth + 2;
    }
    cout << "I explored to a depth of " << depth - 2 << " before I got impatient" << endl;
    return bestMove;
}

void handleEvents(int board[8][8]) {
    if (curTurn == 1) {
        static sf::Vector2i selectedPiecePosition;
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && !isGameOver) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int col = mousePosition.x / squareSize;
                    int row = mousePosition.y / squareSize;
                    if (!isPieceSelected) {
                        // First click, select the piece
                        if ((board[row][col] > 0 && curTurn == 1) ||
                            (board[row][col] < 0 && curTurn == 2)) {
                            isPieceSelected = true;
                            selectedPiecePosition = sf::Vector2i(col, row);
                            int pieceIndex = board[row][col];
                            if (pieceIndex < 0) {
                                pieceIndex = ((pieceIndex + 1) * -1) + 6;
                            } else {
                                pieceIndex = pieceIndex - 1;
                            }
                            pieceSprites[row][col].setTexture(highlightTextures[pieceIndex]);
                        }
                    } else {
                        // Second click, place the piece
                        int selectedCol = selectedPiecePosition.x;
                        int selectedRow = selectedPiecePosition.y;
                        int isLegal = isMoveAllowed(selectedRow, selectedCol, row, col, board);
                        if (isLegal == 1) {
                            int tempChessboard[boardSize][boardSize];
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    tempChessboard[i][j] = board[i][j];
                                }
                            }
                            if (row == 0 && board[selectedRow][selectedCol] == WHITEPAWN) { //promotion
                                board[row][col] = WHITEQUEEN;
                            } else {
                                board[row][col] = board[selectedRow][selectedCol];
                            }
                            board[selectedRow][selectedCol] = 0;
                            bool isMoveAllowed = true;
                            for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                    if ((curTurn == 1 && board[tempRow][tempCol] == 5) ||
                                        (curTurn == 2 && board[tempRow][tempCol] == -5)) {// king
                                        if (isKingInCheck(tempCol, tempRow, board)) {
                                            isMoveAllowed = false;
                                            tempRow = boardSize;
                                            tempCol = boardSize;
                                        }
                                    }

                                }
                            }
                            if (isMoveAllowed) {

                                pieceSprites[row][col] = pieceSprites[selectedRow][selectedCol];
                                pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
                                pieceSprites[row][col].setPosition(col * squareSize, row * squareSize);
                                int pieceIndex = board[row][col];
                                if (pieceIndex < 0) {
                                    pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                } else {
                                    pieceIndex = pieceIndex - 1;
                                }
                                pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
                                isPieceSelected = false;
                                curTurn = (curTurn % 2) + 1;
                                int isChecked = false;
                                if (board[row][col] == WHITEPAWN || board[row][col] == BLACKPAWN) {
                                    hundredMovesNoPawnMoveCounter = 0;
                                } else {
                                    hundredMovesNoPawnMoveCounter++;
                                }

                                if (isInStalemate(false, board) || hundredMovesNoPawnMoveCounter >= 100 ||
                                    isInsufficientMaterial(board)) {
                                    pieceSprites[boardSize][2].setTexture(pieceTextures[14]);
                                    window.draw(pieceSprites[boardSize][2]);
                                    render(board);
                                    isGameOver = true;
                                } else {
                                    for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                        for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                            if (board[tempRow][tempCol] == -5) {// king
                                                if (isKingInCheck(tempCol, tempRow, board)) {
                                                    if (isKingInCheckmate(tempCol, tempRow, board)) {
                                                        pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
                                                        window.draw(pieceSprites[boardSize][1]);
                                                        render(board);
                                                        isGameOver = true;
                                                    } else {
                                                        pieceSprites[boardSize][0].setTexture(pieceTextures[12]);
                                                        window.draw(pieceSprites[boardSize][0]);
                                                        window.display();
                                                        tempRow = boardSize;
                                                        tempCol = boardSize;
                                                        isChecked = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    window.setTitle("Black's Turn");
                                }
                                if (!isChecked) {
                                    pieceSprites[boardSize][0] = sf::Sprite();  // Clear the previous sprite
                                    pieceSprites[boardSize][0].setPosition(boardSize * squareSize, 0);
                                    render(board);
                                }
                            } else {
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        board[i][j] = tempChessboard[i][j];
                                    }
                                }
                            }
                        } else if (isLegal == -1) {
                            int pieceIndex = board[row][col];
                            if (pieceIndex < 0) {
                                pieceIndex = ((pieceIndex + 1) * -1) + 6;
                            } else {
                                pieceIndex = pieceIndex - 1;
                            }
                            pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
                            isPieceSelected = false;
                        }
                    }
                }
            }
        }
    } else if (!isGameOver) {
//        MovePiece greedyMove = playGreedyMove();

        Node rootNode;
        rootNode.doesWhiteMoveFromHere = curTurn == 1;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                rootNode.chessboard[i][j] = board[i][j];
            }
        }
        MovePiece bestMove = iterativeDeepening(rootNode, board);
        int selectedRow = bestMove.initRow;
        int selectedCol = bestMove.initCol;
        int row = bestMove.newRow;
        int col = bestMove.newCol;

        if (selectedRow == -1) {
            pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
            window.draw(pieceSprites[boardSize][1]);
            render(board);
            isGameOver = true;
        } else {
            if (row == 0 && board[selectedRow][selectedCol] == BLACKPAWN) { //promotion
                board[row][col] = BLACKQUEEN;
            } else {
                board[row][col] = board[selectedRow][selectedCol];
            }
            board[selectedRow][selectedCol] = 0;

            pieceSprites[row][col] = pieceSprites[selectedRow][selectedCol];
            pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
            pieceSprites[row][col].setPosition(col * squareSize, row * squareSize);
            int pieceIndex = board[row][col];
            if (pieceIndex < 0) {
                pieceIndex = ((pieceIndex + 1) * -1) + 6;
            } else {
                pieceIndex = pieceIndex - 1;
            }
            pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
            isPieceSelected = false;
            curTurn = (curTurn % 2) + 1;
            int isChecked = false;
            if (board[row][col] == WHITEPAWN || board[row][col] == BLACKPAWN) {
                hundredMovesNoPawnMoveCounter = 0;
            } else {
                hundredMovesNoPawnMoveCounter++;
            }
            if (isInStalemate(true, board) || hundredMovesNoPawnMoveCounter >= 100 || isInsufficientMaterial(board)) {
                pieceSprites[boardSize][2].setTexture(pieceTextures[14]);
                window.draw(pieceSprites[boardSize][2]);
                render(board);
                isGameOver = true;
            } else {
                for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                    for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                        if (board[tempRow][tempCol] == 5) {// king
                            if (isKingInCheck(tempCol, tempRow, board)) {
                                if (isKingInCheckmate(tempCol, tempRow, board)) {
                                    pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
                                    window.draw(pieceSprites[boardSize][1]);
                                    render(board);
                                    isGameOver = true;
                                } else {
                                    pieceSprites[boardSize][0].setTexture(pieceTextures[12]);
                                    window.draw(pieceSprites[boardSize][0]);
                                    window.display();
                                    tempRow = boardSize;
                                    tempCol = boardSize;
                                    isChecked = true;
                                }
                            }
                        }
                    }
                }
            }
            window.setTitle("White's Turn");
            if (!isChecked) {
                pieceSprites[boardSize][0] = sf::Sprite();  // Clear the previous sprite
                pieceSprites[boardSize][0].setPosition(boardSize * squareSize, 0);
                render(board);
            }
        }
    }
}

int main() {
    // Array to represent the chessboard
    int chessboard[boardSize][boardSize] = {
            {-1, -2, -3, -4, -5, -3, -2, -1},
            {-6, -6, -6, -6, -6, -6, -6, -6},
            {0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0},
            {6,  6,  6,  6,  6,  6,  6,  6},
            {1,  2,  3,  4,  5,  3,  2,  1}
    };
    loadTextures();
    createSprites(chessboard);
    while (window.isOpen()) {
        render(chessboard);
        if (!isGameOver) {
//            Node rootNode;
//            rootNode.doesWhiteMoveFromHere = curTurn == 1;
//            for (int i = 0; i < boardSize; i++) {
//                for (int j = 0; j < boardSize; j++) {
//                    rootNode.chessboard[i][j] = chessboard[i][j];
//                }
//            }
//            rootNode.depth = 0;
//            MovePiece bestMove = miniMax2(rootNode, 2, rootNode.doesWhiteMoveFromHere);
            handleEvents(chessboard);
        }
    }
    return 0;
}
