#include <iostream>
#include <vector>

// Globals CAUSE VISUAL STUDIO CANT OPEN A FUCKING HEADER FILE FUCK YOU
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
    virtual void change(int num) = 0;
};
class MineCell : public Cell{
    char mine = 'x';
public:
    char getchar() { 
        if (flagged) {
            return 'f';
        }
        return revealed ? mine : '*'; 
    }
    void flag() {
        if (flagged == true) {
            flagged = false;
        }
        else {
            flagged = true;
        }
    }
    bool isMine() {
        return true;
    }
    bool isFlagged() {
        return flagged;
    }
    void reveal() {
        revealed = true;
    }
    void change(int num) {
    }
};
class NumberCell : public Cell {
private:
    char number = '0';
public:
    NumberCell(int num){
        number = num;
    }
    char getchar() { 
        if (flagged) {
            return 'f';
        }
        return revealed ? number : '*';
    }
    void flag() {
        if (flagged == true) {
            flagged = false;
        }
        else {
            flagged = true;
        }
    }
    void reveal() {
        revealed = true;
    }
    bool isMine() {
        return false;
    }
    bool isFlagged() {
        return flagged;
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

    // 
    void draw(std::vector<std::vector<Cell*>> &v) { 

        //Adds Legend bar to the top
        std::cout << "[ ]";
        for (int it = 0; it < noRows; it++) {
            std::cout << "["<<it<<"]";
        }
        std::cout << "\n";

        for (int i = 0; i < noCols; i++) {
            std::cout << "[" << i << "]"; //Adds legend to the left
            for (int j = 0; j < noRows; j++) {
                std::cout << "[";
                std::cout << v[i][j]->getchar();
                std::cout << "]";
            }
            std::cout << "\n";
        }
    }

    // Using the Factory Design Pattern
    // Fill the board with Mines and Empty Number Cells
    void fillBoard(std::vector<std::vector<Cell*>> &v) {
        CellFactory factory;
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (j == 2) {
                    v[i][j] = factory.create(MINE);
                }
                else {
                    v[i][j] = factory.create(NUMBER);
                }
            }
        }
    }

    //A terrible function to change numbers of a grid by its hood mines
    //I cant spel neihtghbor so I called it 'the hood'
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
    //Function to Check if all Mines are flagged
    //Was lazy and did it the count them all up way
    // Inneficient please re-do when not on pills
    bool allMinesFlagged(std::vector<std::vector<Cell*>> v) {
        int count = 0;
        for (int i = 0; i < noCols; i++) {
            for (int j = 0; j < noRows; j++) {
                if (v[i][j]->isMine() == true && v[i][j]->isFlagged() != true) {
                    count++;
                }
                if (v[i][j]->isMine() == true && v[i][j]->isFlagged() == true) {
                    std::cout << "Bomb at [" << i << "][" << j << "] is flagged\n";
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
                    //std::cout << "Mine exploded at: " << i << "," << j << "\n";
                    return true;
                }
            }
        }
        return false;
    }
    //Function Reveals a Cell
    void revealSingleCell(std::vector<std::vector<Cell*>> &v, int x, int y) {
        v[x][y]->reveal();
    }
    //Function Flags a Cell
    void flagSingleCell(std::vector<std::vector<Cell*>> &v, int x, int y) {
        v[x][y]->flag();
    }
};
class InputHandler{
};
class Game {
private:
    bool endGameFlag = false;
    bool isWinner = false;
    bool isMineExploded = false;
protected:
public:
    
    // Ending text depends on winning variable
    void ending(bool isWin) {
        if (isWin) {
            std::cout << "Congratulations! You Won!";
        }
        else {
            std::cout << "You blew up! Better luck next life!";
        }
    }
    
    // Main function for Game
    void run() { 
        // Vector of Vectors to represent the Cell Grid
        std::vector<std::vector<Cell*>> Cells;
        //Resize the vectors to constants defined in header file
        Cells.resize(noRows, std::vector<Cell*>(noCols, 0));
        
        Board gameBoard;

        gameBoard.fillBoard(Cells); //Adds Mines to Cells
        gameBoard.updateBoardNumbers(Cells); // Adds Numbers to Cells

        while (!endGameFlag) {

            //Draw the Grid
            gameBoard.draw(Cells);

            // Input
            int x, y;
            char a;
            std::cout << "Enter x, y, r(reveal)/f(flag); q to quit\n";
            std::cin >> x >> y >> a;
            // The only guarantee vector doesn't go out of range here
            // is that the player knows to type between 0 - 8
            // else error


            if (a == 'r') {
                Cells[x][y]->reveal();
                //std::cout << "Cell at"<< x << "," << y << "revealed \n";
            }
            else if (a == 'f') {
                Cells[x][y]->flag();
                //std::cout << "Cell at" << x << "," << y << "flagged \n";
            }
            else {
                // If fuck around then: quit the game
                //Is not working properly:)
                //Problem for future me
                endGameFlag = true;
            }
            if (a == 'q') {
                endGameFlag = true;
                //Shows you blew up, change it so you deserted your job?
            }

            //Checks if a mine is revealed
            isMineExploded = gameBoard.isMineRevealed(Cells);
            //Checks if all mines are flagged
            isWinner = gameBoard.allMinesFlagged(Cells);


            if (isWinner) {
                endGameFlag = true;
            }
            if (isMineExploded) {
                endGameFlag = true;
                isWinner = false;
            }


            system("CLS"); 
            // change to be multiplatform
            // cause system CLS only works on windows apparently
        }

        // Ending Text function
        ending(isWinner);
    }

};

int main(){
    Game GameInstance;
    GameInstance.run();

    return 0;
}
