#include "minesweeper.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>

using namespace std;

Minesweeper::Minesweeper() : side(0), mineCount(0), movesLeft(0) {
    // srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
}

bool Minesweeper::isValid(int row, int col) const {
    return row >= 0 && row < side && col >= 0 && col < side;
}

bool Minesweeper::isMine(int row, int col, const vector<vector<char>>& board) const {
    return isValid(row, col) && board[row][col] == '*';
}


