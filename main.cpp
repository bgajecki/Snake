#include <engine/Engine.hpp>
#include "constants.hpp"


GLuint LoadTexture(const char* filename)
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

float calibrateWidth(float size)
{
	return size * (1.0f / (float)WINDOW_WIDTH);
}

float calibrateHeight(float size)
{
	return size * (1.0f / (float)WINDOW_HEIGHT);
}

float calibratePosX(float x)
{
	return x * (2.0f / (float)WINDOW_WIDTH) - 1.0f;
}

float calibratePosY(float y)
{
	return y * (2.0f / (float)WINDOW_HEIGHT) - 1.0f;
}


Snake* snake;
std::array<std::unique_ptr<Point>, MAX_POINTS_ON_MAP> point;
GLuint pointTexture;

void Display()
{
	// Background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Start painting
	snake->Paint();
	for (auto& i : point)
		if(i)
			i->Paint();
	// End painting
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}


void Reshape(int width, int height)
{
	Display();
}

void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		snake->ChangeDirection(Direction_t::UP);
		break;
	case GLUT_KEY_DOWN:
		snake->ChangeDirection(Direction_t::DOWN);
		break;
	case GLUT_KEY_LEFT:
		snake->ChangeDirection(Direction_t::LEFT);
		break;
	case GLUT_KEY_RIGHT:
		snake->ChangeDirection(Direction_t::RIGHT);
		break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	}
}

void Loop(int t)
{
	glutPostRedisplay();
	glutTimerFunc(13, Loop, 0);
}

void Time(int t)
{
	snake->Move();
	snake->isCanChangeDirection = true;
	for (auto& i : point)
	{
		if (i)
		{
			if (isCollision(snake->body[0]->object, i->object))
			{
				snake->AddBodyPart();
				i.reset();
			}
		}
	}
	for (unsigned i = 2; i < snake->body.size() - 1; ++i)
	{
		if (isCollision(snake->body[0]->object, snake->body[i]->object))
		{
			MessageBoxA(0, "Przegra³eœ, zjad³eœ samego siebie!", "Game Over", MB_OK);
			exit(0);
		}
	}

	if (snake->body[0]->object.x > 1.0f || snake->body[0]->object.x < -1.0f
		|| snake->body[0]->object.y > 1.0f || snake->body[0]->object.y < -1.0f)
	{
		MessageBoxA(0, "Przegra³eœ, wyszed³eœ za plansze!", "Game Over", MB_OK);
		exit(0);
	}
	glutTimerFunc(120, Time, 0);
}

void CreatePoint(int t)
{
	unsigned i = 0;
	for (unsigned j = 0; j < MAX_POINTS_ON_MAP; ++j)
	{
		if (point[j] == nullptr)
		{
			point[j] = std::make_unique<Point>();
			i = j;
			break;
		}
	}
	point[i]->object.width = calibrateWidth(50);
	point[i]->object.height = calibrateHeight(50);
	point[i]->object.x = calibratePosX(rand() % (WINDOW_WIDTH - 20u) + 10u);
	point[i]->object.y = calibratePosY(rand() % (WINDOW_HEIGHT - 20u) + 10u);
	point[i]->object.texture = pointTexture;
	glutTimerFunc(2000, CreatePoint, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));

	int argc = 1;
	char* argv[1] = {(char*)""};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_NAME);
	pointTexture = LoadTexture("point.bmp");
	//
	GLuint texture = LoadTexture("snake.bmp");
	Snake sn(0.0f, 0.0f, 
		calibrateWidth(50), calibrateHeight(50), 
		texture, Direction_t::UP);
	snake = &sn;
	//
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(Special);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(13, Loop, 0);
	glutTimerFunc(120, Time, 0);
	glutTimerFunc(2000, CreatePoint, 0);
	glutMainLoop();
	return 0;

}