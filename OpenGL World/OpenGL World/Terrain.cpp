#include "Terrain.h"

Terrain::Terrain(QString loc,int x, int z, int l)
{
	CenterX = x;
	CenterZ = z;
	Length = l;
	GenerateTerrian(loc);
	//ReadMapData(loc);

	qDebug("***** Terrain Loading *****");
	Program = new QOpenGLShaderProgram();
	Program->addShaderFromSourceFile(QOpenGLShader::Vertex, loc + "Shaders/Terrain.vsh");
	Program->addShaderFromSourceFile(QOpenGLShader::Fragment, loc + "Shaders/Terrain.fsh");
	Program->link();

	ProjectionMLoc = Program->uniformLocation("ProjectionM");
	ViewMLoc = Program->uniformLocation("ViewM");

	LightPosLoc = Program->uniformLocation("LightPos");
	lightALoc = Program->uniformLocation("AmbientColor");
	lightDLoc = Program->uniformLocation("DiffuseColor");
	lightSLoc = Program->uniformLocation("SpecularColor");
	SkyColorLoc = Program->uniformLocation("SkyColor");
	
	//OpenGL Buffer
	initializeOpenGLFunctions();
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, TrianglesPos.size() * sizeof(QVector3D), TrianglesPos.constData(), GL_STATIC_DRAW);

	glGenBuffers(1, &UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, TrianglesUV.size() * sizeof(QVector2D), TrianglesUV.constData(), GL_STATIC_DRAW);


	GLuint TextureLoc[] = { Program->uniformLocation("BlendMap"),
		Program->uniformLocation("BgTexture"),
		Program->uniformLocation("rTexture"),
		Program->uniformLocation("gTexture"),
		Program->uniformLocation("bTexture")};
	TexturePack = new TerrainTexturePack(TextureLoc, loc, Program);
}
Terrain::~Terrain()
{
}
void Terrain::GenerateTerrian(QString loc)
{
	QImage HeightMap(loc + "Images/heightmap.png");
	//qDebug() << QString::number(GetTerrrainHeight(0, 0, HeightMap),'f',6);
	float scaleX, scaleY, UVGap;
	UVGap = (float)Gap / Length;
	for (int y = CenterZ - Length / 2; y < CenterZ + Length / 2; y += Gap)
	{
		scaleY = (float)(y + Length / 2) / Length;
		for (int x = CenterX - Length / 2; x < CenterX + Length / 2; x += Gap)
		{
			scaleX = (float)(x + Length / 2) / Length;

			TrianglesPos.append(QVector3D(x,		GetTerrrainHeight(x, y, HeightMap),				y));
			TrianglesPos.append(QVector3D(x,		GetTerrrainHeight(x, y + Gap, HeightMap),		y + Gap));
			TrianglesPos.append(QVector3D(x + Gap,	GetTerrrainHeight(x + Gap, y + Gap, HeightMap), y + Gap));

			TrianglesUV.append(QVector2D(scaleX,			scaleY));
			TrianglesUV.append(QVector2D(scaleX,			scaleY + UVGap));
			TrianglesUV.append(QVector2D(scaleX + UVGap,	scaleY +UVGap));

			TrianglesPos.append(QVector3D(x + Gap,	GetTerrrainHeight(x + Gap, y + Gap, HeightMap), y + Gap));
			TrianglesPos.append(QVector3D(x + Gap,	GetTerrrainHeight(x + Gap, y, HeightMap),		y));
			TrianglesPos.append(QVector3D(x,		GetTerrrainHeight(x, y, HeightMap),				y));

			TrianglesUV.append(QVector2D(scaleX + UVGap,	scaleY + UVGap));
			TrianglesUV.append(QVector2D(scaleX + UVGap,	scaleY));
			TrianglesUV.append(QVector2D(scaleX,			scaleY));
		}
	}
}
float Terrain::GetTerrrainHeight(int x, int y, QImage HeightMap)
{
	float scale = (float)HeightMap.height() / Length;
	QColor color(HeightMap.pixel(clamp(0, 255, qFloor(scale * (x + Length / 2)))
								, clamp(0, 255, qFloor(scale * (y + Length / 2)))));
	float Number = MAX_HEIGHT * color.red() / 256;

	return Number;
}

int Terrain::clamp(int min, int max, int Number)
{
	return (min < Number) ? (Number < max) ? Number : 255 : 0;
}

void Terrain::GenerateItems(QString loc)
{
	qsrand((unsigned int)QTime::currentTime().msec());		//For random seed
	int Top = 100 + qrand() % 100;						//最多幾個
	float temp;

	QFile File(loc + "Data/MapData.pack");
	if (File.open(QIODevice::ReadWrite))
	{
		QTextStream in(&File);
		#pragma region Tree Part
		in << "Tree Tree Tree.png 0\n";
		in << QString::number(Top) + "\n";
		for (int i = 0; i < Top; i++)
		{
			//Translate
			in << QString::number(qrand() % 400 - 200) + " " + QString::number(qrand() % 400 - 200) + "\n";
			//Rotate
			in << QString::number(qrand() % 360) + "\n";
			//Scale
			in << QString::number(2 + (float)(qrand() % 10) / 10) + "\n";
		}
		#pragma endregion
		#pragma region grassModel 
		Top = 100 + qrand() % 100;
		in << "grassModel grassModel grassTexture.png 1\n";
		in << QString::number(Top) + "\n";
		for (int i = 0; i < Top; i++)
		{
			//Translate
			in << QString::number(qrand() % 400 - 200) + " " + QString::number(qrand() % 400 - 200) + "\n";
			//Rotate
			in << QString::number(qrand() % 360) + "\n";
			//Scale
			in << "1\n";
		}
		#pragma endregion
		#pragma region fern
		Top = 100 + qrand() % 100;
		in << "fern fern fern.png 1\n";
		in << QString::number(Top) + "\n";
		for (int i = 0; i < Top; i++)
		{
			//Translate
			in << QString::number(qrand() % 400 - 200) + " " + QString::number(qrand() % 400 - 200) + "\n";
			//Rotate
			in << QString::number(qrand() % 360) + "\n";
			//Scale
			in << "0.4\n";
		}
#pragma endregion
		#pragma region lowPolyTree
		Top = 100 + qrand() % 100;
		in << "lowPolyTree lowPolyTree lowPolyTree.png 0\n";
		in << QString::number(Top) + "\n";
		for (int i = 0; i < Top; i++)
		{
			//Translate
			in << QString::number(qrand() % 400 - 200) + " " + QString::number(qrand() % 400 - 200) + "\n";
			//Rotate
			in << QString::number(qrand() % 360) + "\n";
			//Scale
			in << "0.2\n";
		}
		#pragma endregion
		#pragma region flower
		Top = 100 + qrand() % 100;
		in << "flower flower flower.png 1\n";
		in << QString::number(Top) + "\n";
		for (int i = 0; i < Top; i++)
		{
			//Translate
			in << QString::number(qrand() % 400 - 200) + " " + QString::number(qrand() % 400 - 200) + "\n";
			//Rotate
			in << QString::number(qrand() % 360) + "\n";
			//Scale
			in << "1\n";
		}
		#pragma endregion
		File.close();
	}
}
void Terrain::ReadMapData(QString loc)
{
	qDebug("***** Read Map Data Loading *****");
	QFile File(loc + "Data/MapData.pack");
	if (!File.open(QIODevice::ReadOnly))
	{
		qDebug("Read Map Data doesn't exit !! Now Creating");
		GenerateItems(loc);
		File.open(QIODevice::ReadOnly);
	}

	QString lineheader, OBJstring, MTLstring;
	QTextStream in(&File);

	MapItemsProgram = new QOpenGLShaderProgram();
	MapItemsProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, loc + "Shaders/Model_PhongShading.vsh");
	MapItemsProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, loc + "Shaders/Model_PhongShading.fsh");
	MapItemsProgram->link();

	int ModelCount, lastCount = 0;
	float temp[2];

	QTime timer;
	timer.start();
	while (1)
	{
		in >> OBJstring;
		if (OBJstring == "")
			break;
		else
		{
			in >> MTLstring;																				//MTL

			in >> lineheader;																				//Texture Name
			TextureChangeIndex.push_back(lastCount);
			MapItemsTexture.push_back(new QOpenGLTexture(QImage(loc + "Models/" + lineheader)));

			in >> lineheader;
			useFakeLighNumber.push_back(lineheader.toInt());

			in >> lineheader;																				//ModelCount
			ModelCount = lineheader.toInt();

			for (int i = 0; i < ModelCount; i++)
			{
				MapItems.push_back(new ModelOBJ(OBJstring, loc, "", "", MTLstring, false));

				in >> temp[0] >> temp[1];
				MapItems[lastCount + i]->ModelMatrix.translate(temp[0], 0, temp[1]);

				in >> temp[0];
				MapItems[lastCount + i]->ModelMatrix.rotate(temp[0], 0, 1, 0);

				in >> temp[0];
				MapItems[lastCount + i]->ModelMatrix.scale(temp[0], temp[0], temp[0]);
			}
			lastCount += ModelCount;
		}
	}
	TextureChangeIndex.push_back(lastCount);
	qDebug(qPrintable("Time: " + QString::number((float)timer.elapsed() / 1000,'f',3) + " sec"));
}

void Terrain::RenderTerrain(QMatrix4x4 Projection, QMatrix4x4 View, QVector3D LightPosition, QVector4D LightAColor, QVector4D LightDColor, QVector4D LightSColor, QVector4D SkyColor)
{
	Program->bind();
	Program->setUniformValue(ProjectionMLoc, Projection);
	Program->setUniformValue(ViewMLoc, View);

	Program->setUniformValue(LightPosLoc, LightPosition);
	Program->setUniformValue(lightALoc, LightAColor);
	Program->setUniformValue(lightDLoc, LightDColor);
	Program->setUniformValue(lightSLoc, LightSColor);
	Program->setUniformValue(SkyColorLoc, SkyColor);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	TexturePack->BindAllTexture();
	glDrawArrays(GL_TRIANGLES, 0, TrianglesPos.size());
	TexturePack->ReleaseAllTexture();

	Program->release();

	//Render Item On the Map
	/*glDisable(GL_CULL_FACE);
	MapItemsProgram->bind();

	MapItemsProgram->setUniformValue("ProjectionM", Projection);
	MapItemsProgram->setUniformValue("ViewM", View);

	MapItemsProgram->setUniformValue("LightPosition", LightPosition);
	MapItemsProgram->setUniformValue("AmbientColor", LightAColor);
	MapItemsProgram->setUniformValue("DiffuseColor", LightDColor);
	MapItemsProgram->setUniformValue("SpecularColor", LightSColor);
	MapItemsProgram->setUniformValue("SkyColor", SkyColor);
	int index = 0;
	for (int i = 0; i < (int)MapItems.size(); i++)
	{
		if (TextureChangeIndex[index] == i)
		{
			MapItemsTexture[index]->bind();
			MapItemsProgram->setUniformValue("useFakeLight", useFakeLighNumber[index]);
			index++;
		}
		MapItemsProgram->setUniformValue("ModelM", MapItems[i]->ModelMatrix);
		MapItems[i]->DrawModelOBJ(Projection, View, LightPosition, LightAColor, LightDColor, LightSColor, SkyColor);
	}
	MapItemsProgram->release();
	glEnable(GL_CULL_FACE);*/
}
