#include "Snake.hpp"

Snake::Snake()
{

}

void Snake::Start(float x, float y, float width, float height, GLuint& texture, Direction_t direction)
{
	this->snakeWidth = width;
	this->snakeHeight = height;
	this->snakeTexture = texture;
	body.push_back(std::make_unique<Body>());
	this->body[0]->object.x = x;
	this->body[0]->object.y = y;
	this->body[0]->object.width = this->snakeWidth;
	this->body[0]->object.height = this->snakeHeight;
	this->body[0]->object.texture = this->snakeTexture;
	this->body[0]->direction = direction;
}

void Snake::Move()
{

	for (int i = this->body.size() - 1; i >= 0; --i)
	{
		switch(this->body[i]->direction)
		{
		case Direction_t::UP:
			this->body[i]->object.y += this->snakeBodyDistance;
			break;
		case Direction_t::DOWN:
			this->body[i]->object.y -= this->snakeBodyDistance;
			break;
		case Direction_t::LEFT:
			this->body[i]->object.x -= this->snakeBodyDistance;
			break;
		case Direction_t::RIGHT:
			this->body[i]->object.x += this->snakeBodyDistance;
			break;
		}

		if (i > 0)
			this->body[i]->direction = this->body[i - 1]->direction;

	}

}

void Snake::ChangeDirection(Direction_t direction)
{
	if(isCanChangeDirection)
	switch (direction)
	{
	case Direction_t::UP:
		if (this->body[0]->direction != Direction_t::DOWN && this->body[0]->direction != Direction_t::UP)
		{
			this->body[0]->direction = direction;
			this->isCanChangeDirection = false;
		}
		break;
	case Direction_t::DOWN:
		if (this->body[0]->direction != Direction_t::DOWN && this->body[0]->direction != Direction_t::UP)
		{
			this->body[0]->direction = direction;
			this->isCanChangeDirection = false;
		}
		break;
	case Direction_t::LEFT:
		if (this->body[0]->direction != Direction_t::LEFT && this->body[0]->direction != Direction_t::RIGHT)
		{
			this->body[0]->direction = direction;
			this->isCanChangeDirection = false;
		}
		break;
	case Direction_t::RIGHT:
		if (this->body[0]->direction != Direction_t::LEFT && this->body[0]->direction != Direction_t::RIGHT)
		{
			this->body[0]->direction = direction;
			this->isCanChangeDirection = false;
		}
		break;
	}
	
}

void Snake::AddBodyPart()
{
	body.push_back(std::make_unique<Body>());
	unsigned i = body.size() - 1;
	switch (this->body[i - 1]->direction)
	{
	case Direction_t::UP:
		this->body[i]->object.x = this->body[i - 1]->object.x;
		this->body[i]->object.y = this->body[i - 1]->object.y - this->snakeBodyDistance;
		break;
	case Direction_t::DOWN:
		this->body[i]->object.x = this->body[i - 1]->object.x;
		this->body[i]->object.y = this->body[i - 1]->object.y + this->snakeBodyDistance;
		break;
	case Direction_t::LEFT:
		this->body[i]->object.x = this->body[i - 1]->object.x + this->snakeBodyDistance;
		this->body[i]->object.y = this->body[i - 1]->object.y;
		break;
	case Direction_t::RIGHT:
		this->body[i]->object.x = this->body[i - 1]->object.x - this->snakeBodyDistance;
		this->body[i]->object.y = this->body[i - 1]->object.y;
		break;
	}
	this->body[i]->object.width = this->snakeWidth;
	this->body[i]->object.height = this->snakeHeight;
	this->body[i]->object.texture = this->snakeTexture;
	this->body[i]->direction = this->body[i - 1]->direction;
}

void Snake::Paint()
{
	for (auto& i : this->body)
		i->Paint();
}
