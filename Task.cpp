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
	qDebug() << Name << QString::fromLocal8Bit("被销毁");
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


//利用新的task作为自己的值
void Task::pasteFromNewtask(Task* newtask)
{
	this->Name = newtask->getName();
	this->Deadline = newtask->getDeadline();
	this->Repeat = newtask->getRepeat();
	this->RepeatTime = newtask->getRepeatTime();
	this->IsFinish = newtask->getIsFinish();
	//修改不涉及任务和步骤之间的关系，子节点列表指针和父节点指针 浅拷贝
	//this->procedureTask = newtask->getprocedureTask();
	//this->FatherTask = newtask->getFatherTask();
	emit ChangeTask(TodoArray, this);
}
//把自己和自己的子节点全部置为未完成，自己的父节点也未完成
void Task::redoTask(TaskArray array)
{
	Task* var = this;
	//重做父节点
	while (var->getFatherTask())
	{
		var->getFatherTask()->setIsFinish(false);
		var = var->getFatherTask();
	}
	var = this;
	//重做子节点
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
		//暂时设置成小时
		this->setDeadline(this->getDeadline().addSecs(length * 3600));
		break;
	case Hour:
		//暂时设置成秒
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
		//重复的时候需要把自己的子节点全部置为未完成包括自己
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