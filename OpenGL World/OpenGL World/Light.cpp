#include "Light.h"

Light::Light(QVector3D pos)
{
	Position = pos;
	AmbientColor = QVector4D(0.5, 0.5, 0.5, 1);;
	DiffuseColor = QVector4D(0.6, 0.6, 0.6, 1);
	SpecularColor = QVector4D(0.8, 0.8, 0.8, 1);
	SkyColor = QVector4D(0, 0, 0.3, 1);
}
