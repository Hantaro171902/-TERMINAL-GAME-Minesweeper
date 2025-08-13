#include "minesweeper.hpp"
#include "ultils.hpp"
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

void Minesweeper::makeMove(int &x, int &y) const {
    while (true) {
        cout << "Enter your move [row] [column] -> ";
        cin >> x >> y;
        if (x < side && y < side) return;
    }
}

void Minesweeper::printBoard(const std::vector<vector<char>>& board) const {
    clearTerminal();
    printf("\n\n\t\t\t    ");
    for (int i = 0; i < side; i++)
        printf("%s", (i > 9) ? " " : "  ");
    printf("\n\t\t\t    ");
    for (int i = 0; i < side; i++)
        printf("%d ", i % 10);
    printf("\n\n");

    for (int i = 0; i < side; i++) {
        printf("\t\t\t    ");
        for (int j = 0; j < side; j++)
            printf("%c ", board[i][j]);
        printf(" %2d\n", i);
    }
}

int Minesweeper::countAdjacentMines(int row, int col) const {
    int count = 0;
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};
    for (int i = 0; i < 8; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        if (isMine(nr, nc, myBoard)) count++;
    }
    return count;
}

bool Minesweeper::playMinesUntil(int row, int col) {
    if (myBoard[row][col] != '-') {
        cout << "You have already revealed this cell!" << endl;
        return false;
    }

    if (realBoard[row][col] == '*') {
        myBoard[row][col] = '*';
        for (auto &m : mines)
            myBoard[m.first][m.second] = '*'; // Reveal all mines
        printBoard(myBoard);
        cout << "Game Over! You hit a mine." << endl;
        return true; // Game over
    }

    int count = countAdjacentMines(row, col);
    myBoard[row][col] = (count > 0) ? '0' + count : ' ';
    movesLeft--;

    if (!count) {
        int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dc[] = {0, 0, 1, -1, -1, 1, -1, 1};
        for (int i = 0; i < 8; i++) {
            int nr = row + dr[i];
            int nc = col + dc[i];
            if (isValid(nr, nc) && myBoard[nr][nc] == '-') {
                playMinesUntil(nr, nc);
            }
        }
    }
    return false;
}

void Minesweeper::placeMines() {
    vector<bool> mark(side * side, false);
    for (int i = 0; i < mineCount; i++) {
        int random = rand() % (side * side);
        int x = random / side;
        int y = random % side;
        if (!mark[random]) {
            mines.push_back({x, y});
            realBoard[x][y] = '*';
            mark[random] = true;
            i++;
        }
    }
}

void Minesweeper::initialiseBoards() {
    srand(time(NULL));
    realBoard.assign(side, vector<char>(side, '-'));
    myBoard.assign(side, vector<char>(side, '-'));
}

void Minesweeper::cheatMines() const {
    cout << "The mine locations are - " << endl;
    printBoard(realBoard);
}

void Minesweeper::replaceMine(int row, int col) {
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            if (realBoard[i][j] != '*') {
                realBoard[i][j] = '*';
                realBoard[row][col] = '-';
                return;
            }
        }
    }
}

void Minesweeper::chooseDifficulty() {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t\tMINESWEEPER";
    cout << "\n\n\t\t\t\t\t\tCHOOSE DIFFICULTY LEVEL : ";
    cout << "\n\n\t\t\t\t\t\t0.BEGINNER\n\t\t\t\t\t\t1.INTERMEDIATE\n\t\t\t\t\t\t2.ADVANCED";
    cout << "\n\n\t\t\t\t\t\tENTER CHOICE (0-2) : ";
    int choice;
    cin >> choice;
    if (choice == 0) {
        side = 9; mineCount = 10;
    } else if (choice == 1) {
        side = 16; mineCount = 40;
    } else if (choice == 2) {
        side = 24; mineCount = 99;
    } else {
        exit(0);
    }
}

void Minesweeper::play() {
    bool gameOver = false;
    movesLeft = side * side - mineCount;
    mines.clear();

    initialiseBoards();
    placeMines();

    int currentMoveIndex = 0;
    while (!gameOver) {
        cout << "Current Status of Board : " << endl;
        printBoard(myBoard);

        int x, y;
        makeMove(x, y);

        if (currentMoveIndex == 0 && isMine(x, y, realBoard)) {
            replaceMine(x, y);
        }

        gameOver = playMinesUntil(x, y);
        currentMoveIndex++;

        if (!gameOver && movesLeft == 0) {
            cout << "You Won!" << endl;
            gameOver = true;
        } 
    }
}
