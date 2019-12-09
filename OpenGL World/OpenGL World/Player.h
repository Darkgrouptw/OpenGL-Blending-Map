#pragma once
#include <QtMath>
#include <QVector3D>
class Player
{
public:
	Player(float, float, float);
	~Player();

	static void			MoveForward();
	static void			MoveBackward();
	static void			MoveLeft();
	static void			MoveRight();

	static void			TurnLeft();
	static void			TurnRight();

	static QVector3D	Position();
	static QVector3D	LookForward();
private:
	static float PosX, PosY, PosZ;
	static const float Speed;

	static int Angle;
	static const int TurnAngle;
};

