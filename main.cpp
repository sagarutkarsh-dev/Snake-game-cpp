#include <iostream>
#include <unistd.h>
#include <termios.h> 
#include <fcntl.h>  
#include <vector>
#include <ctime>

using namespace std;

// This function checks if a key has been pressed without blocking the game
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int prev_x,prev_y;
int speed = 400000;
vector<vector<int>> Trail(0, vector<int>(2,-1));
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    srand(time(0));
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

int InTrail(int i, int j){
    for(int k = 0; k < Trail.size(); k++){
        if(Trail[k][0] == j && Trail[k][1] == i)
            return 1;
    }

    return 0;
}

void Draw() {
    system("clear"); 
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (i == y && j == x)
                cout << "S";
            else if (i == fruitY && j == fruitX)
                cout << "*";
            else if (InTrail(i, j))
                cout << "s";
            else
                cout << " ";
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}

void Input() {
    if (kbhit()) {
        switch (getchar()) {
        case 'a': if(dir != RIGHT) dir = LEFT; break;
        case 'd': if(dir != LEFT) dir = RIGHT; break;
        case 'w': if(dir != DOWN) dir = UP; break;
        case 's': if(dir != UP) dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break; 
    case DOWN: y++; break;
    default: break;
    }
    
    // Wall Collision (Game Over if you hit wall)
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    if(InTrail(y,x))
        gameOver = true;
    
    // Eating Fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        speed -= 10000;

        Trail.push_back({-1, -1});

        fruitX = rand() % width;
        fruitY = rand() % height;
    }
}

void SnakeTrail(){
    for(int i = Trail.size()-1; i > 0; i--){
        Trail[i][0] = Trail[i-1][0];
        Trail[i][1] = Trail[i-1][1];
    }

    if (Trail.size() > 0) {
        Trail[0][0] = x;
        Trail[0][1] = y;
    }

}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        SnakeTrail();
        Input();
        Logic();
        usleep(speed); // Speed control
    }
    return 0;
}
