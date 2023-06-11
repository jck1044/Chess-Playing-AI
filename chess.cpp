#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>


struct MovePiece {
    int piece;
    int initRow = -1;
    int initCol = -1;
    int newRow = -1;
    int newCol = -1;
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

// Load chess piece textures
sf::Texture pieceTextures[15];
sf::Texture highlightTextures[12];
sf::Sprite pieceSprites[boardSize + 1][boardSize];
sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));

void loadTextures() {
    pieceTextures[0].loadFromFile("../piecePics/whiteRook.png");
    pieceTextures[1].loadFromFile("../piecePics/whiteKnight.png");
    pieceTextures[2].loadFromFile("../piecePics/whiteBishop.png");
    pieceTextures[3].loadFromFile("../piecePics/whiteQueen.png");
    pieceTextures[4].loadFromFile("../piecePics/whiteKing.png");
    pieceTextures[5].loadFromFile("../piecePics/whitePawn.png");
    pieceTextures[6].loadFromFile("../piecePics/blackRook.png");
    pieceTextures[7].loadFromFile("../piecePics/blackKnight.png");
    pieceTextures[8].loadFromFile("../piecePics/blackBishop.png");
    pieceTextures[9].loadFromFile("../piecePics/blackQueen.png");
    pieceTextures[10].loadFromFile("../piecePics/blackKing.png");
    pieceTextures[11].loadFromFile("../piecePics/blackPawn.png");
    pieceTextures[12].loadFromFile("../piecePics/inCheck.png");
    pieceTextures[13].loadFromFile("../piecePics/inCheckMate.png");
    pieceTextures[14].loadFromFile("../piecePics/draw.png");

    highlightTextures[0].loadFromFile("../piecePics/whiteRookHighlight.png");
    highlightTextures[1].loadFromFile("../piecePics/whiteKnightHighlight.png");
    highlightTextures[2].loadFromFile("../piecePics/whiteBishopHighlight.png");
    highlightTextures[3].loadFromFile("../piecePics/whiteQueenHighlight.png");
    highlightTextures[4].loadFromFile("../piecePics/whiteKingHighlight.png");
    highlightTextures[5].loadFromFile("../piecePics/whitePawnHighlight.png");
    highlightTextures[6].loadFromFile("../piecePics/blackRookHighlight.png");
    highlightTextures[7].loadFromFile("../piecePics/blackKnightHighlight.png");
    highlightTextures[8].loadFromFile("../piecePics/blackBishopHighlight.png");
    highlightTextures[9].loadFromFile("../piecePics/blackQueenHighlight.png");
    highlightTextures[10].loadFromFile("../piecePics/blackKingHighlight.png");
    highlightTextures[11].loadFromFile("../piecePics/blackPawnHighlight.png");
}

void createSprites() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (chessboard[i][j] != 0) {
                int pieceIndex = chessboard[i][j];
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

void drawPieces() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (chessboard[i][j] != 0) {
                window.draw(pieceSprites[i][j]);
            }
        }
    }
    window.draw(pieceSprites[boardSize][0]);
    window.draw(pieceSprites[boardSize][1]);
    window.draw(pieceSprites[boardSize][2]);
}

int getBoardEvaluation() {
    int score = 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (chessboard[row][col] == WHITEPAWN) {
                score++;
            } else if (chessboard[row][col] == BLACKPAWN) {
                score--;
            } else if (chessboard[row][col] == WHITEKNIGHT || chessboard[row][col] == WHITEBISHOP) {
                score = score + 3;
            } else if (chessboard[row][col] == BLACKKNIGHT || chessboard[row][col] == BLACKBISHOP) {
                score = score - 3;
            } else if (chessboard[row][col] == WHITEROOK) {
                score = score + 5;
            } else if (chessboard[row][col] == BLACKROOK) {
                score = score - 5;
            } else if (chessboard[row][col] == WHITEQUEEN) {
                score = score + 9;
            } else if (chessboard[row][col] == BLACKQUEEN) {
                score = score - 9;
            }
        }
    }
    return score;
}

bool isRookMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    // Check if the move is a horizontal move
    if ((isWhite && chessboard[secondRow][secondCol] <= 0) ||
        (!isWhite && chessboard[secondRow][secondCol] >= 0)) {
        if (firstRow == secondRow && firstCol != secondCol) {
            // Check if there are any pieces blocking the horizontal path
            int step = (secondCol > firstCol) ? 1 : -1;
            for (int x = firstCol + step; x != secondCol; x += step) {
                if (chessboard[firstRow][x] != 0) {
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
                if (chessboard[y][firstCol] != 0) {
                    return false;
                }
            }
            return true;
        }
    }
    // The move is not legal for a black rook
    return false;
}

bool isBishopMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    if ((isWhite && chessboard[secondRow][secondCol] <= 0) ||
        (!isWhite && chessboard[secondRow][secondCol] >= 0)) {
        // Check if the move is a diagonal move
        if (std::abs(firstCol - secondCol) == std::abs(firstRow - secondRow)) {
            // Determine the direction of the diagonal move
            int xStep = (secondCol > firstCol) ? 1 : -1;
            int yStep = (secondRow > firstRow) ? 1 : -1;
            // Check if there are any pieces blocking the diagonal path
            int x = firstCol + xStep;
            int y = firstRow + yStep;
            while (x != secondCol && y != secondRow) {
                if (chessboard[y][x] != 0) {
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

bool isKnightMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    if ((isWhite && chessboard[secondRow][secondCol] <= 0) ||
        (!isWhite && chessboard[secondRow][secondCol] >= 0)) {
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

bool isQueenMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    if ((isWhite && chessboard[secondRow][secondCol] <= 0) ||
        (!isWhite && chessboard[secondRow][secondCol] >= 0)) {
        // Calculate the absolute differences in row and column
        int diffX = std::abs(firstCol - secondCol);
        int diffY = std::abs(firstRow - secondRow);
        // Check if the move is a valid queen move (horizontal, vertical, or diagonal)
        if ((firstCol == secondCol || firstRow == secondRow) || (diffX == diffY)) {
            // Horizontal move
            if (firstCol == secondCol && firstRow != secondRow) {
                int step = (secondRow > firstRow) ? 1 : -1;
                for (int y = firstRow + step; y != secondRow; y += step) {
                    if (chessboard[y][firstCol] != 0) {
                        return false;
                    }
                }
            }
                // Vertical move
            else if (firstRow == secondRow && firstCol != secondCol) {
                int step = (secondCol > firstCol) ? 1 : -1;
                for (int x = firstCol + step; x != secondCol; x += step) {
                    if (chessboard[firstRow][x] != 0) {
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
                    if (chessboard[y][x] != 0) {
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

bool isKingMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    if ((isWhite && chessboard[secondRow][secondCol] <= 0) ||
        (!isWhite && chessboard[secondRow][secondCol] >= 0)) {
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

bool isPawnMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite) {
    if (isWhite) {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow - 1 == secondRow && chessboard[secondRow][secondCol] == 0) {
            return true;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 6 && secondRow == 4 && chessboard[4][secondCol] == 0 &&
            chessboard[5][secondCol] == 0) {
            return true;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == 1 &&
            chessboard[secondRow][secondCol] < 0) {
            return true;
        }
    } else {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow + 1 == secondRow && chessboard[secondRow][secondCol] == 0) {
            return true;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 1 && secondRow == 3 && chessboard[3][secondCol] == 0 &&
            chessboard[2][secondCol] == 0) {
            return true;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == -1 &&
            chessboard[secondRow][secondCol] > 0) {
            return true;
        }
    }
    return false;
}

int isMoveAllowed(int firstRow, int firstCol, int secondRow, int secondCol) {
    int piece = chessboard[firstRow][firstCol];
    if (firstRow == secondRow && firstCol == secondCol) { //unselecting piece
        return -1;
    } else if (piece == WHITEPAWN) {
        return isPawnMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKPAWN) {
        return isPawnMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    } else if (piece == WHITEROOK) {
        return isRookMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKROOK) {
        return isRookMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    } else if (piece == WHITEBISHOP) {
        return isBishopMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKBISHOP) {
        return isBishopMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    } else if (piece == WHITEKNIGHT) {
        return isKnightMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKKNIGHT) {
        return isKnightMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    } else if (piece == WHITEQUEEN) {
        return isQueenMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKQUEEN) {
        return isQueenMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    } else if (piece == WHITEKING) {
        return isKingMoveValid(firstCol, firstRow, secondCol, secondRow, true);
    } else if (piece == BLACKKING) {
        return isKingMoveValid(firstCol, firstRow, secondCol, secondRow, false);
    }
    return false;
}

bool isKingInCheck(int kingX, int kingY) {
    bool isWhiteKing = chessboard[kingY][kingX] > 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (row == 1 && col == 5 && !isWhiteKing) {
                int debug = 0;
            }
            if ((isWhiteKing && chessboard[row][col] < 0) || (!isWhiteKing && chessboard[row][col] > 0)) {
                if (isMoveAllowed(row, col, kingY, kingX)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isInStalemate(bool isWhite) {
    bool isPossibleMove = false;
    for (int initRow = 0; initRow < boardSize; initRow++) {
        for (int initCol = 0; initCol < boardSize; initCol++) {
            if ((isWhite && chessboard[initRow][initCol] > 0) ||
                (!isWhite && chessboard[initRow][initCol] < 0)) {
                if (isWhite && chessboard[initRow][initCol] == WHITEKING ||
                    !isWhite && chessboard[initRow][initCol] == BLACKKING) {
                    if (isKingInCheck(initCol, initRow)) {
                        return false;
                    }
                }
                for (int newRow = 0; newRow < boardSize; newRow++) {
                    for (int newCol = 0; newCol < boardSize; newCol++) {
                        if (initRow != newRow || initCol != newCol) {
                            if (initCol == 0 && newCol == 0 && initRow == 0 && newRow == 1) {
                                int debug = 0;
                            }
                            if (isMoveAllowed(initRow, initCol, newRow, newCol)) {
                                int tempChessboard[boardSize][boardSize];
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        tempChessboard[i][j] = chessboard[i][j];
                                    }
                                }
                                chessboard[newRow][newCol] = chessboard[initRow][initCol];
                                chessboard[initRow][initCol] = 0;
                                if (isWhite && chessboard[newRow][newCol] == WHITEKING ||
                                    !isWhite && chessboard[newRow][newCol] == BLACKKING) {
                                    if (!isKingInCheck(newCol, newRow)) {
                                        for (int i = 0; i < boardSize; i++) {
                                            for (int j = 0; j < boardSize; j++) {
                                                chessboard[i][j] = tempChessboard[i][j];
                                            }
                                        }
                                        return false;
                                    }
                                } else {
                                    for (int i = 0; i < boardSize; i++) {
                                        for (int j = 0; j < boardSize; j++) {
                                            chessboard[i][j] = tempChessboard[i][j];
                                        }
                                    }
                                    return false;
                                }
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        chessboard[i][j] = tempChessboard[i][j];
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

bool isInsufficientMaterial() {
    int numWhiteBishops = 0;
    int numWhiteKnights = 0;
    int numBlackBishops = 0;
    int numBlackKnights = 0;
    for (auto &row: chessboard) {
        for (int col: row) {
            if (col == WHITEPAWN || col == BLACKPAWN || col == WHITEQUEEN || col == BLACKQUEEN || col == WHITEROOK ||
                col == BLACKROOK) {
                return false;
            }
            if (col == WHITEBISHOP) {
                numWhiteBishops++;
                if (numWhiteBishops > 1) {
                    return false;
                }
            } else if (col == WHITEKNIGHT) {
                numWhiteKnights++;
                if (numWhiteKnights > 1) {
                    return false;
                }
            } else if (col == BLACKBISHOP) {
                numBlackBishops++;
                if (numBlackBishops > 1) {
                    return false;
                }
            } else if (col == BLACKKNIGHT) {
                numBlackKnights++;
                if (numBlackKnights > 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool canKingEscapeCheck(int kingX, int kingY) {
    // Check all possible king moves
    int kingMoves[8][2] = {{1,  0},
                           {1,  1},
                           {0,  1},
                           {-1, 1},
                           {-1, 0},
                           {-1, -1},
                           {0,  -1},
                           {1,  -1}};
    for (int i = 0; i < 8; i++) {
        int newX = kingX + kingMoves[i][0];
        int newY = kingY + kingMoves[i][1];
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            // Check if the new position is not under threat
            if (chessboard[newY][newX] == 0) {
                int tempChessboard[boardSize][boardSize];
                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        tempChessboard[i][j] = chessboard[i][j];
                    }
                }
                chessboard[newY][newX] = chessboard[kingY][kingX];
                chessboard[kingY][kingX] = 0;
                if (!isKingInCheck(newX, newY)) {
                    for (int i = 0; i < boardSize; i++) {
                        for (int j = 0; j < boardSize; j++) {
                            chessboard[i][j] = tempChessboard[i][j];
                        }
                    }
                    return true;
                }
                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        chessboard[i][j] = tempChessboard[i][j];
                    }
                }
            }
        }
    }
    // King cannot escape check by moving
    return false;
}

bool isKingInCheckmate(int kingX, int kingY) {
    bool isKingWhite = chessboard[kingY][kingX] > 0;
    // Check if the king is in check
    if (!isKingInCheck(kingX, kingY)) {
        return false;
    }
    // Check if the king can escape the check
    if (canKingEscapeCheck(kingX, kingY)) {
        return false;
    }
    // Check if any piece can block or capture the attacking piece
    int tempChessboard[boardSize][boardSize];
    // Copy values from chessboard to tempChessboard
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            tempChessboard[i][j] = chessboard[i][j];
        }
    }
    for (int initRow = 0; initRow < boardSize; initRow++) {
        for (int initCol = 0; initCol < boardSize; initCol++) {
            if ((isKingWhite && chessboard[initRow][initCol] > 0) ||
                (!isKingWhite && chessboard[initRow][initCol] < 0)) {
                for (int newRow = 0; newRow < boardSize; newRow++) {
                    for (int newCol = 0; newCol < boardSize; newCol++) {
                        if ((initRow != newRow || initCol != newCol) &&
                            isMoveAllowed(initRow, initCol, newRow, newCol)) {
                            chessboard[newRow][newCol] = chessboard[initRow][initCol];
                            chessboard[initRow][initCol] = 0;
                            if (!isKingInCheck(kingX, kingY)) {
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        chessboard[i][j] = tempChessboard[i][j];
                                    }
                                }
                                return false;
                            }
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    chessboard[i][j] = tempChessboard[i][j];
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

void render() {
    window.clear(sf::Color::White);
    drawBoard();
    drawPieces();
    window.display();
}

static bool isPieceSelected = false;
bool isGameOver = false;

MovePiece playGreedyMove() {
    MovePiece bestMove{};
    int bestScore = INT16_MAX;
    for (int initRow = boardSize - 1; initRow >= 0; initRow--) {
        for (int initCol: favorCenter) {
            if (chessboard[initRow][initCol] < 0) {
                for (int selectedRow = boardSize - 1; selectedRow >= 0; selectedRow--) {
                    for (int selectedCol: favorCenter) {
                        int isLegal = isMoveAllowed(initRow, initCol, selectedRow, selectedCol);
                        if (isLegal == 1) {
                            int tempChessboard[boardSize][boardSize];
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    tempChessboard[i][j] = chessboard[i][j];
                                }
                            }
                            if (selectedRow == 0 && chessboard[selectedRow][selectedCol] == BLACKPAWN) { //promotion
                                chessboard[selectedRow][selectedCol] = BLACKQUEEN;
                            } else {
                                chessboard[selectedRow][selectedCol] = chessboard[initRow][initCol];
                            }
                            chessboard[initRow][initCol] = 0;
                            bool isMoveAllowed = true;
                            for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                    if ((curTurn == 1 && chessboard[tempRow][tempCol] == 5) ||
                                        (curTurn == 2 && chessboard[tempRow][tempCol] == -5)) {// king
                                        if (isKingInCheck(tempCol, tempRow)) {
                                            isMoveAllowed = false;
                                            tempRow = boardSize;
                                            tempCol = boardSize;
                                        }
                                    }

                                }
                            }
                            if (isMoveAllowed) {
                                if (initRow == 3 && initCol == 7 && selectedRow == 4 && selectedCol == 6) {
                                    int debug = 0;
                                }
                                int score = getBoardEvaluation();
                                for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                    for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                        if (chessboard[tempRow][tempCol] == 5) {// king
                                            if (isKingInCheck(tempCol, tempRow)) {
                                                if (isKingInCheckmate(tempCol, tempRow)) {
                                                    score = INT16_MIN;
                                                } else {
                                                    score++;
                                                }
                                            }
                                        }
                                    }
                                }
                                if (score < bestScore) {
                                    bestMove.initRow = initRow;
                                    bestMove.initCol = initCol;
                                    bestMove.newRow = selectedRow;
                                    bestMove.newCol = selectedCol;
                                    bestScore = score;
                                }
                            }
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    chessboard[i][j] = tempChessboard[i][j];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return bestMove;
}


void handleEvents() {
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
                        if ((chessboard[row][col] > 0 && curTurn == 1) ||
                            (chessboard[row][col] < 0 && curTurn == 2)) {
                            isPieceSelected = true;
                            selectedPiecePosition = sf::Vector2i(col, row);
                            int pieceIndex = chessboard[row][col];
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
                        int isLegal = isMoveAllowed(selectedRow, selectedCol, row, col);
                        if (isLegal == 1) {
                            int tempChessboard[boardSize][boardSize];
                            for (int i = 0; i < boardSize; i++) {
                                for (int j = 0; j < boardSize; j++) {
                                    tempChessboard[i][j] = chessboard[i][j];
                                }
                            }
                            if (row == 0 && chessboard[selectedRow][selectedCol] == WHITEPAWN) { //promotion
                                chessboard[row][col] = WHITEQUEEN;
                            } else {
                                chessboard[row][col] = chessboard[selectedRow][selectedCol];
                            }
                            chessboard[selectedRow][selectedCol] = 0;
                            bool isMoveAllowed = true;
                            for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                    if ((curTurn == 1 && chessboard[tempRow][tempCol] == 5) ||
                                        (curTurn == 2 && chessboard[tempRow][tempCol] == -5)) {// king
                                        if (isKingInCheck(tempCol, tempRow)) {
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
                                int pieceIndex = chessboard[row][col];
                                if (pieceIndex < 0) {
                                    pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                } else {
                                    pieceIndex = pieceIndex - 1;
                                }
                                pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
                                isPieceSelected = false;
                                curTurn = (curTurn % 2) + 1;
                                int isChecked = false;
                                if (chessboard[row][col] == WHITEPAWN || chessboard[row][col] == BLACKPAWN) {
                                    hundredMovesNoPawnMoveCounter = 0;
                                } else {
                                    hundredMovesNoPawnMoveCounter++;
                                }

                                if (isInStalemate(false) || hundredMovesNoPawnMoveCounter >= 100 ||
                                    isInsufficientMaterial()) {
                                    pieceSprites[boardSize][2].setTexture(pieceTextures[14]);
                                    window.draw(pieceSprites[boardSize][2]);
                                    render();
                                    isGameOver = true;
                                } else {
                                    for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                                        for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                                            if (chessboard[tempRow][tempCol] == -5) {// king
                                                if (isKingInCheck(tempCol, tempRow)) {
                                                    if (isKingInCheckmate(tempCol, tempRow)) {
                                                        pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
                                                        window.draw(pieceSprites[boardSize][1]);
                                                        render();
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
                                    render();
                                }
                            } else {
                                for (int i = 0; i < boardSize; i++) {
                                    for (int j = 0; j < boardSize; j++) {
                                        chessboard[i][j] = tempChessboard[i][j];
                                    }
                                }
                            }
                        } else if (isLegal == -1) {
                            int pieceIndex = chessboard[row][col];
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
        MovePiece greedyMove = playGreedyMove();
        int selectedRow = greedyMove.initRow;
        int selectedCol = greedyMove.initCol;
        int row = greedyMove.newRow;
        int col = greedyMove.newCol;

        if (selectedRow == -1) {
            pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
            window.draw(pieceSprites[boardSize][1]);
            render();
            isGameOver = true;
        } else {
            if (row == 0 && chessboard[selectedRow][selectedCol] == BLACKPAWN) { //promotion
                chessboard[row][col] = BLACKQUEEN;
            } else {
                chessboard[row][col] = chessboard[selectedRow][selectedCol];
            }
            chessboard[selectedRow][selectedCol] = 0;

            pieceSprites[row][col] = pieceSprites[selectedRow][selectedCol];
            pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
            pieceSprites[row][col].setPosition(col * squareSize, row * squareSize);
            int pieceIndex = chessboard[row][col];
            if (pieceIndex < 0) {
                pieceIndex = ((pieceIndex + 1) * -1) + 6;
            } else {
                pieceIndex = pieceIndex - 1;
            }
            pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
            isPieceSelected = false;
            curTurn = (curTurn % 2) + 1;
            int isChecked = false;
            if (chessboard[row][col] == WHITEPAWN || chessboard[row][col] == BLACKPAWN) {
                hundredMovesNoPawnMoveCounter = 0;
            } else {
                hundredMovesNoPawnMoveCounter++;
            }
            if (isInStalemate(true) || hundredMovesNoPawnMoveCounter >= 100 ||
                isInsufficientMaterial()) {
                pieceSprites[boardSize][2].setTexture(pieceTextures[14]);
                window.draw(pieceSprites[boardSize][2]);
                render();
                isGameOver = true;
            } else {
                for (int tempRow = 0; tempRow < boardSize; tempRow++) {
                    for (int tempCol = 0; tempCol < boardSize; tempCol++) {
                        if (chessboard[tempRow][tempCol] == 5) {// king
                            if (isKingInCheck(tempCol, tempRow)) {
                                if (isKingInCheckmate(tempCol, tempRow)) {
                                    pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
                                    window.draw(pieceSprites[boardSize][1]);
                                    render();
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
                render();
            }
        }
    }
}

int main() {
    loadTextures();
    createSprites();
    while (window.isOpen()) {
        handleEvents();
        render();
    }
    return 0;
}
