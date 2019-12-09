#pragma once
#include "ModelOBJ.h"
#include "TerrainTexturePack.h"
#include <QtMath>
#include <QColor>
#include <QTime>					//For random seed
#include <QMatrix4x4>

#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>
class Terrain : protected QOpenGLFunctions_4_3_Core
{
public:
	Terrain(QString, int, int, int);
	~Terrain();

	void RenderTerrain(QMatrix4x4, QMatrix4x4, QVector3D, QVector4D, QVector4D, QVector4D, QVector4D);

private:
	void GenerateTerrian(QString);
	float GetTerrrainHeight(int, int, QImage);			// 拿Terrain的高度
	int	 clamp(int, int, int);							
	void GenerateItems(QString);						// 讀 Data/MapData.pack
	void ReadMapData(QString);

	QOpenGLShaderProgram	*MapItemsProgram;
	QVector<ModelOBJ *>		MapItems;
	QVector<int>			TextureChangeIndex;			// 貼圖些換的Index
	QVector<int>			useFakeLighNumber;	
	QVector<QOpenGLTexture *>	MapItemsTexture;

	QOpenGLShaderProgram	*Program;
	TerrainTexturePack		*TexturePack;
	int						CenterX, CenterZ;			// 中心座標
	int						Length;						// 長 & 寬
	const int				Gap = 10;					// 切出的每個小正方形，長寬各100

	//給地形的座標
	QVector<QVector3D>		TrianglesPos;
	QVector<QVector2D>		TrianglesUV;

	GLuint					ProjectionMLoc;
	GLuint					ViewMLoc;

	GLuint					lightALoc;
	GLuint					lightDLoc;
	GLuint					lightSLoc;
	GLuint					SkyColorLoc;
	GLuint					LightPosLoc;

	GLuint					VertexBuffer, UVBuffer;

	static const int		MAX_HEIGHT = 30;
};
// Read Map 的格式
//Tree Tree Tree.png 0	|| .obj .mtl picture useFakeLight (Tree & grassModel)
//X						|| X 組資料
//X X	 				|| Tranlate (X Z)
//X						|| Roate (只旋轉 Y)
//X						|| Scale