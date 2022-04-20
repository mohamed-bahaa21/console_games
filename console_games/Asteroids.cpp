#define OLC_PGE_APPLICATION
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#include "olcPixelGameEngine.h";

class Asteroids : public olc::PixelGameEngine
{
public:
	Asteroids()
	{
		sAppName = "Asteroids";
	}

public:
	bool OnUserCreate() override
	{
		// called one at the start
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

		return true;

	}
};

int main()
{
	Asteroids game;
	if (game.Construct(256, 240, 4, 4))
		game.Start();

	return 0;
}
