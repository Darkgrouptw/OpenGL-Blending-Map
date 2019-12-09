#include "openglworld.h"

OpenGLWorld::OpenGLWorld(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	/*showFullScreen();
	QDesktopWidget desktop;
	qDebug(qPrintable("Screen Width: " + QString::number(desktop.screenGeometry(0).width()) + " Height: " + QString::number(desktop.screenGeometry(0).height())));
	ui.openGLWidget->setGeometry(0, 0, desktop.screenGeometry(0).width(), desktop.screenGeometry(0).height());*/

	QTimer *timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(RefreshWidget()));
	timer->start(10);

	OpenGLWidget::projection.perspective(45, 16.f / 9, 0.1, 1000);
	OpenGLWidget::view.lookAt(
		Player::Position(),
		Player::LookForward(),
		QVector3D(0, 5, 0));
}

OpenGLWorld::~OpenGLWorld()
{

}

void OpenGLWorld::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_W:
		IsMoveForward = true;
		break;
	case Qt::Key_A:
		IsMoveLeft = true;
		break;
	case Qt::Key_S:
		IsMoveBackward = true;
		break;
	case Qt::Key_D:
		IsMoveRight = true;
		break;

	case Qt::Key_Left:
		IsTurnLeft = true;
		break;
	case Qt::Key_Right:
		IsTurnRight = true;
		break;
	}
}
void OpenGLWorld::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_W:
		IsMoveForward = false;
		break;
	case Qt::Key_A:
		IsMoveLeft = false;
		break;
	case Qt::Key_S:
		IsMoveBackward = false;
		break;
	case Qt::Key_D:
		IsMoveRight = false;
		break;

	case Qt::Key_Left:
		IsTurnLeft = false;
		break;
	case Qt::Key_Right:
		IsTurnRight = false;
		break;
	}
}

void OpenGLWorld::RefreshWidget()
{
	#pragma region Player Moving
	if (IsMoveForward)
		Player::MoveForward();
	if (IsMoveLeft)
		Player::MoveLeft();
	if (IsMoveBackward)
		Player::MoveBackward();
	if (IsMoveRight)
		Player::MoveRight();

	if (IsTurnLeft)
		Player::TurnLeft();
	if (IsTurnRight)
		Player::TurnRight();

	if (IsMoveForward || IsMoveBackward || IsMoveLeft || IsMoveRight || IsTurnLeft || IsTurnRight)
	{
		OpenGLWidget::view.setToIdentity();
		OpenGLWidget::view.lookAt(
			Player::Position(),
			Player::LookForward(),
			QVector3D(0, 1, 0));
	}
	#pragma endregion
	ui.openGLWidget->update();
}
