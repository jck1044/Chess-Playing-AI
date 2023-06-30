#include "chess.h"
#include "gui.cpp"
#include "gameplay.cpp"
int getBoardEvaluation(int board[8][8]) {
    double score = 0;
    double pieceScore = 0;
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

    int numWhiteOnCol0 = 0;
    int numWhiteOnCol1 = 0;
    int numWhiteOnCol2 = 0;
    int numWhiteOnCol3 = 0;
    int numWhiteOnCol4 = 0;
    int numWhiteOnCol5 = 0;
    int numWhiteOnCol6 = 0;
    int numWhiteOnCol7 = 0;
    int numBlackOnCol0 = 0;
    int numBlackOnCol1 = 0;
    int numBlackOnCol2 = 0;
    int numBlackOnCol3 = 0;
    int numBlackOnCol4 = 0;
    int numBlackOnCol5 = 0;
    int numBlackOnCol6 = 0;
    int numBlackOnCol7 = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int piece = board[row][col];
            if (piece == WHITEPAWN) {
                if (col == 0) {
                    numWhiteOnCol0++;
                    if (numWhiteOnCol0 > 1) {
                        score -= .2;
                    }
                } else if (col == 1) {
                    numWhiteOnCol1++;
                    if (numWhiteOnCol1 > 1) {
                        score -= .2;
                    }
                } else if (col == 2) {
                    numWhiteOnCol2++;
                    if (numWhiteOnCol2 > 1) {
                        score -= .2;
                    }
                } else if (col == 3) {
                    if (numWhiteOnCol3 > 1) {
                        score -= .2;
                    }
                    numWhiteOnCol3++;
                } else if (col == 4) {
                    if (numWhiteOnCol4 > 1) {
                        score -= .2;
                    }
                    numWhiteOnCol4++;
                } else if (col == 5) {
                    if (numWhiteOnCol5 > 1) {
                        score -= .2;
                    }
                    numWhiteOnCol5++;
                } else if (col == 6) {
                    if (numWhiteOnCol6 > 1) {
                        score -= .2;
                    }
                    numWhiteOnCol6++;
                } else if (col == 7) {
                    if (numWhiteOnCol7 > 1) {
                        score -= .2;
                    }
                    numWhiteOnCol7++;
                }
                if (row == 6) {
                    score -= .25;
                }
                pieceScore += pawnValue;
            } else if (piece == BLACKPAWN) {
                if (col == 0) {
                    numBlackOnCol0++;
                    if (numBlackOnCol0 > 1) {
                        score -= .2;
                    }
                } else if (col == 1) {
                    numBlackOnCol1++;
                    if (numBlackOnCol1 > 1) {
                        score -= .2;
                    }
                } else if (col == 2) {
                    numBlackOnCol2++;
                    if (numBlackOnCol2 > 1) {
                        score -= .2;
                    }
                } else if (col == 3) {
                    if (numBlackOnCol3 > 1) {
                        score -= .2;
                    }
                    numBlackOnCol3++;
                } else if (col == 4) {
                    if (numBlackOnCol4 > 1) {
                        score -= .2;
                    }
                    numBlackOnCol4++;
                } else if (col == 5) {
                    if (numBlackOnCol5 > 1) {
                        score -= .2;
                    }
                    numBlackOnCol5++;
                } else if (col == 6) {
                    if (numBlackOnCol6 > 1) {
                        score -= .2;
                    }
                    numBlackOnCol6++;
                } else if (col == 7) {
                    if (numBlackOnCol7 > 1) {
                        score -= .2;
                    }
                    numBlackOnCol7++;
                }
                if (row == 1) {
                    score += .25;
                }
                pieceScore -= pawnValue;
            } else if (piece == WHITEKNIGHT) {
                if (row == 7 && (col == 1 || col == 6)) {
                    score -= .25;
                }
                pieceScore += knightValue;
            } else if (piece == BLACKKNIGHT) {
                if (row == 0 && (col == 1 || col == 6)) {
                    score += .25;
                }
                pieceScore -= knightValue;
            } else if (piece == WHITEBISHOP) {
                if (row == 7 && (col == 2 || col == 5)) {
                    score -= .25;
                }
                pieceScore += bishopValue;
            } else if (piece == BLACKBISHOP) {
                if (row == 0 && (col == 2 || col == 5)) {
                    score += .25;
                }
                pieceScore -= bishopValue;
            } else if (piece == WHITEROOK) {
                if (row == 7 && (col == 0 || col == 7)) {
                    score -= .25;
                }
                pieceScore += rookValue;
            } else if (piece == BLACKROOK) {
                if (row == 0 && (col == 0 || col == 7)) {
                    score += .25;
                }
                pieceScore -= rookValue;
            } else if (piece == WHITEQUEEN) {
                pieceScore += queenValue;
            } else if (piece == BLACKQUEEN) {
                pieceScore -= queenValue;
            } else if (piece == WHITEKING) {
                whiteKingRow = row;
                whiteKingCol = col;
            } else if (piece == BLACKKING) {
                blackKingRow = row;
                blackKingCol = col;
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
    score = score + (pieceScore * 2);
    if (isDraw(true, board) || isDraw(false, board)) {
        score = score * -1;
    }
    int roundedScore = round(score);
    return roundedScore;
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
//                            childNode.moveToGetHere = move;
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
    int depth = 2;
    startTime = std::chrono::steady_clock::now();
    endTime = startTime + std::chrono::seconds(180);
    while (std::chrono::steady_clock::now() < endTime) {
        MovePiece currentMove = miniMax(currentNode, depth, false, board, alpha, beta);
        if (currentMove.initRow != -69) {
            // If the search is complete, exit the loop
            if (currentMove.score == INT16_MIN || currentMove.score == INT16_MAX) {
                break;
            }
            bestMove = currentMove;
            auto curTime = std::chrono::steady_clock::now();
            auto duration = curTime - startTime;
            cout << "So far I have reached depth " << depth << " in "
                 << (std::chrono::duration<double>(duration).count())
                 << " seconds" << endl;
            cout << "bestMove score: " << bestMove.score << endl;
            cout << bestMove.initRow << "," << bestMove.initCol << " to " << bestMove.newRow << "," << bestMove.newCol
                 << endl;
            depth = depth + 2;
            if (depth > 4) {
                break;
            }
        }
    }
    cout << "I explored to a depth of " << (depth - 2) << " before I got impatient" << endl;
    return bestMove;
}

void handleEvents(int board[8][8]) {
    render(board);
    if (curTurn == 1) {
//        Node rootNode;
//        rootNode.doesWhiteMoveFromHere = curTurn == 1;
//        for (int i = 0; i < boardSize; i++) {
//            for (int j = 0; j < boardSize; j++) {
//                rootNode.chessboard[i][j] = board[i][j];
//            }
//        }
//        MovePiece bestMove = iterativeDeepening(rootNode, board);
//        int selectedRow = bestMove.initRow;
//        int selectedCol = bestMove.initCol;
//        int row = bestMove.newRow;
//        int col = bestMove.newCol;
//
//        if (selectedRow == -1) {
//            pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
//            window.draw(pieceSprites[boardSize][1]);
//            render(board);
//            isGameOver = true;
//        } else {
//            if (row == 7 && board[selectedRow][selectedCol] == WHITEPAWN) { //promotion
//                board[row][col] = WHITEQUEEN;
//            } else {
//                board[row][col] = board[selectedRow][selectedCol];
//            }
//            board[selectedRow][selectedCol] = 0;
//
//            pieceSprites[row][col] = pieceSprites[selectedRow][selectedCol];
//            pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
//            pieceSprites[row][col].setPosition(col * squareSize, row * squareSize);
//            int pieceIndex = board[row][col];
//            if (pieceIndex < 0) {
//                pieceIndex = ((pieceIndex + 1) * -1) + 6;
//            } else {
//                pieceIndex = pieceIndex - 1;
//            }
//            pieceSprites[row][col].setTexture(pieceTextures[pieceIndex]);
//            isPieceSelected = false;
//            curTurn = (curTurn % 2) + 1;
//            int isChecked = false;
//            if (board[row][col] == WHITEPAWN || board[row][col] == BLACKPAWN) {
//                hundredMovesNoPawnMoveCounter = 0;
//            } else {
//                hundredMovesNoPawnMoveCounter++;
//            }
//            if (isInStalemate(true, board) || hundredMovesNoPawnMoveCounter >= 100 || isInsufficientMaterial(board)) {
//                pieceSprites[boardSize][2].setTexture(pieceTextures[14]);
//                window.draw(pieceSprites[boardSize][2]);
//                render(board);
//                isGameOver = true;
//            } else {
//                for (int tempRow = 0; tempRow < boardSize; tempRow++) {
//                    for (int tempCol = 0; tempCol < boardSize; tempCol++) {
//                        if (board[tempRow][tempCol] == -5) {// king
//                            if (isKingInCheck(tempCol, tempRow, board)) {
//                                if (isKingInCheckmate(tempCol, tempRow, board)) {
//                                    pieceSprites[boardSize][1].setTexture(pieceTextures[13]);
//                                    window.draw(pieceSprites[boardSize][1]);
//                                    render(board);
//                                    isGameOver = true;
//                                } else {
//                                    pieceSprites[boardSize][0].setTexture(pieceTextures[12]);
//                                    window.draw(pieceSprites[boardSize][0]);
//                                    window.display();
//                                    tempRow = boardSize;
//                                    tempCol = boardSize;
//                                    isChecked = true;
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            window.setTitle("Black's Turn");
//            if (!isChecked) {
//                pieceSprites[boardSize][0] = sf::Sprite();  // Clear the previous sprite
//                pieceSprites[boardSize][0].setPosition(boardSize * squareSize, 0);
//                render(board);
//            }
//        }
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
                            bool isCastle = false;
                            if (row == 0 && board[selectedRow][selectedCol] == WHITEPAWN) { //promotion
                                board[row][col] = WHITEQUEEN;
                            } else if (board[selectedRow][selectedCol] == WHITEROOK &&
                                       board[row][col] == WHITEKING) { //castle
                                isCastle = true;
                                board[row][col] = 0;
                                if (selectedCol == 0) { //long
                                    board[selectedRow][2] = WHITEKING;
                                    board[selectedRow][3] = WHITEROOK;
                                } else { //short
                                    board[selectedRow][6] = WHITEKING;
                                    board[selectedRow][5] = WHITEROOK;
                                }
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
                                if (isCastle) {
                                    if (selectedCol == 0) { //long
                                        pieceSprites[selectedRow][2] = pieceSprites[selectedRow][selectedCol];
                                        pieceSprites[selectedRow][3] = pieceSprites[row][col];
                                        pieceSprites[selectedRow][2].setPosition(2 * squareSize,
                                                                                 selectedRow * squareSize);
                                        pieceSprites[selectedRow][3].setPosition(3 * squareSize,
                                                                                 selectedRow * squareSize);
                                        int pieceIndex = board[selectedRow][2];
                                        if (pieceIndex < 0) {
                                            pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                        } else {
                                            pieceIndex = pieceIndex - 1;
                                        }
                                        pieceSprites[selectedRow][2].setTexture(pieceTextures[pieceIndex]);
                                        pieceIndex = board[selectedRow][3];
                                        if (pieceIndex < 0) {
                                            pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                        } else {
                                            pieceIndex = pieceIndex - 1;
                                        }
                                        pieceSprites[selectedRow][3].setTexture(pieceTextures[pieceIndex]);
                                    } else { //short
                                        pieceSprites[selectedRow][6] = pieceSprites[selectedRow][selectedCol];
                                        pieceSprites[selectedRow][5] = pieceSprites[row][col];
                                        pieceSprites[selectedRow][6].setPosition(6 * squareSize,
                                                                                 selectedRow * squareSize);
                                        pieceSprites[selectedRow][5].setPosition(5 * squareSize,
                                                                                 selectedRow * squareSize);
                                        int pieceIndex = board[selectedRow][6];
                                        if (pieceIndex < 0) {
                                            pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                        } else {
                                            pieceIndex = pieceIndex - 1;
                                        }
                                        pieceSprites[selectedRow][6].setTexture(pieceTextures[pieceIndex]);
                                        pieceIndex = board[selectedRow][5];
                                        if (pieceIndex < 0) {
                                            pieceIndex = ((pieceIndex + 1) * -1) + 6;
                                        } else {
                                            pieceIndex = pieceIndex - 1;
                                        }
                                        pieceSprites[selectedRow][5].setTexture(pieceTextures[pieceIndex]);
                                    }
                                    pieceSprites[row][col] = sf::Sprite();  // Clear the previous sprite
                                    pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
                                } else {
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
                                }
                                isPieceSelected = false;
                                curTurn = (curTurn % 2) + 1;
                                int isChecked = false;
                                if (board[row][col] == WHITEPAWN || board[row][col] == BLACKPAWN) {
                                    hundredMovesNoPawnMoveCounter = 0;
                                } else {
                                    hundredMovesNoPawnMoveCounter++;
                                }

                                if (isDraw(false, board)) {
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
                    cout << "--- CURRENT BOARD STATE ---" << endl;
                    for (int i = 0; i < boardSize; i++) {
                        cout << "{";
                        for (int j = 0; j < boardSize; j++) {
                            if (j == 7) {
                                cout << board[i][j] << "}," << endl;
                            } else {
                                cout << board[i][j] << ", ";
                            }
                        }
                    }
                    cout << endl;
                }
            }
        }
    } else if (!isGameOver) {
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
            bool isCastle = false;
            if (row == 0 && board[selectedRow][selectedCol] == BLACKPAWN) { //promotion
                board[row][col] = BLACKQUEEN;
            } else if (board[selectedRow][selectedCol] == BLACKROOK &&
                       board[row][col] == BLACKKING) { //castle
                isCastle = true;
                board[row][col] = 0;
                if (selectedCol == 0) { //long
                    board[selectedRow][2] = BLACKKING;
                    board[selectedRow][3] = BLACKROOK;
                } else { //short
                    board[selectedRow][6] = BLACKKING;
                    board[selectedRow][5] = BLACKROOK;
                }
            } else {
                board[row][col] = board[selectedRow][selectedCol];
            }
            board[selectedRow][selectedCol] = 0;

            if (board[selectedRow][selectedCol] == BLACKROOK && board[row][col] == BLACKKING) { //castle
                isCastle = true;
            }
            if (isCastle) {
                if (selectedCol == 0) { //long
                    pieceSprites[selectedRow][2] = pieceSprites[selectedRow][selectedCol];
                    pieceSprites[selectedRow][3] = pieceSprites[row][col];
                    pieceSprites[selectedRow][2].setPosition(2 * squareSize, selectedRow * squareSize);
                    pieceSprites[selectedRow][3].setPosition(3 * squareSize, selectedRow * squareSize);
                    int pieceIndex = board[selectedRow][2];
                    if (pieceIndex < 0) {
                        pieceIndex = ((pieceIndex + 1) * -1) + 6;
                    } else {
                        pieceIndex = pieceIndex - 1;
                    }
                    pieceSprites[selectedRow][2].setTexture(pieceTextures[pieceIndex]);
                    pieceIndex = board[selectedRow][3];
                    if (pieceIndex < 0) {
                        pieceIndex = ((pieceIndex + 1) * -1) + 6;
                    } else {
                        pieceIndex = pieceIndex - 1;
                    }
                    pieceSprites[selectedRow][3].setTexture(pieceTextures[pieceIndex]);
                } else { //short
                    pieceSprites[selectedRow][6] = pieceSprites[selectedRow][selectedCol];
                    pieceSprites[selectedRow][5] = pieceSprites[row][col];
                    pieceSprites[selectedRow][6].setPosition(6 * squareSize, selectedRow * squareSize);
                    pieceSprites[selectedRow][5].setPosition(5 * squareSize, selectedRow * squareSize);
                    int pieceIndex = board[selectedRow][6];
                    if (pieceIndex < 0) {
                        pieceIndex = ((pieceIndex + 1) * -1) + 6;
                    } else {
                        pieceIndex = pieceIndex - 1;
                    }
                    pieceSprites[selectedRow][6].setTexture(pieceTextures[pieceIndex]);
                    pieceIndex = board[selectedRow][5];
                    if (pieceIndex < 0) {
                        pieceIndex = ((pieceIndex + 1) * -1) + 6;
                    } else {
                        pieceIndex = pieceIndex - 1;
                    }
                    pieceSprites[selectedRow][5].setTexture(pieceTextures[pieceIndex]);
                }
                pieceSprites[row][col] = sf::Sprite();  // Clear the previous sprite
                pieceSprites[selectedRow][selectedCol] = sf::Sprite();  // Clear the previous sprite
            } else {
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
            }
            isPieceSelected = false;
            curTurn = (curTurn % 2) + 1;
            int isChecked = false;
            if (board[row][col] == WHITEPAWN || board[row][col] == BLACKPAWN) {
                hundredMovesNoPawnMoveCounter = 0;
            } else {
                hundredMovesNoPawnMoveCounter++;
            }
            if (isDraw(true, board)) {
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
        cout << "--- CURRENT BOARD STATE ---" << endl;
        for (int i = 0; i < boardSize; i++) {
            cout << "{";
            for (int j = 0; j < boardSize; j++) {
                if (j == 7) {
                    cout << board[i][j] << "}," << endl;
                } else {
                    cout << board[i][j] << ", ";
                }
            }
        }
        cout << endl;
    }
}

int main() {
    // Array to represent the chessboard
    int chessboard[boardSize][boardSize] = {
            {BLACKROOK, BLACKKNIGHT, BLACKBISHOP, BLACKQUEEN, BLACKKING, BLACKBISHOP, BLACKKNIGHT, BLACKROOK},
            {BLACKPAWN, BLACKPAWN,   BLACKPAWN,   BLACKPAWN,  BLACKPAWN, BLACKPAWN,   BLACKPAWN,   BLACKPAWN},
            {0,         0,           0,           0,          0,         0,           0,           0},
            {0,         0,           0,           0,          0,         0,           0,           0},
            {0,         0,           0,           0,          0,         0,           0,           0},
            {0,         0,           0,           0,          0,         0,           0,           0},
            {WHITEPAWN, WHITEPAWN,   WHITEPAWN,   WHITEPAWN,  WHITEPAWN, WHITEPAWN,   WHITEPAWN,   WHITEPAWN},
            {WHITEROOK, WHITEKNIGHT, WHITEBISHOP, WHITEQUEEN, WHITEKING, WHITEBISHOP, WHITEKNIGHT, WHITEROOK}
    };
//    // Array to represent the chessboard
//    int chessboard[boardSize][boardSize] = {
//            {-1, -2, -3, 0, 0, 0, -2, -1},
//            {-6, 0, 0, -5, 0, 0, 0, -6},
//            {0, -6, -6, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, -6, -6, 0},
//            {0, 0, 6, 0, 0, 0, 0, 0},
//            {0, 0, 0, 2, 0, 0, 0, 0},
//            {6, 6, 0, 0, 0, 6, 6, 6},
//            {0, 0, 0, 0, 1, 1, 5, 0},
//    };
    loadTextures();
    createSprites(chessboard);
    while (window.isOpen()) {
        render(chessboard);
        if (!isGameOver) {
            handleEvents(chessboard);
        }
    }
    return 0;
}
