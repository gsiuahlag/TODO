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
	Task* Ttask;
public slots:
	void OnBtnOk();
	void OnBtnCancel();
private:
private:
	Ui::ChangeTaskInfomationClass ui;
};
