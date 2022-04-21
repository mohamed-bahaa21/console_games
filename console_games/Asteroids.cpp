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
		float angle;
	};

	vector<sSpaceObject> vecAsteroids;
	sSpaceObject player;

protected:
	// called one at the start
	virtual bool OnUserCreate()
	{
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });

		// initialize player position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.angle = 0.0f;

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// called once per frame
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		// steer 
		if (m_keys[VK_LEFT].bHeld)
			player.angle -= 5.0f * fElapsedTime;
		if (m_keys[VK_RIGHT].bHeld)
			player.angle += 5.0f * fElapsedTime;

		// thrust
		if (m_keys[VK_UP].bHeld)
		{
			// acceleration changes velocity (with respect to time)
			player.dx += sin(player.angle) * 20.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
		}

		// velocity changes position (with respect to time)
		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;

		// keep ship in gamespace
		WrapCoordinates(player.x, player.y, player.x, player.y);

		// update and draw asteroids
		for (auto &a : vecAsteroids)
		{
			a.x += a.dx *fElapsedTime;
			a.y += a.dy *fElapsedTime;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			for (int x = 0; x < a.nSize; x++)
				for (int y = 0; y < a.nSize; y++)
					Draw(a.x + x, a.y + y, PIXEL_QUARTER, FG_RED);
		}




		return true;
	}

	void DrawWireFrameModel(const vector<pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short color = FG_WHITE)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		vector<pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// rotate 
		for (int i = 0; i < 3; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * cosf(player.angle) - vecTransformedCoordinates[i].second * sinf(player.angle);
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].first * sinf(player.angle) + vecTransformedCoordinates[i].second * cosf(player.angle);
		}

		// scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// translate
		for (int i = 0; i < 3; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + player.x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + player.y;
		}

		// draw closed polygon
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(vecTransformedCoordinates[i % verts].first,
				vecTransformedCoordinates[i % verts].second,
				vecTransformedCoordinates[j % verts].first,
				vecTransformedCoordinates[j % verts].second);
		}
	}



	void WrapCoordinates(float ix, float iy, float &ox, float &oy)
	{
		float screen_width = (float)ScreenWidth();
		float screen_height = (float)ScreenHeight();
		ix = ox;
		iy = oy;
		if (ix < 0.0f) ox = ix + screen_width;
		if (ix >= screen_width) ox = ix - screen_width;
		if (iy < 0.0f) oy = iy + screen_height;
		if (iy >= screen_height) oy = iy - screen_height;

	}

	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
	{
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);
		olcConsoleGameEngine::Draw(fx, fy, c, col);
	}

};





int main()
{
	Asteroids game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();
	return 0;
}
