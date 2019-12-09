#pragma once
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
class TerrainTexturePack : protected QOpenGLFunctions_4_3_Core
{
public:
	TerrainTexturePack(GLuint *, QString, QOpenGLShaderProgram *);								//GetAllTextureLoc;
	~TerrainTexturePack();

	void BindAllTexture();
	void ReleaseAllTexture();

private:
	QOpenGLTexture *BlendMap;
	QOpenGLTexture *BgTexture;
	QOpenGLTexture *rTexture;
	QOpenGLTexture *gTexture;
	QOpenGLTexture *bTexture;

	GLuint	BlendMapLoc;
	GLuint	BgTextureLoc;
	GLuint	rTextureLoc;
	GLuint	gTextureLoc;
	GLuint	bTextureLoc;
};

