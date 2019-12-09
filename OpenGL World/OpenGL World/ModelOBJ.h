#pragma once
#include "OBJLoader.h"

#include <QOpenGLTexture>
#include <QFile>
#include <QMap>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
class ModelOBJ : protected QOpenGLFunctions_4_3_Core
{
public:
	ModelOBJ(QString,
		QString,
		QString,
		QString,
		QString,
		bool);																	//最後一個 bool，如果是 false 代表，要外包一個program & texture (畫 Terrain 的 Tree 時候要重複畫很多次)
	~ModelOBJ(void);

	void load2Stack(QString, int);
	void Obj2Buffer(QString,
		QString,
		QString);

	void DrawModelOBJ(QMatrix4x4, QMatrix4x4, QVector3D, QVector4D, QVector4D, QVector4D, QVector4D);

	QMatrix4x4						ModelMatrix;
private:
	QOpenGLShaderProgram			*program;
	GLuint							projectionLoc;
	GLuint							viewLoc;
	GLuint							useFakeLightLoc;
	GLuint							lightPositionLoc;
	GLuint							lightALoc, lightDLoc, lightSLoc;
	GLuint							skyColorLoc;								//For fog

	QOpenGLTexture					*Texture;
	bool							hasTexture;
	bool							hasMTL;

	bool							NeedAllInfo;								//不用 mtl & texture & program
#pragma region Shader Part
	const int						PARTSNUM = 1;

	GLuint							VertexBuffer;								//VBO
	GLuint							uVertexBuffer;								//uVBO
	GLuint							nVertexBuffer;								//nVBO

	//存這obj出進來的座標
	GLuint							VertexStack;
	GLuint							uVertexStack;
	GLuint							nVertexStack;

	GLuint							modelLoc;
	GLuint							M_KaID;
	GLuint							M_KdID;
	GLuint							M_KsID;

	int								vertices_size;
	int								uvs_size;
	int								normals_size;
	int								materialCount;

	QVector<QString>				*mtls;					//use material
	QVector<unsigned int>			*faces;					//face count
	QMap<QString, QVector3D>		KDs;					//mtl-name&Kd
	QMap<QString, QVector3D>		KSs;					//mtl-name&Ks
#pragma endregion
};

