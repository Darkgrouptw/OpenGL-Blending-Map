/********************************************************************************
** Form generated from reading UI file 'openglworld.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENGLWORLD_H
#define UI_OPENGLWORLD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_OpenGLWorldClass
{
public:
    QWidget *centralWidget;
    OpenGLWidget *openGLWidget;

    void setupUi(QMainWindow *OpenGLWorldClass)
    {
        if (OpenGLWorldClass->objectName().isEmpty())
            OpenGLWorldClass->setObjectName(QStringLiteral("OpenGLWorldClass"));
        OpenGLWorldClass->resize(1600, 900);
        centralWidget = new QWidget(OpenGLWorldClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new OpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 0, 1600, 900));
        OpenGLWorldClass->setCentralWidget(centralWidget);

        retranslateUi(OpenGLWorldClass);

        QMetaObject::connectSlotsByName(OpenGLWorldClass);
    } // setupUi

    void retranslateUi(QMainWindow *OpenGLWorldClass)
    {
        OpenGLWorldClass->setWindowTitle(QApplication::translate("OpenGLWorldClass", "OpenGLWorld", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OpenGLWorldClass: public Ui_OpenGLWorldClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENGLWORLD_H
