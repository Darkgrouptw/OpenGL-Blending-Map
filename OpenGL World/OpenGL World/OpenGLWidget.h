#pragma once
#include "Player.h"
#include "Light.h"
#include "MiniMap.h"
//#include "ModelOBJ.h"
#include "Terrain.h"

#include <QDir>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
class OpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions_4_3_Core
{
public:
	OpenGLWidget(QWidget *);
	~OpenGLWidget();

	void initializeGL();
	void paintGL();

	void RenderScene();
	void RenderMiniMap();

	//////////////////////////////////////////////////////////////////////////
	// Static 變數
	//////////////////////////////////////////////////////////////////////////
	static const int					WIDTH = 1600;
	static const int					HEIGTH = 800;
	static QMatrix4x4					projection;
	static QMatrix4x4					view;

private:
	QString								FileLocation;		//Image & Shaders的位置

	Light								*light;

	Terrain								*terrains;
	ModelOBJ							*Tree;
	ModelOBJ							*grass;

	MiniMap								*MapUI;
};

