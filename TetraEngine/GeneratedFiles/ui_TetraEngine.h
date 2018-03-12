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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TetraEngineClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QScrollBar *horizontalScrollBar;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *TetraEngineClass)
    {
        if (TetraEngineClass->objectName().isEmpty())
            TetraEngineClass->setObjectName(QStringLiteral("TetraEngineClass"));
        TetraEngineClass->resize(600, 400);
        centralWidget = new QWidget(TetraEngineClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 201, 201));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalScrollBar = new QScrollBar(verticalLayoutWidget);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalScrollBar);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        TetraEngineClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TetraEngineClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        TetraEngineClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TetraEngineClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TetraEngineClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TetraEngineClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TetraEngineClass->setStatusBar(statusBar);
        toolBar = new QToolBar(TetraEngineClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        TetraEngineClass->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addSeparator();

        retranslateUi(TetraEngineClass);

        QMetaObject::connectSlotsByName(TetraEngineClass);
    } // setupUi

    void retranslateUi(QMainWindow *TetraEngineClass)
    {
        TetraEngineClass->setWindowTitle(QApplication::translate("TetraEngineClass", "TetraEngine", nullptr));
        label->setText(QApplication::translate("TetraEngineClass", "TextLabel", nullptr));
        toolBar->setWindowTitle(QApplication::translate("TetraEngineClass", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TetraEngineClass: public Ui_TetraEngineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TETRAENGINE_H
