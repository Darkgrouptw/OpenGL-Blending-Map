#pragma once
#include <QVector3D>
#include <QVector4D>
class Light
{
public:
	Light(QVector3D);
	~Light();

	QVector3D			Position;
	QVector4D			AmbientColor;
	QVector4D			DiffuseColor;
	QVector4D			SpecularColor;
	QVector4D			SkyColor;
};

