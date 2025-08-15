#include "minesweeper.hpp"
#include "ultils.hpp"
#include "color.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <iomanip>

using namespace std;

#define RESET "\033[0m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_BLACK "\033[40m"
#define BG_GRAY "\033[100"

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
        cout << endl;
        cout << "Enter your move [row] [column] -> ";
        cin >> x >> y;
        if (x < side && y < side) return;
    }
}

void Minesweeper::printBoard(const vector<vector<char>>& board) const {
    clearTerminal();

    setTextColor(32);

    cout << R"(
                            *                                                             
                            (  `                                                            
                            )\))(   (            (      (  (      (    (           (   (    
                            ((_)()\  )\   (      ))\ (   )\))(    ))\  ))\ `  )    ))\  )(   
                            (_()((_)((_)  )\ )  /((_))\ ((_)()\  /((_)/((_)/(/(   /((_)(()\  
                            |  \/  | (_) _(_/( (_)) ((_)_(()((_)(_)) (_)) ((_)_\ (_))   ((_) 
                            | |\/| | | || ' \))/ -_)(_-<\ V  V // -_)/ -_)| '_ \)/ -_) | '_| 
                            |_|  |_| |_||_||_| \___|/__/ \_/\_/ \___|\___|| .__/ \___| |_|   
                                                                        |_|

    )";
    resetTextColor();
    int i, j = 0;

    cout << "\n\n\t\t\t    " << setw(2);
    for (i = 0; i < side; i++)
        cout << (i > 9 ? "  " : "");

    // Column
    cout << "\n\t\t\t    ";
    for (i = 0; i < side; i++) {
        cout << " " << i%10 << setw(2);
        
    }
       
    cout << "\n\n";

    for (i = 0; i < side; i++) {
        cout << "\t\t\t " << setw(2) << i << " ";
        
        for (j = 0; j < side; j++) {
            char cell = board[i][j];
            string color;

            switch(cell) {
                case '1': color = BG_BLUE; break;
                case '2': color = BG_GREEN; break;
                case '3': color = BG_RED; break;
                case '4': color = BG_MAGENTA; break;
                case '5': color = BG_CYAN; break;
                case '6': color = BG_YELLOW; break;
                case '*': color = BG_RED; break; // mine
                case '-': color = BG_BLACK; break; // unrevealed
                default:  color = BG_WHITE; break; // empty space
            }

            cout << color << " " << cell << " " << RESET;
        }
        cout << endl;
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
    int placed = 0;
    while (placed < mineCount) {
        int random = rand() % (side * side);
        int x = random / side;
        int y = random % side;
        if (!mark[random]) {
            mines.push_back({x, y});
            realBoard[x][y] = '*';
            mark[random] = true;
            placed++;
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
    bool exitGame = false;
    while (!exitGame) {
        chooseDifficulty(); // Always allow difficulty selection before each game

        bool gameOver = false;
        movesLeft = side * side - mineCount;
        mines.clear();

        initialiseBoards();
        placeMines();

        int currentMoveIndex = 0;
        while (!gameOver) {
            cout << "Current Status of Board : " << endl;
            printBoard(myBoard);

            cout << endl;

            cout << "Enter your move [row] [column] or R (restart) or Q (quit): ";
            string input;
            cin >> input;

            if (input == "R" || input == "r") {
                cout << "Restarting game..." << endl;
                gameOver = true; // Break inner loop to restart
                continue;
            }
            if (input == "Q" || input == "q") {
                cout << "Quitting game..." << endl;
                exitGame = true;
                break;
            }

            int x, y;
            try {
                x = stoi(input);
                cin >> y;
            } catch (...) {
                cout << "Invalid input. Please enter row and column numbers." << endl;
                continue;
            }

            if (currentMoveIndex == 0 && isMine(x, y, realBoard)) {
                replaceMine(x, y);
            }

            bool moveResult = playMinesUntil(x, y);
            currentMoveIndex++;

            // Check win after move
            if (movesLeft == 0) {
                cout << "You Won!" << endl;
                gameOver = true;
            }

            // If moveResult is true, game over (mine hit)
            if (moveResult) {
                gameOver = true;
            }
        }
    }
}