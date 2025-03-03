#include <cassert>
#include <iostream>
#include "chess.h"

// Test 1: Verify that the starting position is evaluated as equal.
void testInitialPosition() {
    ChessState state;
    // Set up white pieces (using your initial position bitboards)
    state.pieces[5][0] = 0x000000000000FF00ULL; // White pawns
    state.pieces[4][0] = 0x0000000000000010ULL; // White king
    state.pieces[3][0] = 0x0000000000000008ULL; // White queen
    state.pieces[2][0] = 0x0000000000000024ULL; // White bishops
    state.pieces[1][0] = 0x0000000000000042ULL; // White knights
    state.pieces[0][0] = 0x0000000000000081ULL; // White rooks

    // Set up black pieces
    state.pieces[5][1] = 0x00FF000000000000ULL; // Black pawns
    state.pieces[4][1] = 0x1000000000000000ULL; // Black king
    state.pieces[3][1] = 0x0800000000000000ULL; // Black queen
    state.pieces[2][1] = 0x2400000000000000ULL; // Black bishops
    state.pieces[1][1] = 0x4200000000000000ULL; // Black knights
    state.pieces[0][1] = 0x8100000000000000ULL; // Black rooks

    // Ensure it's White's turn
    state.isWhiteToMove = true;

    int score = evaluate(state);
    std::cout << "Test Initial Position: Score = " << score << std::endl;
    // Both sides have equal material so the score should be 0.
    assert(score == 0 && "Initial position should be equal (0 score)");
}

// Test 2: Remove one black pawn to give White a material advantage.
void testWhiteAdvantage() {
    ChessState state;
    // Set up initial position again.
    state.pieces[5][0] = 0x000000000000FF00ULL;
    state.pieces[4][0] = 0x0000000000000010ULL;
    state.pieces[3][0] = 0x0000000000000008ULL;
    state.pieces[2][0] = 0x0000000000000024ULL;
    state.pieces[1][0] = 0x0000000000000042ULL;
    state.pieces[0][0] = 0x0000000000000081ULL;

    state.pieces[5][1] = 0x00FF000000000000ULL;
    state.pieces[4][1] = 0x1000000000000000ULL;
    state.pieces[3][1] = 0x0800000000000000ULL;
    state.pieces[2][1] = 0x2400000000000000ULL;
    state.pieces[1][1] = 0x4200000000000000ULL;
    state.pieces[0][1] = 0x8100000000000000ULL;

    state.isWhiteToMove = true;

    // Remove one black pawn.
    // For instance, remove the pawn at bit index 48.
    state.pieces[5][1] &= ~(1ULL << 48);

    int score = evaluate(state);
    std::cout << "Test White Advantage: Score = " << score << std::endl;
    // With one less black pawn (worth 10000 points), evaluation should show White ahead by 10000.
    assert(score == 10000 && "White advantage by one pawn should give a score of 10000");
}

// Test 3: With Black to move, the evaluation function negates the score.
void testBlackToMove() {
    ChessState state;
    // Set up the board as in testWhiteAdvantage.
    state.pieces[5][0] = 0x000000000000FF00ULL;
    state.pieces[4][0] = 0x0000000000000010ULL;
    state.pieces[3][0] = 0x0000000000000008ULL;
    state.pieces[2][0] = 0x0000000000000024ULL;
    state.pieces[1][0] = 0x0000000000000042ULL;
    state.pieces[0][0] = 0x0000000000000081ULL;

    state.pieces[5][1] = 0x00FF000000000000ULL;
    state.pieces[4][1] = 0x1000000000000000ULL;
    state.pieces[3][1] = 0x0800000000000000ULL;
    state.pieces[2][1] = 0x2400000000000000ULL;
    state.pieces[1][1] = 0x4200000000000000ULL;
    state.pieces[0][1] = 0x8100000000000000ULL;

    // Remove one black pawn as before.
    state.pieces[5][1] &= ~(1ULL << 48);

    // Set it to Black's turn.
    state.isWhiteToMove = false;

    int score = evaluate(state);
    std::cout << "Test Black To Move: Score = " << score << std::endl;
    // With white ahead by 10000 points, but Black to move the function returns -score.
    assert(score == -10000 && "Black to move with a white advantage should give a score of -10000");
}

int main() {
    testInitialPosition();
    testWhiteAdvantage();
    testBlackToMove();

    std::cout << "All evaluation tests passed!" << std::endl;
    return 0;
}
