#include "AddTaskChild.h"

AddTaskChild::AddTaskChild(Task* task, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//��ʾԭ����Ϣ
	ui.LabName->setText(task->getName());
	ui.LabDeadline->setText(task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
	father = task;
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
