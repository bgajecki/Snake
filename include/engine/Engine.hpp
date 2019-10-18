#pragma once

#include "Snake.hpp"
#include "Point.hpp"
#include "Constants.hpp"

class GameProcess
{
public:
	GameProcess();

	void Display();
	void Special(int, int, int);
	void Keyboard(unsigned char, int, int);
	void Time(int);
	void CreatePoint(int);

private:
	bool isCollision(Object_t&, Object_t&);
	bool isCollision(Object&, Object&);
	GLuint loadTexture(const char*);
	float calibrateWidth(float);
	float calibrateHeight(float);
	float calibratePosX(float);
	float calibratePosY(float);

	Snake snake;
	std::array<std::unique_ptr<Point>, CONSTANTS::MAX_POINTS_ON_MAP> points;
	GLuint pointTexture;
};
