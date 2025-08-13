#pragma once

#include <vector>

class Minesweeper {
public:
    Minesweeper();
    void chooseDifficulty();
    void play();

private:
    static const int MAX_MINE = 99;
    static const int MAX_SIDE = 25;

    int side;
    int mineCount;
    int movesLeft;

    std::vector<std::vector<char>> realBoard; // Board to hold mine counts
    std::vector<std::vector<char>> myBoard; // Board to display to the player
    std::vector<std::vector<int,int>> mines; // Track revealed cells

    bool isValid(int row, int col) const;
    bool isMine(int row, int col, const std::vector<std::vector<char>>& board) const;
    void makeMove(int &x, int &y) const;
    void printBoard(const std::vector<std::vector<char>>& board) const;
    bool playMinesUntil(int row, int col);
    void placeMines();
    void initialiseBoards();
    void cheatMines() const;
    void replaceMine(int row, int col);
    int countAdjacentMines(int row, int col, const std::vector<std::vector<char>>& board) const;

}