#include "ChangeTaskInfomation.h"

ChangeTaskInfomation::ChangeTaskInfomation(Task* task, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//利用传递过来的task拷贝新的task
	Ttask = Task::copyFromOldtask(task);
	//显示原有信息
	ui.Name->setText(Ttask->getName());
	ui.Deadline->setDateTime(Ttask->getDeadline());
	if (Ttask->getRepeat() == true)
	{
		ui.Repeat->setCheckState(Qt::Checked);
	}
	else
	{
		ui.Repeat->setCheckState(Qt::Unchecked);
	}
	ui.Length->setValue(Ttask->getRepeatTime().Length);
	switch (Ttask->getRepeatTime().Unit)
	{
	case Year:
		ui.Unit->setCurrentIndex(0);
		break;
	case Month:
		ui.Unit->setCurrentIndex(1);
		break;
	case Week:
		ui.Unit->setCurrentIndex(2);
		break;
	case Day:
		ui.Unit->setCurrentIndex(3);
		break;
	case Hour:
		ui.Unit->setCurrentIndex(4);
		break;
	default:
		ui.Unit->setCurrentIndex(4);
		break;
	}
	connect(ui.BtnOk, SIGNAL(clicked()), this, SLOT(OnBtnOk()));
	connect(ui.BtnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCancel()));
}
Task* ChangeTaskInfomation::getTask()
{
	return this->Ttask;
}
void ChangeTaskInfomation::setTask(Task* t)
{
	this->Ttask = t;
}
void ChangeTaskInfomation::OnBtnOk()
{
	//获取修改后的信息，赋值给自己的task
	Ttask->setName(ui.Name->text());
	Ttask->setDeadline(ui.Deadline->dateTime());
	if (ui.Repeat->checkState() == Qt::Checked)
	{
		Ttask->setRepeat(true);
	}
	else
	{
		Ttask->setRepeat(false);
	}
	switch (ui.Unit->currentIndex())
	{
	case 0:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Year });
		break;
	case 1:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Month });
		break;
	case 2:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Week });
		break;
	case 3:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Day });
		break;
	case 4:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Hour });
		break;
	default:
		Ttask->setRepeatTime({ ui.Length->text().toInt(), Year });
		break;
	}
	accept();
}
void ChangeTaskInfomation::OnBtnCancel()
{
	reject();
}
ChangeTaskInfomation::~ChangeTaskInfomation()
{
	delete Ttask;
}
