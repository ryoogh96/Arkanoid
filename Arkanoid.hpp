#ifndef  ARKANOID_HPP
#define  ARKANOID_HPP

#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

class Arkanoid : private NonCopyable
{
private:
	RenderWindow mainWindow;
	Texture texture, tPaddle, tBall;
	Sprite *blocks, paddle, ball;
	Vector2f paddlePos;
	Vector2f ballPos;
	Vector2f ballSpeed;
	Vector2f ballSize;
	Vector2f boardSize;
	const int startBallPosX = 200, startBallPosY = 200;
	const float PlayerSpeed;
	const Time TimePerFrame;

	Font font;
	Time StatisticsUpdateTime;
	std::size_t StatisticsNumFrames;
	Text StatisticsText;
	bool isMovingLeft, isMovingRight;
	int Life;


private:
	void update();
	void processEvents();
	void render();

	void updateStatistics(Time elapsedTime);
	void handlePlayerInput(Keyboard::Key, bool isPressed);
	bool isInside();

public:
	Arkanoid();
	~Arkanoid();
	void run();
	
};

#endif // ! ARKANOID_HPP
