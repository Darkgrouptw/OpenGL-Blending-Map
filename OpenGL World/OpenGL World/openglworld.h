#ifndef OPENGLWORLD_H
#define OPENGLWORLD_H

#include <QtWidgets/QMainWindow>
#include "ui_openglworld.h"

#include <QDesktopWidget>
#include "Player.h"
#include "OpenGLWidget.h"
#include <QTimer>
#include <QKeyEvent>

class OpenGLWorld : public QMainWindow
{
	Q_OBJECT

public:
	OpenGLWorld(QWidget *parent = 0);
	~OpenGLWorld();

	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
private:
	Ui::OpenGLWorldClass ui;

	bool				IsMoveForward = false;
	bool				IsMoveLeft = false;
	bool				IsMoveBackward = false;
	bool				IsMoveRight = false;
	bool				IsTurnLeft = false;
	bool				IsTurnRight = false;
private slots:
	void RefreshWidget();

};

#endif // OPENGLWORLD_H
