#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"

class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);

private:
    Ui::TODOClass ui;
};
