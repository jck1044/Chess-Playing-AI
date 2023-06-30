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
    MovePiece bestChild;
    bool doesWhiteMoveFromHere{};
};

sf::RenderWindow window(sf::VideoMode(1400, 1200), "White's Turn");
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
const int EMPTY = 0;
int curTurn = 1;
int hundredMovesNoPawnMoveCounter = 0;
std::vector<int> favorCenter = {4, 3, 5, 2, 6, 1, 7, 0};

// Load chess piece textures
sf::Texture pieceTextures[15];
sf::Texture highlightTextures[12];
sf::Sprite pieceSprites[boardSize + 1][boardSize];
sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));

int isMoveAllowed(int firstRow, int firstCol, int secondRow, int secondCol, int board[8][8]);
