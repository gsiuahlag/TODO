#include "Task.h"
#include <QDebug>
#include <windows.h>
int Task::taskNumber = 0;
Task::Task()
{
	this->Id = ++taskNumber;
	this->Name = "NULL";
	this->Deadline = QDateTime(QDate::currentDate());
	this->Repeat = false;
	this->RepeatTime.Length = 0;
	this->RepeatTime.Unit = Year;
	this->procedureTask = new QList<Task *>();
	this->FatherTask = NULL;
	this->IsFinish = false;
}
Task::~Task()
{
	qDebug() << Name << QString::fromLocal8Bit("������");
	delete procedureTask;
}

QString Task::getName()
{
	return Name;
}
void Task::setName(QString name)
{
	this->Name = name;
}
int Task::getId()
{
	return Id;
}
QDateTime Task::getDeadline()
{
	return Deadline;
}
void Task::setDeadline(QDateTime deadline)
{
	this->Deadline = deadline;
}
bool Task::getRepeat()
{
	return Repeat;
}
void Task::setRepeat(bool repeat)
{
	this->Repeat = repeat;
}
QList<Task*>* Task::getprocedureTask()
{
	return procedureTask;
}
void Task::setprocedureTask(QList<Task*>* proceduretask)
{
	this->procedureTask = proceduretask;
}
Task* Task::getFatherTask()
{
	return FatherTask;
}
void Task::setFatherTask(Task* fathertask)
{
	this->FatherTask = fathertask;
}
bool Task::getIsFinish()
{
	return IsFinish;
}
void Task::setIsFinish(bool isfinish)
{
	this->IsFinish = isfinish;
}


//�����µ�task��Ϊ�Լ���ֵ
void Task::pasteFromNewtask(Task* newtask)
{
	this->Name = newtask->getName();
	this->Deadline = newtask->getDeadline();
	this->Repeat = newtask->getRepeat();
	this->RepeatTime = newtask->getRepeatTime();
	this->IsFinish = newtask->getIsFinish();
	//�޸Ĳ��漰����Ͳ���֮��Ĺ�ϵ���ӽڵ��б�ָ��͸��ڵ�ָ�� ǳ����
	//this->procedureTask = newtask->getprocedureTask();
	//this->FatherTask = newtask->getFatherTask();
	emit ChangeTask(TodoArray, this);
}
//���Լ����Լ����ӽڵ�ȫ����Ϊδ��ɣ��Լ��ĸ��ڵ�Ҳδ���
void Task::redoTask(TaskArray array)
{
	Task* var = this;
	//�������ڵ�
	while (var->getFatherTask())
	{
		var->getFatherTask()->setIsFinish(false);
		var = var->getFatherTask();
	}
	var = this;
	//�����ӽڵ�
	QQueue<Task*> q;
	q.push_back(var);
	
	while (!q.isEmpty())
	{
		var = q.front();
		q.pop_front();
		var->setIsFinish(false);
		if (array == TodoArray)
		{
			emit ChangeTask(TodoArray, var);
		}
		else if (array == FinishArray)
		{
			emit ChangeTask(FinishArray, var);
		}
		QListIterator<Task*> itor(*var->getprocedureTask());
		for (itor.toFront(); itor.hasNext();)
		{
			q.push_back(itor.next());
		}
	}

}

void Task::repeatOnce()
{
	int length = this->getRepeatTime().Length;
	switch (this->getRepeatTime().Unit)
	{
	case Year:
		this->setDeadline(this->getDeadline().addYears(length));
		break;
	case Month:
		this->setDeadline(this->getDeadline().addMonths(length));
		break;
	case Week:
		this->setDeadline(this->getDeadline().addDays(length * 7));
		break;
	case Day:
		//��ʱ���ó�Сʱ
		this->setDeadline(this->getDeadline().addSecs(length * 3600));
		break;
	case Hour:
		//��ʱ���ó���
		this->setDeadline(this->getDeadline().addSecs(length));
		break;
	default:
		break;
	}
	this->setIsFinish(false);
	emit ChangeTask(TodoArray, this);
	if (this->getprocedureTask()->size() != 0)
	{
		Task* var = this;
		//�ظ���ʱ����Ҫ���Լ����ӽڵ�ȫ����Ϊδ��ɰ����Լ�
		QQueue<Task*> q;
		q.push_back(var);
		while (!q.isEmpty())
		{
			var = q.front();
			q.pop_front();
			var->setIsFinish(false);
			emit ChangeTask(TodoArray, var);
			QListIterator<Task*> itor(*var->getprocedureTask());
			for (itor.toFront(); itor.hasNext();)
			{
				q.push_back(itor.next());
			}
		}
	}
}