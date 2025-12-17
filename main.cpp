#include <iostream>
#include <unistd.h> // For sleep() on Linux

using namespace std;

// Global Variables
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    // Clear screen (Linux command)
    system("clear"); 

    // Top Wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Side Walls
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; // Left wall
            
            if (i == y && j == x)
                cout << "O"; // Snake Head
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Fruit
            else
                cout << " "; // Empty space

            if (j == width - 1)
                cout << "#"; // Right wall
        }
        cout << endl;
    }

    // Bottom Wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}

void Input() {
    // We will handle keyboard input here later
}

void Logic() {
    // Movement logic goes here
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        // Slow down the game slightly
        usleep(100000); // 100 milliseconds
    }
    return 0;
}