#include "ChangeTaskInfomation.h"

ChangeTaskInfomation::ChangeTaskInfomation(Task* task, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//���ô��ݹ�����task�����µ�task
	Ttask = Task::copyFromOldtask(task);
	//��ʾԭ����Ϣ
	ui.Name->setText(Ttask->getName());
	ui.Deadline->setDateTime(Ttask->getDeadline());
	//���û�и��׽ڵ�Ľ�ֹʱ���������ʱ�䲻�ܳ���1��
	ui.Deadline->setMaximumDate(QDate::currentDate().addDays(365));
	//���ʱ�䲻�ܳ������׵Ľ�ֹʱ��,��Сʱ�䲻�ܳ�������
	if (task->getFatherTask() != NULL)
		ui.Deadline->setMaximumDateTime(task->getFatherTask()->getDeadline());
	ui.Deadline->setMinimumDateTime(QDateTime::currentDateTime());
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
	connect(ui.Repeat, SIGNAL(clicked(bool)), this, SLOT(StateChange()));
}
void ChangeTaskInfomation::StateChange()
{
	if (ui.Repeat->checkState()==Qt::Unchecked)
	{
		ui.Unit->setDisabled(true);
		ui.Length->setDisabled(true);
	}
	else
	{
		ui.Unit->setDisabled(false);
		ui.Length->setDisabled(false);
	}
}
void ChangeTaskInfomation::OnBtnOk()
{
	//��ȡ�޸ĺ����Ϣ����ֵ���Լ���task
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
