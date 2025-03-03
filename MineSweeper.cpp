#include <iostream>
#include <vector>
#include <random>

// Globals 
constexpr int noCols = 9;
constexpr int noRows = 9;


enum Type {MINE, NUMBER};
// Classes
class Cell{
protected:
    bool revealed = false;
    bool flagged = false;
public:
    virtual char getchar() = 0;
    virtual void flag() = 0;
    virtual void reveal() = 0;
    virtual bool isMine() = 0;
    virtual bool isFlagged() = 0;
    virtual bool isZero() = 0;
    virtual void change(int num) = 0;
};

class MineCell : public Cell{
    char mine = 'x';
public:
    char getchar() { 
        if (flagged) { return 'f';}
        return revealed ? mine : '*'; 
    }
    void flag() {
        if (flagged) { flagged = false;}
        else { flagged = true; }
    }

    bool isMine() { return true;}
    bool isFlagged() {return flagged;}
    bool isZero() {return false;}
    void reveal() { revealed = true;}

    void change(int num) { ; } // empty
};
class NumberCell : public Cell {
private:
    char number = '0';
public:
    NumberCell(int num){
        number = num;
    }

    char getchar() { 
        if (flagged) { return 'f';}
        return revealed ? number : '*';
    }
    void flag() {
        if (flagged) { flagged = false;}
        else {flagged = true;}
    }

    void reveal() {revealed = true;}
    bool isMine() {return false;}
    bool isFlagged() {return flagged;}
    bool isZero() {
        if (number == '0') {
            return true;
        }
        return false;
    }

    void change(int num) {
        number = '0' + num;
    }
 
};
// Factory class responsible for creating cells
class CellFactory{
public:
    Cell* create(Type t) {
        switch (t) {
        case MINE:
            return new MineCell();
        case NUMBER:
            return new NumberCell('5');
        }
    }
};
class Board{
public:

    // VT-100 escape sequence
    void clear_screen(){
       // printf(
       //     "\033[2J"       // clear the screen
       //     "\033[1;1H");  // move cursor home
       // for(int i = 0; i < 20; i++)
            std::cout << std::endl;
     }
    // Draw to Terminal Screen Function
    void draw(std::vector<std::vector<Cell*>> &v) { 
        // Adds Legend bar to the top
        std::cout << "<---x--->" << std::endl;
        std::cout << "[ ]";
        for (int it = 0; it < noRows; it++) {
            std::cout << "["<<it<<"]";
        }
        std::cout << "\n";

        for (int i = 0; i < noCols; i++) {
            std::cout << "[" << i << "]"; // Adds legend to the left
            for (int j = 0; j < noRows; j++) {
                std::cout << "[";
                std::cout << v[i][j]->getchar();
                std::cout << "]";
            }
            std::cout << "\n";
        }
    }

    // Fill the board with Mines and Empty Number Cells
    void fillBoard(std::vector<std::vector<Cell*>> &v) {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 9);

        CellFactory factory;
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (dist(rd) % 3 == 0) {
                    v[i][j] = factory.create(MINE);
                }
                else {
                    v[i][j] = factory.create(NUMBER);
                }
            }
        }
    }

    // A terrible function to change numbers of a grid by its hood mines
    // I cant spel neihtghbor so I called it 'the hood'
    void updateBoardNumbers(std::vector<std::vector<Cell*>>& v) {
        int count = 0;
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                count = 0;
                // Top
                if (i > 0) {
                    //Left
                    if (j > 0) {
                        if (v[i - 1][j - 1]->isMine() == true) {
                            count++;
                       }
                    }
                    //Up
                    if (v[i - 1][j]->isMine() == true) {
                        count++;
                    }
                    //Right
                    if (j + 1 != noRows) {
                        if (v[i - 1][j + 1]->isMine() == true) {
                            count++;
                        }
                    }
                }
                //Left
                if (j > 0) {
                    if (v[i][j - 1]->isMine() == true) {
                        count++;
                    }
                }
                //Right
                if (j + 1 != noRows) {
                    if (v[i][j + 1]->isMine() == true) {
                        count++;
                    }
                }
                // Bottom
                if (i + 1 != noCols) {
                    //Left
                    if (j > 0) {
                        if (v[i + 1][j - 1]->isMine() == true) {
                            count++;
                        }
                    }
                    //Bottom
                    if (v[i + 1][j]->isMine() == true) {
                        count++;
                    }
                    //Right
                    if (j + 1 != noRows) {
                        if (v[i + 1][j + 1]->isMine() == true) {
                            count++;
                        }
                    }
                }
                v[i][j]->change(count);
            }
        }
    }
    // Function to Check if all Mines are flagged
    bool allMinesFlagged(std::vector<std::vector<Cell*>> v) {
        int count = 0;
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (v[i][j]->isMine() == true && v[i][j]->isFlagged() != true) {
                    count++;
                }
            }
        }
        if (count != 0) {
            return false;
        }
        else {
            return true;
        }
        return false;
        
    }
    // Function to check if mine is revealed
    bool isMineRevealed(std::vector<std::vector<Cell*>> v) {
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (v[i][j]->getchar() == 'x') {
                    return true;
                }
            }
        }
        return false;
    }
    // Function to clear all empty cells
    void revealAllZeroes(std::vector<std::vector<Cell*>> v) {
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (v[i][j]->isZero()) {
                    v[i][j]->reveal();
                }
            }
        }
    }
    // Function Reveals a Cell
    void revealSingleCell(std::vector<std::vector<Cell*>> &v, int x, int y) {
        v[x][y]->reveal();
    }
    // Function Flags a Cell
    void flagSingleCell(std::vector<std::vector<Cell*>> &v, int x, int y) {
        v[x][y]->flag();
    }
};

class Game {
private:
    bool endGameFlag = false;
    bool isWinner = false;
    bool isMineExploded = false;
    bool quitjob = false;
public:
    
    // Main function for Game
    void run() { 
        // Vector of Vectors to represent the Cell Grid
        // Maybe an std::array would be better here?
        std::vector<std::vector<Cell*>> Cells;
        // Resize the vectors to defined constants
        // The Game params should be able to change depending on difficulty selected
        // To add
        Cells.resize(noRows, std::vector<Cell*>(noCols, 0));
        
        Board gameBoard;

        gameBoard.fillBoard(Cells); // Adds Mines to Cells
        gameBoard.updateBoardNumbers(Cells); // Adds Numbers to Cells
        gameBoard.revealAllZeroes(Cells); // Reveals all empty Cells

        while (!endGameFlag) {

            // Draw the Grid
            gameBoard.draw(Cells);

            // Input
            int x, y;
            char answer;
            std::cout << "Enter x, y, r(reveal)/f(flag); q to quit\n";
            std::cin >> y >> x >> answer;
            // The only guarantee vector doesn't go out of range here
            // is that the player knows to type between 0 - Row/Col max
            // else error

            if (answer == 'r') {
                Cells[x][y]->reveal();
            }
            else if (answer == 'f') {
                Cells[x][y]->flag();
            }
            else {
                endGameFlag = true;
                break;
            }
                
            // Checks if a mine is revealed
            isMineExploded = gameBoard.isMineRevealed(Cells);
            // Checks if all mines are flagged
            isWinner = gameBoard.allMinesFlagged(Cells);

            if (isWinner) {
                endGameFlag = true;
            }
            if (isMineExploded) {
                endGameFlag = true;
                isWinner = false;
            }

            gameBoard.clear_screen();
        }
    }
};

int main(){
    Game GameInstance;
    GameInstance.run();

    return 0;
}
