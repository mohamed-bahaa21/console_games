#include <iostream>
#include <conio.h>
using namespace std;

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(x) // milli-seconds
#endif

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool gameOver, pause;

const int width = 30, height = 30;
const int world_width = 20, world_height = 20;
int x, y, appleX, appleY, score, scoreX, scoreY;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void setConsoleColor(unsigned short color)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	SetConsoleTextAttribute(hOut, color);
}

const unsigned short DARK_BLUE = FOREGROUND_BLUE;
const unsigned short BRIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

void Setup() {
	setConsoleColor(BRIGHT_BLUE);
	//setConsoleColour(DARK_BLUE);

	gameOver = false;
	pause = false;
	dir = STOP;

	x = world_width / 2;
	y = world_height / 2;

	appleX = rand() % world_width;
	appleY = rand() % world_height;

	score = 0;
	scoreX = 23;
	scoreY = 5;
}

void drawSnake()
{
	cout << "O";
}

void Draw() {
	system("cls");
	//cls();
	
	

	for (int i = 0; i < world_width + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < world_height; i++) {
		for (int j = 0; j < world_width; j++)
		{
			if (j == 0)
				cout << "#";


			if (i == y && j == x)
				drawSnake();
			else if (i == appleY && j == appleX)
				cout << "&";
			else if (j == scoreX && i == scoreY)
				cout << "";
			else {
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}


			if (j == world_width - 1)
				cout << "#";
		}
		cout << endl;
	}

	for (int i = 0; i < world_width + 2; i++)
		cout << "#";
	cout << endl;

	cout << "Score: " << score << endl;
	cout.flush();
}

void GameInput() {
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
		case 'c':
			if (pause)
				pause = false;
			else if (!pause)
				pause = true;
			break;
		}
	}
}

void PauseInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'x':
			gameOver = true;
			break;
		case 'c':
			if (pause)
				pause = false;
			else if (!pause)
				pause = true;
			break;
		}
	}
}

void wallCollisionX() {
	if (x >= world_width - 1)
		x = world_width - 1;
	else if (x <= 0)
		x = 0 + 1;
}
void wallCollisionY() {
	if (y >= world_height - 1)
		y = world_height - 1;
	else if (y <= 0)
		y = 0;
}

void Logic() {
	int& prevX = tailX[0];
	int& prevY = tailY[0];

	int prev2X, prev2Y;

	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];

		tailX[i] = prevX;
		tailY[i] = prevY;

		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir)
	{
	case LEFT:
		x--;
		wallCollisionX();
		break;
	case RIGHT:
		x++;
		wallCollisionX();
		break;
	case UP:
		y--;
		wallCollisionY();
		break;
	case DOWN:
		y++;
		wallCollisionY();
		break;
	default:
		break;
	}

	// passing walls
	if (x >= width)  x = 0; else if (x < 0) x = width - 1;
	if (y >= height)  x = 0; else if (y < 0) y = height - 1;

	// crashing to walls
	if (x > world_width || x<0 || y>world_height || y < 0)
	{
		gameOver = true;
	}

	// eating tail
	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
		{
			gameOver = true;
		}
	}

	// eating apple
	if (x == appleX && y == appleY)
	{
		appleX = rand() % world_width;
		appleY = rand() % world_height;
		score += 10;
		nTail++;
	}
}

int snake() {

	Setup();

	while (!gameOver) {

		if (!pause) {
			Draw();
			GameInput();
			Logic();
			sleep(25);
		}
		else {
			PauseInput();
		}
	}

	return 0;
}