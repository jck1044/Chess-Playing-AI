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

void render(int board[8][8]) {
    window.clear(sf::Color::White);
    drawBoard();
    drawPieces(board);
    window.display();
}