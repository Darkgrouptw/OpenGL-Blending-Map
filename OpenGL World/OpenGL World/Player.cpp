#include "Player.h"
float							Player::PosX = 0;
float							Player::PosY = 20;
float							Player::PosZ = 0;

int								Player::Angle = -90;

const float						Player::Speed = 2; //0.2f;
const int						Player::TurnAngle = 2;

void Player::MoveForward()
{
	PosX += Speed * qCos(Angle* M_PI /  180);
	PosZ += Speed * qSin(Angle* M_PI /  180);
}
void Player::MoveBackward()
{
	PosX -= Speed * qCos(Angle* M_PI /  180);
	PosZ -= Speed * qSin(Angle* M_PI /  180);
}
void Player::MoveLeft()
{
	PosX += Speed * qCos(((Angle + 270) % 360)* M_PI / 180);
	PosZ += Speed * qSin(((Angle + 270) % 360)* M_PI / 180);
}
void Player::MoveRight()
{
	PosX += Speed * qCos(((Angle + 90) % 360)* M_PI / 180);
	PosZ += Speed * qSin(((Angle + 90) % 360)* M_PI / 180);
}

void Player::TurnLeft()
{
	Angle = (Angle + 360 - TurnAngle) % 360;
}
void Player::TurnRight()
{
	Angle = (Angle + TurnAngle) % 360;
} 

QVector3D Player::Position()
{
	return QVector3D(PosX, PosY, PosZ);
}
QVector3D Player::LookForward()
{
	return QVector3D(PosX + Speed * qCos(Angle * M_PI / 180), PosY, PosZ + Speed * qSin(Angle * M_PI / 180));
}
