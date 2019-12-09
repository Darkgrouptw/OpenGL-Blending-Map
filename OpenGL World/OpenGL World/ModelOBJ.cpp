#include "ModelOBJ.h"

ModelOBJ::ModelOBJ(QString OBJList,
	QString loc,
	QString VshaderFileName,
	QString FshaderFileName,
	QString mtlName,
	bool needAllInfo)
{
	NeedAllInfo = needAllInfo;
	if (NeedAllInfo)
	{
		program = new QOpenGLShaderProgram();
		program->addShaderFromSourceFile(QOpenGLShader::Vertex, loc + "Shaders/" + VshaderFileName + ".vsh");
		program->addShaderFromSourceFile(QOpenGLShader::Fragment, loc + "Shaders/" + FshaderFileName + ".fsh");
		program->link();

		projectionLoc = program->uniformLocation("ProjectionM");
		viewLoc = program->uniformLocation("ViewM");
		modelLoc = program->uniformLocation("ModelM");

		useFakeLightLoc = program->uniformLocation("useFakeLight");

		lightPositionLoc = program->uniformLocation("LightPosition");
		lightALoc = program->uniformLocation("AmbientColor");
		lightDLoc = program->uniformLocation("DiffuseColor");
		lightSLoc = program->uniformLocation("SpecularColor");
		skyColorLoc = program->uniformLocation("SkyColor");

		M_KaID = program->uniformLocation("Material.Ka");
		M_KdID = program->uniformLocation("Material.Kd");
		M_KsID = program->uniformLocation("Material.Ks");
	}

	faces = new QVector<unsigned int>();
	mtls = new QVector<QString>();

	hasTexture = false;
	hasMTL = false;

	initializeOpenGLFunctions();
	Obj2Buffer(OBJList, loc, mtlName);
}
ModelOBJ::~ModelOBJ(void)
{
	if (NeedAllInfo)
		delete program;
}

void ModelOBJ::load2Stack(QString obj, int i){
	QVector<QVector3D> vertices;
	QVector<QVector2D> uvs;
	QVector<QVector3D> normals; // Won't be used at the moment.
	QVector<unsigned int> materialIndices;

	bool res = OBJLoader::loadOBJ(obj, vertices, uvs, normals, faces[i], mtls[i]);
	if (!res)
		qDebug(qPrintable("load failed"));

	glGenBuffers(1, &VertexStack);
	glBindBuffer(GL_ARRAY_BUFFER, VertexStack);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.constData(), GL_STATIC_DRAW);
	vertices_size = vertices.size();

	glGenBuffers(1, &uVertexStack);
	glBindBuffer(GL_ARRAY_BUFFER, uVertexStack);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(QVector2D), uvs.constData(), GL_STATIC_DRAW);
	uvs_size = uvs.size();

	glGenBuffers(1, &nVertexStack);
	glBindBuffer(GL_ARRAY_BUFFER, nVertexStack);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), normals.constData(), GL_STATIC_DRAW);
	normals_size = normals.size();
}
void ModelOBJ::Obj2Buffer(QString OBJList,
	QString loc,
	QString mtlName)
{
	QVector<QVector3D> Kds;
	QVector<QVector3D> Kas;
	QVector<QVector3D> Kss;
	QVector<QString> Materials;						//mtl-name
	QString texture = "";

	if (mtlName != "")
		hasMTL = OBJLoader::loadMTL(loc + "Models/" + mtlName + ".mtl", Kds, Kas, Kss, Materials, texture);

	if (hasMTL)
		for (int i = 0; i < Materials.size(); i++)
		{
			QString mtlname = Materials[i];
			//name       QVector3D
			KDs[mtlname] = Kds[i];
		}
	load2Stack(loc + "Models/" + OBJList + ".obj", 0);

	//Texture File
	QFile TextureFile(loc + "Models/" + texture);
	if (texture != "" && NeedAllInfo)
		if (TextureFile.exists())
		{
			hasTexture = true;
			Texture = new QOpenGLTexture(QImage(loc + "Models/" + texture));
		}
		else
			hasTexture = false;

	GLuint totalSize[3] = { 0, 0, 0 };
	for (int i = 0; i < PARTSNUM; i++)
	{
		totalSize[0] += vertices_size * sizeof(QVector3D);
		totalSize[1] += uvs_size * sizeof(QVector2D);
		totalSize[2] += normals_size * sizeof(QVector3D);
	}

	//generate VBO
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &uVertexBuffer);
	glGenBuffers(1, &nVertexBuffer);

	//bind vbo ,第一次bind也同等於 create VBO 
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[0], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVertexBuffer);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[1], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVertexBuffer);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[2], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_COPY_WRITE_BUFFER, VertexBuffer);
	glBindBuffer(GL_COPY_READ_BUFFER, VertexStack);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertices_size * sizeof(QVector3D));
	glInvalidateBufferData(VertexStack);//free vbo
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	glBindBuffer(GL_COPY_WRITE_BUFFER, uVertexBuffer);
	glBindBuffer(GL_COPY_READ_BUFFER, uVertexStack);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, uvs_size * sizeof(QVector2D));
	glInvalidateBufferData(uVertexStack);//free vbo
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	glBindBuffer(GL_COPY_WRITE_BUFFER, nVertexBuffer);
	glBindBuffer(GL_COPY_READ_BUFFER, nVertexStack);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, normals_size * sizeof(QVector3D));
	glInvalidateBufferData(nVertexStack);//free vbo
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

void ModelOBJ::DrawModelOBJ(QMatrix4x4 projection, QMatrix4x4 view, QVector3D LightPosition, QVector4D LightAColor,QVector4D LightDColor,QVector4D LightSColor ,QVector4D SkyColor)
{
	if (NeedAllInfo)
	{
		program->bind();

		program->setUniformValue(projectionLoc, projection);
		program->setUniformValue(viewLoc, view);
		program->setUniformValue(modelLoc, ModelMatrix);

		program->setUniformValue(useFakeLightLoc, 0);

		program->setUniformValue(lightPositionLoc, LightPosition);
		program->setUniformValue(lightALoc, LightAColor);
		program->setUniformValue(lightDLoc, LightDColor);
		program->setUniformValue(lightSLoc, LightSColor);
		program->setUniformValue(skyColorLoc, SkyColor);
	}
	#pragma region 傳送 Vertex, UV, Normal 的資訊
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uVertexBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, nVertexBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	int vertexIDoffset = 0;						//glVertexID's offset 
	QString mtlname;							//material name
	QVector3D Ks(1, 1, 1);						//because .mtl excluding specular , so give it here.

	if (hasMTL)
		for (int j = 0; j < mtls[0].size(); j++)
		{
			mtlname = mtls[0][j];

			if (NeedAllInfo)
			{
				//find the material diffuse color in map:KDs by material name.
				program->setUniformValue(M_KdID, KDs[mtlname]);
				program->setUniformValue(M_KsID, Ks);
			}

			if (hasTexture)
				Texture->bind();

			// (primitive   , glVertexID base , vertex count)
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[0][j + 1] * 3);

			if (hasTexture)
				Texture->release();

			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[0][j + 1] * 3;//glVertexID's base offset is face count*3
		}
	else
		glDrawArrays(GL_TRIANGLES, 0, faces[0][0] * 3);
	#pragma endregion
	if (NeedAllInfo)
		program->release();

}