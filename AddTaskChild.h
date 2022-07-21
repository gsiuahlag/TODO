#pragma once

#include <QDialog>
#include "ui_AddTaskChild.h"
#include "Task.h"
class AddTaskChild : public QDialog
{
	Q_OBJECT

public:
	AddTaskChild(Task* task, QWidget *parent = nullptr);
	~AddTaskChild();
	Task* Newprocedure;
	Task* father;
public slots:
	void OnBtnOk();
	void OnBtnCancel();
private:
	Ui::AddTaskChildClass ui;
};
