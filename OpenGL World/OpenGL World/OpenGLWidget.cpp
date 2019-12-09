#include "OpenGLWidget.h"
QMatrix4x4	OpenGLWidget::projection;
QMatrix4x4	OpenGLWidget::view;

OpenGLWidget::OpenGLWidget(QWidget *parent = 0) :QOpenGLWidget(parent)
{
	QDir dir("Shaders");
	qDebug(qPrintable("Path: " + dir.currentPath()));

	if (!dir.exists())
		FileLocation = "../x64/Release/";
	else
		FileLocation = "";
	qDebug(qPrintable("File Location: " + FileLocation));
}
OpenGLWidget::~OpenGLWidget()
{
}
//GLuint id;
void OpenGLWidget::initializeGL()
{
	/*QImage img(FileLocation + "Models/fern.png");
	img.save(FileLocation + "Models/fern.png");*/
	light = new Light(QVector3D(20, 20, 20));

	initializeOpenGLFunctions();
	glClearColor(light->SkyColor.x(), light->SkyColor.y(), light->SkyColor.z(), light->SkyColor.w());

	//Depth 一樣的時候 要保留
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);

	//可以透明
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//後面的面不要畫
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	MapUI = new MiniMap(FileLocation);
	terrains = new Terrain(FileLocation, 0, 0, 450);						//-225 ~ 225 的正方形

}
void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();
	RenderMiniMap();
}

void OpenGLWidget::RenderScene()
{
	terrains->RenderTerrain(projection, view, light->Position, light->AmbientColor, light->DiffuseColor, light->SpecularColor, light->SkyColor);

}
void OpenGLWidget::RenderMiniMap()
{
	MapUI->Draw();
}