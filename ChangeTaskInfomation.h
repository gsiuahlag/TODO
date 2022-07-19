#pragma once

#include <QDialog>
#include "ui_ChangeTaskInfomation.h"
#include "Task.h"
class ChangeTaskInfomation : public QDialog
{
	Q_OBJECT

public:
	ChangeTaskInfomation(Task* task, QWidget* parent = nullptr);
	~ChangeTaskInfomation();
public:
	Task* getTask();
public slots:
	void OnBtnOk();
	void OnBtnCancel();
private:
	Task* Ttask;
	void setTask(Task* t);
private:
	Ui::ChangeTaskInfomationClass ui;
};
