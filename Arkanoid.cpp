///////////////////////////////////////////////////////////////
// StringHelpers.hpp is originally from ///////////////////////
// SFML Game Development Examples files from github ///////////
// https://github.com/SFML/SFML-Game-Development-Book/blob/master/01_Intro/Source/Game.cpp
// and I also refered class structrues from here //////////////
// Got graphics resources from opengameart.org ////////////////
// 2018-03-09 ryoogh96@gmail.com //////////////////////////////
///////////////////////////////////////////////////////////////
#include "Arkanoid.hpp"
#include "StringHelpers.hpp"
#include <iostream>
using namespace sf;


Arkanoid::Arkanoid()
	: mainWindow(VideoMode(1024, 768), "Arkanoid"),
	paddlePos(160, 500),
	ballPos(startBallPosX, startBallPosY),
	ballSpeed(5,5),
	ballSize(6,6),
	boardSize(300, 600),
	PlayerSpeed(6.0f),
	TimePerFrame(seconds(1.0f / 60.0f)),
	isMovingLeft(false), isMovingRight(false),
	Life(10)
{
	mainWindow.setFramerateLimit(60);

	blocks = new Sprite[25];

	texture.loadFromFile("Breakout-B/Breakout-001-B.png");
	tPaddle.loadFromFile("Breakout-B/paddle.png");
	tBall.loadFromFile("Breakout-B/ball.png");

	paddle.setTexture(tPaddle);
	ball.setTexture(tBall);

	int srcX = 0, srcY = 0;
	int destX = 32, destY = 16;
	for (int i = 0; i < 25; i++)
	{
		blocks[i].setTexture(texture);
		blocks[i].setTextureRect(IntRect(srcX, srcY, destX, destY));
		blocks[i].setPosition(srcX + 100, srcY);
		if (srcX < 160)
		{
			srcX += 32;
			destX += 32;
		}
		else
		{
			srcX = 0; srcY += 16;
			destX = 32; destY += 16;
		}
	}

	font.loadFromFile("Media/Sansation.ttf");
	StatisticsText.setFont(font);
	StatisticsText.setPosition(500, 5);
	StatisticsText.setCharacterSize(30);

	paddle.setPosition(paddlePos);
	ball.setPosition(ballPos);

	
}

Arkanoid::~Arkanoid()
{
	//_CrtIsValidHeapPointer error
	//delete blocks;
}

void Arkanoid::run()
{
	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;
	while (mainWindow.isOpen())
	{
		Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update();
		}

		updateStatistics(elapsedTime);
		render();
	}

}


void Arkanoid::update()
{
	srand(time(0));

	// Detect coolsion detection with ball and blocks
	for (int i = 0; i < 25; i++)
	{
		if (FloatRect(ballPos, ballSize).intersects(blocks[i].getGlobalBounds()))
		{
			blocks[i].setPosition(-100, 0);
			//ballSpeed.x -= 5; ballSpeed.y -= 5;
		}
	}

	// ball moves with gravity 
	if (isInside())
	{
		if (ballPos.x > boardSize.x)
			ballSpeed.x -= 5;
		else if (ballPos.x < 0)
			ballSpeed.x += 5;

		if (ballPos.y < 0)
			ballSpeed.y += 5;
		

	}
	// set it start position
	else
	{
		ballPos.x = startBallPosX; ballPos.y = startBallPosY;
		ballSpeed.x = 5; ballSpeed.y = 5;
	}
	

	if (FloatRect(ballPos, ballSize).intersects(paddle.getGlobalBounds()))
	{
		if((rand() % 101) % 2 )
			ballSpeed.x += rand() % 6; 
		else ballSpeed.x -= rand() % 6;
			
		ballSpeed.y -= rand() % 5;
	}

	ballPos.x += ballSpeed.x;
	ballPos.y += ballSpeed.y;

	ball.setPosition(ballPos);

	if (isMovingLeft)  paddle.move(-PlayerSpeed, 0);
	if (isMovingRight) paddle.move(PlayerSpeed, 0);
}

void Arkanoid::processEvents()
{
	Event event;
	while (mainWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
		case Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case Event::Closed:
			mainWindow.close();
			break;
		}
		
	}
}

void Arkanoid::render()
{
	mainWindow.clear(Color::Black);

	for (int i = 0; i < 25; i++)
		mainWindow.draw(blocks[i]);

	mainWindow.draw(ball);
	mainWindow.draw(paddle);
	mainWindow.draw(StatisticsText);

	mainWindow.display();
}

void Arkanoid::handlePlayerInput(Keyboard::Key key, bool isPressed)
{
	
	if (key == Keyboard::A)
		isMovingLeft = isPressed;
	else if (key == Keyboard::D)
		isMovingRight = isPressed;
}

bool Arkanoid::isInside()
{
	// Detect when the ball is out of board
	if (ballPos.x > boardSize.x, ballPos.y > boardSize.y)
	{
		Life -= 1;
		return false;
	}
		
	else return true;
}

// shows FPS, Time, Life from SFML Game Development Example 01
// https://github.com/SFML/SFML-Game-Development-Book/blob/master/01_Intro/Source/Game.cpp
void Arkanoid::updateStatistics(Time elapsedTime)
{
	StatisticsUpdateTime += elapsedTime;
	StatisticsNumFrames += 1;

	if (StatisticsUpdateTime >= seconds(1.0f))
	{
		StatisticsText.setString(
			"FPS = " + toString(StatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(StatisticsUpdateTime.asMicroseconds() / StatisticsNumFrames) + "us\n" +
			"Life = " + toString(Life));

		StatisticsUpdateTime -= seconds(1.0f);
		StatisticsNumFrames = 0;
	}
}