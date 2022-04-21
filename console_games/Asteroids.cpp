#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

#include "olcConsoleGameEngine.h";

class Asteroids : public olcConsoleGameEngine
{
public:
	Asteroids()
	{
		m_sAppName = L"Asteroids";
	}

private:
	struct sSpaceObject
	{
		float x;
		float y;
		float dx;
		float dy;
		int nSize;
	};

	vector<sSpaceObject> vecAsteroids;

protected:
	// called one at the start
	virtual bool OnUserCreate()
	{
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16 });

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// called once per frame
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		for (auto &a : vecAsteroids)
		{
			a.x += a.dx *fElapsedTime;
			a.y += a.dy *fElapsedTime;

			for (int x = 0; x < a.nSize; x++)
			{
				for (int y = 0; y < a.nSize; y++)
				{
					Draw(a.x + x, a.y + y, PIXEL_QUARTER, FG_RED);
				}

			}
		}

		return true;
	}
};




int main()
{
	Asteroids game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();
	return 0;
}
