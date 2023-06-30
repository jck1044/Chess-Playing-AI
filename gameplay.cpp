bool isRookMoveValid(int firstCol, int firstRow, int secondCol, int secondRow, bool isWhite, int board[8][8]) {
    // Check if the move is castling
    if (isWhite && firstRow == secondRow) {
        // Check if the king and rook are in their initial positions
        if (board[firstRow][firstCol] == WHITEROOK && board[secondRow][secondCol] == WHITEKING && firstRow == 7 &&
            secondRow == 7 && secondCol == 4) {
            if (firstCol == 0 || firstCol == 7) {
                if (firstCol == 0) {
                    // Check if there are any pieces between the king and rook
                    for (int col = 1; col < 4; col++) {
                        if (board[firstRow][col] != 0) {
                            return false;
                        }
                    }
                    for (int row = 0; row < boardSize; row++) {
                        for (int col = 0; col < boardSize; col++) {
                            if (board[row][col] < 0) {
                                if (isMoveAllowed(row, col, firstRow, 2, board)) {
                                    return false;
                                }
                                if (isMoveAllowed(row, col, firstRow, 3, board)) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                } else {
                    // Check if there are any pieces between the king and rook
                    for (int col = 5; col < 7; col++) {
                        if (board[firstRow][col] != 0) {
                            return false;
                        }
                    }
                    for (int row = 0; row < boardSize; row++) {
                        for (int col = 0; col < boardSize; col++) {
                            if (board[row][col] < 0) {
                                if (isMoveAllowed(row, col, firstRow, 5, board)) {
                                    return false;
                                }
                                if (isMoveAllowed(row, col, firstRow, 6, board)) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }
    } else if (!isWhite && firstRow == secondRow && secondCol == 4) {
        // Check if the king and rook are in their initial positions
        if (board[firstRow][firstCol] == BLACKROOK && board[secondRow][secondCol] == BLACKKING && firstRow == 0 &&
            secondRow == 0 && secondCol == 4) {
            if (firstCol == 0 || firstCol == 7) {
                if (firstCol == 0) {
                    // Check if there are any pieces between the king and rook
                    for (int col = 1; col < 4; col++) {
                        if (board[firstRow][col] != 0) {
                            return false;
                        }
                    }
                    for (int row = 0; row < boardSize; row++) {
                        for (int col = 0; col < boardSize; col++) {
                            if (board[row][col] > 0) {
                                if (isMoveAllowed(row, col, firstRow, 2, board)) {
                                    return false;
                                }
                                if (isMoveAllowed(row, col, firstRow, 3, board)) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                } else {
                    // Check if there are any pieces between the king and rook
                    for (int col = 5; col < 7; col++) {
                        if (board[firstRow][col] != 0) {
                            return false;
                        }
                    }
                    for (int row = 0; row < boardSize; row++) {
                        for (int col = 0; col < boardSize; col++) {
                            if (board[row][col] > 0) {
                                if (isMoveAllowed(row, col, firstRow, 5, board)) {
                                    return false;
                                }
                                if (isMoveAllowed(row, col, firstRow, 6, board)) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }
    }

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

bool isDraw(int isWhite, int board[8][8]) {
    return (isInStalemate(isWhite, board) || hundredMovesNoPawnMoveCounter >= 100 || isInsufficientMaterial(board));
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
