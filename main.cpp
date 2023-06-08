#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

sf::RenderWindow window(sf::VideoMode(1200, 1200), "White's Turn");
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
sf::Texture pieceTextures[12];
sf::Texture highlightTextures[12];
sf::Sprite pieceSprites[boardSize][boardSize];

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
}

void drawBoard() {
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
}


int isMoveAllowed(int firstRow, int firstCol, int secondRow, int secondCol) {
    int piece = chessboard[firstRow][firstCol];
    if (firstRow == secondRow && firstCol == secondCol) { //unselecting piece
        return 0;
    }
    if (piece == WHITEPAWN) {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow - 1 == secondRow && chessboard[secondRow][secondCol] == 0) {
            return 1;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 6 && secondRow == 4 && chessboard[4][secondCol] == 0 &&
            chessboard[5][secondCol] == 0) {
            return 1;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == 1 && chessboard[secondRow][secondCol] < 0) {
            return 1;
        }
    } else if (piece == BLACKPAWN) {
        // Check if the move is a one-step forward move
        if (firstCol == secondCol && firstRow + 1 == secondRow && chessboard[secondRow][secondCol] == 0) {
            return 1;
        }
        // Check if the move is a two-step forward move from the initial position (starting row)
        if (firstCol == secondCol && firstRow == 1 && secondRow == 3 && chessboard[3][secondCol] == 0 &&
            chessboard[2][secondCol] == 0) {
            return 1;
        }
        // Check if the move is a capture (diagonal move)
        if (std::abs(firstCol - secondCol) == 1 && firstRow - secondRow == -1 && chessboard[secondRow][secondCol] > 0) {
            return 1;
        }
    } else if (piece == WHITEROOK) {
        // Check if the move is a horizontal move
        if (chessboard[secondRow][secondCol] <= 0) {
            if (firstRow == secondRow && firstCol != secondCol) {
                // Check if there are any pieces blocking the horizontal path
                int step = (secondCol > firstCol) ? 1 : -1;
                for (int x = firstCol + step; x != secondCol; x += step) {
                    if (chessboard[firstRow][x] != 0) {
                        return -1;
                    }
                }
                return 1;
            }
            // Check if the move is a vertical move
            if (firstCol == secondCol && firstRow != secondRow) {
                // Check if there are any pieces blocking the vertical path
                int step = (secondRow > firstRow) ? 1 : -1;
                for (int y = firstRow + step; y != secondRow; y += step) {
                    if (chessboard[y][firstCol] != 0) {
                        return -1;
                    }
                }
                return 1;
            }
        }
        // The move is not legal for a white rook
        return -1;
    } else if (piece == BLACKROOK) {
        // Check if the move is a horizontal move
        if (chessboard[secondRow][secondCol] >= 0) {
            if (firstRow == secondRow && firstCol != secondCol) {
                // Check if there are any pieces blocking the horizontal path
                int step = (secondCol > firstCol) ? 1 : -1;
                for (int x = firstCol + step; x != secondCol; x += step) {
                    if (chessboard[firstRow][x] != 0) {
                        return -1;
                    }
                }
                return 1;
            }
            // Check if the move is a vertical move
            if (firstCol == secondCol && firstRow != secondRow) {
                // Check if there are any pieces blocking the vertical path
                int step = (secondRow > firstRow) ? 1 : -1;
                for (int y = firstRow + step; y != secondRow; y += step) {
                    if (chessboard[y][firstCol] != 0) {
                        return -1;
                    }
                }
                return 1;
            }
        }
        // The move is not legal for a black rook
        return -1;
    } else if (piece == WHITEBISHOP) {
        if (chessboard[secondRow][secondCol] <= 0) {
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
                        return -1;
                    }
                    x += xStep;
                    y += yStep;
                }
                return 1;
            }

            // The move is not legal for a white bishop
            return -1;
        }
    } else if (piece == BLACKBISHOP) {
        if (chessboard[secondRow][secondCol] >= 0) {
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
                        return -1;
                    }
                    x += xStep;
                    y += yStep;
                }
                return 1;
            }

            // The move is not legal for a black bishop
            return -1;
        }
    } else if (piece == WHITEKNIGHT) {
        if (chessboard[secondRow][secondCol] <= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);

            // Check if the move is an L-shaped knight move
            if ((diffX == 2 && diffY == 1) || (diffX == 1 && diffY == 2)) {
                return 1;
            }

            // The move is not legal for a white knight
            return -1;
        }
    } else if (piece == BLACKKNIGHT) {
        if (chessboard[secondRow][secondCol] >= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);

            // Check if the move is an L-shaped knight move
            if ((diffX == 2 && diffY == 1) || (diffX == 1 && diffY == 2)) {
                return 1;
            }

            // The move is not legal for a white knight
            return -1;
        }
    } else if (piece == WHITEQUEEN) {
        if (chessboard[secondRow][secondCol] <= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);

            // Check if the move is a valid queen move (horizontal, vertical, or diagonal)
            if ((firstCol == secondCol || firstRow == secondRow) || (diffX == diffY)) {
                // Check if there are any pieces blocking the path

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

            // The move is not legal for a white knight
            return -1;
        }
    } else if (piece == BLACKQUEEN) {
        if (chessboard[secondRow][secondCol] >= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);

            // Check if the move is a valid queen move (horizontal, vertical, or diagonal)
            if ((firstCol == secondCol || firstRow == secondRow) || (diffX == diffY)) {
                // Check if there are any pieces blocking the path

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

            // The move is not legal for a white knight
        }
        return -1;
    } else if (piece == WHITEKING) {
        if (chessboard[secondRow][secondCol] <= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);
            // Check if the move is a valid king move
            if (diffX <= 1 && diffY <= 1) {
                return true;
            }
        }
        return -1;
    } else if (piece == BLACKKING) {
        if (chessboard[secondRow][secondCol] >= 0) {
            // Calculate the absolute differences in row and column
            int diffX = std::abs(firstCol - secondCol);
            int diffY = std::abs(firstRow - secondRow);
            // Check if the move is a valid king move
            if (diffX <= 1 && diffY <= 1) {
                return true;
            }
        }
        return -1;
    }

    return -1;
}

void handleEvents() {
    static bool isPieceSelected = false;
    static sf::Vector2i selectedPiecePosition;

    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                int col = mousePosition.x / squareSize;
                int row = mousePosition.y / squareSize;
                    if (!isPieceSelected) {
                        // First click, select the piece
                        if ((chessboard[row][col] > 0 && curTurn == 1) || (chessboard[row][col] < 0 && curTurn == 2)) {
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
                            chessboard[row][col] = chessboard[selectedRow][selectedCol];
                            chessboard[selectedRow][selectedCol] = 0;
                            pieceSprites[row][col] = pieceSprites[selectedRow][selectedCol];
                            pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite

                            // Adjust the position of the moved sprite
                            pieceSprites[row][col].setPosition(col * squareSize, row * squareSize);


                            int pieceIndex = chessboard[row][col];
                            if (pieceIndex < 0) {
                                pieceIndex = ((pieceIndex + 1) * -1) + 6;
                            } else {
                                pieceIndex = pieceIndex - 1;
                            }
                            pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);

                            // Reset the selection state
                            isPieceSelected = false;
                            curTurn = (curTurn % 2) + 1;
                            if (curTurn == 1) {
                                window.setTitle("White's Turn");
                            } else {
                                window.setTitle("Black's Turn");
                            }
                        } else if (isLegal == 0) {
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
}


void render() {
    window.clear(sf::Color::White);
    drawBoard();
    drawPieces();
    window.display();
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
