#include "MiniMap.h"

MiniMap::MiniMap(QString loc)
{
	qDebug("***** MiniMap *****");
	Program = new QOpenGLShaderProgram();
	Program->addShaderFromSourceFile(QOpenGLShader::Vertex,	loc + "Shaders/MiniMap.vsh");
	Program->addShaderFromSourceFile(QOpenGLShader::Fragment, loc + "Shaders/MiniMap.fsh");
	Program->link();

	PVMLoc = Program->uniformLocation("PVM");

	QVector<QVector3D> Vertex;
	Vertex.push_back(QVector3D(-1.1, -10, -1.1));
	Vertex.push_back(QVector3D(-1.1, -10, 1.1));
	Vertex.push_back(QVector3D(1.1, -10, 1.1));
	Vertex.push_back(QVector3D(1.1, -10, -1.1));

	QVector<QVector4D> FrontColor;
	FrontColor.push_back(QVector4D(0.6, 0.6, 0.6, 0.6));
	FrontColor.push_back(QVector4D(0.6, 0.6, 0.6, 0.6));
	FrontColor.push_back(QVector4D(0.6, 0.6, 0.6, 0.6));
	FrontColor.push_back(QVector4D(0.6, 0.6, 0.6, 0.6));

	QVector<QVector4D> BackColor;
	BackColor.push_back(QVector4D(1, 1, 1, 0.3));
	BackColor.push_back(QVector4D(1, 1, 1, 0.3));
	BackColor.push_back(QVector4D(1, 1, 1, 0.3));
	BackColor.push_back(QVector4D(1, 1, 1, 0.3));

	Projection.ortho(-3.2, 3.2, -1.8, 1.8, 0.1, 10);
	View.lookAt(QVector3D(0, -1, 0),
		QVector3D(0, -10, 0),
		QVector3D(0, -1, -1));
	FrontModel.translate(-2.65, 0, -1.25);
	FrontModel.scale(0.5, 1, 0.5);
	BackModel.translate(-2.65, 0, -1.25);
	BackModel.scale(0.5, 1, 0.5);
	FrontModel.scale(0.98, 1, 0.98);


	initializeOpenGLFunctions();
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(QVector3D), Vertex.constData(), GL_STATIC_DRAW);

	glGenBuffers(1, &FrontColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, FrontColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, FrontColor.size() * sizeof(QVector4D), FrontColor.constData(), GL_STATIC_DRAW);

	glGenBuffers(1, &BackColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BackColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, BackColor.size() * sizeof(QVector4D), BackColor.constData(), GL_STATIC_DRAW);

	//Font Setting
	qDebug("***** Font Setting *****");
	FontProgram = new QOpenGLShaderProgram();
	FontProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, loc + "Shaders/Font.vsh");
	FontProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, loc + "Shaders/Font.fsh");
	FontProgram->link();

	FontPVMLoc = FontProgram->uniformLocation("PVM");
	FontImage = new QOpenGLTexture(QImage(loc + "Images/Letter.png"));

	for (int i = 0; i <= 10; i++)
		FontPosition.push_back(QVector2D((i - (i >= 8) * 8) * 128, 256 + (i >= 8) * 128));			//For 0 ~ 9 and :
	FontPosition.push_back(QVector2D(0, 896));										//For X
	FontPosition.push_back(QVector2D(128, 896));									//For Y
	FontPosition.push_back(QVector2D(640, 128));									//For -

	#pragma region Font Setting
	QVector<QVector3D> FontVertex[12];
	for (int i = 0; i < 10; i++)
	{
		FontVertex[i].push_back(QVector3D(-1 + i * 0.2, -10, 0.8));
		FontVertex[i].push_back(QVector3D(-1 + i * 0.2, -10, 1));
		FontVertex[i].push_back(QVector3D(-0.8 + i * 0.2, -10, 1));
		FontVertex[i].push_back(QVector3D(-0.8 + i * 0.2, -10, 0.8));

		glGenBuffers(1, &FontVertexBuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, FontVertex[i].size() * sizeof(QVector3D), FontVertex[i].constData(), GL_STATIC_DRAW);
	}
	//////////////////////////////////////////////////////////////////////////
	//For -
	//////////////////////////////////////////////////////////////////////////
	FontVertex[10].push_back(QVector3D(-1 + 1.5 * 0.2, -10, 0.8));
	FontVertex[10].push_back(QVector3D(-1 + 1.5 * 0.2, -10, 1));
	FontVertex[10].push_back(QVector3D(-0.8 + 1.5 * 0.2, -10, 1));
	FontVertex[10].push_back(QVector3D(-0.8 + 1.5 * 0.2, -10, 0.8));
	glGenBuffers(1, &FontVertexBuffer[10]);
	glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[10]);
	glBufferData(GL_ARRAY_BUFFER, FontVertex[10].size() * sizeof(QVector3D), FontVertex[10].constData(), GL_STATIC_DRAW);

	FontVertex[11].push_back(QVector3D(-1 + 6.5 * 0.2, -10, 0.8));
	FontVertex[11].push_back(QVector3D(-1 + 6.5 * 0.2, -10, 1));
	FontVertex[11].push_back(QVector3D(-0.8 + 6.5 * 0.2, -10, 1));
	FontVertex[11].push_back(QVector3D(-0.8 + 6.5 * 0.2, -10, 0.8));
	glGenBuffers(1, &FontVertexBuffer[11]);
	glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[11]);
	glBufferData(GL_ARRAY_BUFFER, FontVertex[11].size() * sizeof(QVector3D), FontVertex[11].constData(), GL_STATIC_DRAW);


	//////////////////////////////////////////////////////////////////////////
	// Set UV Location
	//////////////////////////////////////////////////////////////////////////
	UVSetting(FontUV[0], 11);
	UVSetting(FontUV[1], 10);
	UVSetting(FontUV[2], 0);
	UVSetting(FontUV[3], 0);
	UVSetting(FontUV[4], 0);
	UVSetting(FontUV[5], 12);
	UVSetting(FontUV[6], 10);
	UVSetting(FontUV[7], 0);
	UVSetting(FontUV[8], 0);
	UVSetting(FontUV[9], 0);
	UVSetting(FontUV[10], 13);
	UVSetting(FontUV[11], 13);

	for (int i = 0; i < 12; i++)
	{
		glGenBuffers(1, &FontVertexUVBuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexUVBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, FontUV[i].size() * sizeof(QVector2D), FontUV[i].constData(), GL_STATIC_DRAW);
	}
	#pragma endregion
}
MiniMap::~MiniMap()
{
}

void MiniMap::Draw()
{
	DrawBack();
	DrawFont();
	DrawXY();
}
void MiniMap::DrawBack()
{
	Program->bind();
	Program->setUniformValue(PVMLoc, Projection * View * BackModel);

	//Back Color
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, BackColorBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	//Front Color
	Program->setUniformValue(PVMLoc, Projection * View * FrontModel);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, FrontColorBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	Program->release();
}
void MiniMap::DrawFont()
{
	UVSetting(FontUV[2], GetNumber(Player::Position().x(), 3));
	UVSetting(FontUV[3], GetNumber(Player::Position().x(), 2));
	UVSetting(FontUV[4], GetNumber(Player::Position().x(), 1));
	UVSetting(FontUV[7], GetNumber(Player::Position().z(), 3));
	UVSetting(FontUV[8], GetNumber(Player::Position().z(), 2));
	UVSetting(FontUV[9], GetNumber(Player::Position().z(), 1));
	for (int i = 2; i < 10  ; i++)
	{
		glGenBuffers(1, &FontVertexUVBuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexUVBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, FontUV[i].size() * sizeof(QVector2D), FontUV[i].constData(), GL_STATIC_DRAW);
		if (i == 4)
			i = 6;
	}
	FontProgram->bind();
	FontProgram->setUniformValue(FontPVMLoc, Projection * View * FrontModel);
	FontImage->bind();

	for (int i = 0; i < 10; i++)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexUVBuffer[i]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	if (Player::Position().x() < 0)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[10]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexUVBuffer[10]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	if (Player::Position().z() > 0)				//因為Z軸指向自己
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexBuffer[11]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, FontVertexUVBuffer[11]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	FontImage->release();
	FontProgram->release();
}
void MiniMap::DrawXY()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(Projection.constData());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((View * FrontModel).constData());

	glLineWidth(2);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(-1, -10, -0.8);
	glVertex3d(-0.8, -10, -0.8);

	glVertex3d(-0.8, -10, -0.8);
	glVertex3d(-0.85, -10, -0.85);

	glVertex3d(-0.8, -10, -0.8);
	glVertex3d(-0.85, -10, -0.75);
	glEnd();


	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(-1, -10, -0.8);
	glVertex3d(-1, -10, -1);

	glVertex3d(-1.05, -10, -0.95);
	glVertex3d(-1, -10, -1);

	glVertex3d(-0.95, -10, -0.95);
	glVertex3d(-1, -10, -1);
	glEnd();
}

void MiniMap::UVSetting(QVector<QVector2D> &input, int index)
{
	input.clear();
	input.push_back(QVector2D(FontPosition[index].x() / 1024, FontPosition[index].y() / 1024));
	input.push_back(QVector2D(FontPosition[index].x() / 1024, (FontPosition[index].y() + 128) / 1024));
	input.push_back(QVector2D((FontPosition[index].x() + 128) / 1024, (FontPosition[index].y() + 128) / 1024));
	input.push_back(QVector2D((FontPosition[index].x() + 128) / 1024, FontPosition[index].y() / 1024));
}
int MiniMap::GetNumber(float number, int getIndex)
{
	int temp = qRound(number);
		switch (getIndex)
		{
		case 3:
			return qAbs(temp / 100);
			break;
		case 2:
			return qAbs((temp % 100 - temp % 10) / 10);
			break;
		case 1:
			return qAbs(temp % 10);
			break;
		}
	return 0;
}

