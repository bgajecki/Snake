#pragma once

#include "Head.hpp"
#include "Body.hpp"

class Snake : public Object
{
public:
	Snake();
	void Start(float, float, float, float, GLuint&, Direction_t);
	void Move();
	void ChangeDirection(Direction_t);
	void AddBodyPart();
	void Paint() override;

	std::vector<std::unique_ptr<Body>> body;
	GLuint snakeTexture;
	float snakeWidth, snakeHeight;
	const float snakeBodyDistance = 0.09f;
	bool isCanChangeDirection = true;
};