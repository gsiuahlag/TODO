#include "AddTaskChild.h"

AddTaskChild::AddTaskChild(Task* task, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//显示原有信息
	ui.LabName->setText(task->getName());
	ui.LabDeadline->setText(task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
	father = task;
	//如果没有父亲节点的截止时间限制最大时间不能超过1年
	ui.Deadline->setMaximumDate(QDate::currentDate().addDays(365));
	//孩子时间不能超过父亲时间,最小时间不能超过当天
	if (task->getFatherTask() != NULL)
		ui.Deadline->setMaximumDateTime(task->getFatherTask()->getDeadline());
	ui.Deadline->setMinimumDateTime(QDateTime::currentDateTime());
	connect(ui.BtnOk, SIGNAL(clicked()), this, SLOT(OnBtnOk()));
	connect(ui.BtnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCancel()));
}
void AddTaskChild::OnBtnOk()
{
	Newprocedure = new Task(ui.Name->text(), ui.Deadline->dateTime(), father);
	accept();
}
void AddTaskChild::OnBtnCancel()
{
	reject();
}
AddTaskChild::~AddTaskChild()
{}
