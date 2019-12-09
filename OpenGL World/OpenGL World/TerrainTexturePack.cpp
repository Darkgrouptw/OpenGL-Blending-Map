#include "TerrainTexturePack.h"

TerrainTexturePack::TerrainTexturePack(GLuint TextureLoc[], QString loc, QOpenGLShaderProgram *Program)
{
	qDebug("***** Loading Multitexture *****");
	BlendMap = new QOpenGLTexture(QImage(loc + "Images/Multitexture/blendMap.png"));
	qDebug("Loading blendMap");
	BgTexture = new QOpenGLTexture(QImage(loc + "Images/Multitexture/grassy.png"));
	qDebug("Loading BgTexture");
	rTexture = new QOpenGLTexture(QImage(loc + "Images/Multitexture/mud.png"));
	qDebug("Loading rTexture");
	gTexture = new QOpenGLTexture(QImage(loc + "Images/Multitexture/grassFlowers.png"));
	qDebug("Loading gTexture");
	bTexture = new QOpenGLTexture(QImage(loc + "Images/Multitexture/path.png"));
	qDebug("Loading bTexture");

	BlendMapLoc = TextureLoc[0];
	BgTextureLoc = TextureLoc[1];
	rTextureLoc = TextureLoc[2];
	gTextureLoc = TextureLoc[3];
	bTextureLoc = TextureLoc[4];

	Program->bind();
	Program->setUniformValue(BlendMapLoc, 0);
	Program->setUniformValue(BgTextureLoc, 1);
	Program->setUniformValue(rTextureLoc, 2);
	Program->setUniformValue(gTextureLoc, 3);
	Program->setUniformValue(bTextureLoc, 4);
	Program->release();

	initializeOpenGLFunctions();
}
TerrainTexturePack::~TerrainTexturePack()
{
}

void TerrainTexturePack::BindAllTexture()
{
	glActiveTexture(GL_TEXTURE0);
	BlendMap->bind();
	glActiveTexture(GL_TEXTURE1);
	BgTexture->bind();
	glActiveTexture(GL_TEXTURE2);
	rTexture->bind();
	glActiveTexture(GL_TEXTURE3);
	gTexture->bind();
	glActiveTexture(GL_TEXTURE4);
	bTexture->bind();
}

void TerrainTexturePack::ReleaseAllTexture()
{
	BlendMap->release();
	BgTexture->release();
	rTexture->release();
	gTexture->release();
	bTexture->release();
	glActiveTexture(GL_TEXTURE0);		//指回第0個位置 
}