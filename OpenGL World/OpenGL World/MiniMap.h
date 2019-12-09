#pragma once
#include "Player.h"

#include <QtMath>
#include <QVector>
#include <QMatrix4x4>

#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>
class MiniMap : public QOpenGLWidget , protected QOpenGLFunctions_4_3_Core
{
public:
	MiniMap(QString);
	~MiniMap();

	void Draw();

private:
	void DrawBack();
	void DrawFont();
	void DrawXY();
	void UVSetting(QVector<QVector2D> &, int);

	int GetNumber(float, int);

	QVector<QVector2D>				FontUV[12];					

	QOpenGLShaderProgram			*Program;
	GLuint							VertexBuffer;
	GLuint							FrontColorBuffer;
	GLuint							BackColorBuffer;
	
	//For Font letter
	QOpenGLTexture					*FontImage;
	QOpenGLShaderProgram			*FontProgram;
	GLuint							FontVertexBuffer[12];
	GLuint							FontVertexUVBuffer[12];
	QVector<QVector2D>				FontPosition;


	GLuint							PVMLoc ,FontPVMLoc;

	QMatrix4x4						Projection, View, BackModel, FrontModel;
};

