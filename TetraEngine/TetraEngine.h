#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TetraEngine.h"

class TetraEngine : public QMainWindow
{
	Q_OBJECT

public:
	TetraEngine(QWidget *parent = Q_NULLPTR);

private:
	Ui::TetraEngineClass ui;
};
