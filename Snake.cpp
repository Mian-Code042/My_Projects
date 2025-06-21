#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

// Game state
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 20;
const int width = 20;
int headX, headY, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tail_len;

// Move cursor to (x,y) without clearing screen
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setup() {
    gameOver = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    tail_len = 0;
}

void draw() {
    gotoXY(0, 0);

    // Top Border
    cout << "\t\t";
    for (int i = 0; i < width + 2; i++)
        cout << "==";
    cout << endl;

    // Game Area
    for (int i = 0; i < height; i++) {
        cout << "\t\t||";
        for (int j = 0; j < width; j++) {
            if (j == headX && i == headY)
                cout << "O ";
            else if (j == fruitX && i == fruitY)
                cout << "* ";
            else {
                bool printTail = false;
                for (int k = 0; k < tail_len; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o ";
                        printTail = true;
                        break;
                    }
                }
                if (!printTail)
                    cout << "  ";
            }
        }
        cout << "||" << endl;
    }

    // Bottom Border
    cout << "\t\t";
    for (int i = 0; i < width + 2; i++)
        cout << "==";
    cout << endl;

    cout << "\t\tScore: " << score << endl;
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;
    for (int i = 1; i < tail_len; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }

    // Game over on wall hit
    if (headX < 0 || headX >= width || headY < 0 || headY >= height)
        gameOver = true;

    // Game over on tail hit
    for (int i = 0; i < tail_len; i++) {
        if (tailX[i] == headX && tailY[i] == headY)
            gameOver = true;
    }

    // Fruit eaten
    if (headX == fruitX && headY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail_len++;
    }
}

int main() {
    char start;
    cout << "\t---------------------------------------" << endl;
    cout << "\t\t: Snake King :\n";
    cout << "\t---------------------------------------" << endl;
    cout << "Press 's' to Start Game! ";
    cin >> start;

    if (start == 's' || start == 'S') {
        setup();
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100); // delay
        }
    }

    cout << "\n\t\tGame Over! Final Score: " << score << endl;
    system("pause");
    return 0;
}
