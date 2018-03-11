/********************************************************************************
** Form generated from reading UI file 'TetraEngine.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TETRAENGINE_H
#define UI_TETRAENGINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TetraEngineClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TetraEngineClass)
    {
        if (TetraEngineClass->objectName().isEmpty())
            TetraEngineClass->setObjectName(QStringLiteral("TetraEngineClass"));
        TetraEngineClass->resize(600, 400);
        menuBar = new QMenuBar(TetraEngineClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        TetraEngineClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TetraEngineClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TetraEngineClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TetraEngineClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TetraEngineClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TetraEngineClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TetraEngineClass->setStatusBar(statusBar);

        retranslateUi(TetraEngineClass);

        QMetaObject::connectSlotsByName(TetraEngineClass);
    } // setupUi

    void retranslateUi(QMainWindow *TetraEngineClass)
    {
        TetraEngineClass->setWindowTitle(QApplication::translate("TetraEngineClass", "TetraEngine", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TetraEngineClass: public Ui_TetraEngineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TETRAENGINE_H
