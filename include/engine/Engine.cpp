#include "Engine.hpp"

GameProcess::GameProcess() : snake()
{
	GLuint snakeTexture = this->loadTexture(CONSTANTS::SNAKE_TEXTURE);
	this->pointTexture = this->loadTexture(CONSTANTS::POINT_TEXTURE);
	this->snake.Start(0.0f, 0.0f,
		this->calibrateWidth(50), this->calibrateHeight(50),
		snakeTexture, Direction_t::UP);
}

void GameProcess::Display()
{
	// Background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Start painting
	this->snake.Paint();
	for (auto& i : this->points)
		if (i)
			i->Paint();
	// End painting
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void GameProcess::Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		this->snake.ChangeDirection(Direction_t::UP);
		break;
	case GLUT_KEY_DOWN:
		this->snake.ChangeDirection(Direction_t::DOWN);
		break;
	case GLUT_KEY_LEFT:
		this->snake.ChangeDirection(Direction_t::LEFT);
		break;
	case GLUT_KEY_RIGHT:
		this->snake.ChangeDirection(Direction_t::RIGHT);
		break;
	}
}

void GameProcess::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	}
}

void GameProcess::Time(int t)
{
	this->snake.Move();
	this->snake.isCanChangeDirection = true;
	for (auto& i : this->points)
	{
		if (i)
		{
			if (this->isCollision(*this->snake.body[0], *i))
			{
				this->snake.AddBodyPart();
				i.reset();
			}
		}
	}

	if (this->snake.body.size() > 2)
	{
		for (unsigned i = 2; i < this->snake.body.size() - 1; ++i)
		{
			if (isCollision(*this->snake.body[0], *this->snake.body[i]))
			{
				MessageBoxA(0, "Przegra³eœ, zjad³eœ samego siebie!", "Game Over", MB_OK);
				exit(0);
			}
		}
	}
	if (this->snake.body[0]->object.x > 1.0f || this->snake.body[0]->object.x < -1.0f
		|| this->snake.body[0]->object.y > 1.0f || this->snake.body[0]->object.y < -1.0f)
	{
		MessageBoxA(0, "Przegra³eœ, wyszed³eœ za plansze!", "Game Over", MB_OK);
		exit(0);
	}
}

void GameProcess::CreatePoint(int t)
{
	unsigned i = 0;
	for (unsigned j = 0; j < CONSTANTS::MAX_POINTS_ON_MAP; ++j)
	{
		if (this->points[j] == nullptr)
		{
			this->points[j] = std::make_unique<Point>();
			i = j;
			break;
		}
	}
	this->points[i]->object.width = this->calibrateWidth(50);
	this->points[i]->object.height = this->calibrateHeight(50);
	this->points[i]->object.x = this->calibratePosX(rand() % (CONSTANTS::WINDOW_WIDTH - 20u) + 10u);
	this->points[i]->object.y = this->calibratePosY(rand() % (CONSTANTS::WINDOW_HEIGHT - 20u) + 10u);
	this->points[i]->object.texture = this->pointTexture;
}

bool GameProcess::isCollision(Object_t& o1, Object_t& o2)
{
	if (o1.x < o2.x + o2.width && o1.x + o1.width > o2.x
		&& o1.y < o2.y + o2.height && o1.y + o1.height > o2.y)
		return true;
	else
		return false;
}

bool GameProcess::isCollision(Object& o1, Object& o2)
{
	return this->isCollision(o1.object, o2.object);
}

GLuint GameProcess::loadTexture(const char* filename)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;

	FILE* file;
	fopen_s(&file, filename, "rb");

	if (file == NULL) return false;

	if (fread(header, 1, 54, file) != 54)
	{
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*) & (header[0x0A]);
	imageSize = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);

	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return texture;
}


float GameProcess::calibrateWidth(float size)
{
	return size * (1.0f / (float)CONSTANTS::WINDOW_WIDTH);
}

float GameProcess::calibrateHeight(float size)
{
	return size * (1.0f / (float)CONSTANTS::WINDOW_HEIGHT);
}

float GameProcess::calibratePosX(float x)
{
	return x * (2.0f / (float)CONSTANTS::WINDOW_WIDTH) - 1.0f;
}

float GameProcess::calibratePosY(float y)
{
	return y * (2.0f / (float)CONSTANTS::WINDOW_HEIGHT) - 1.0f;
}