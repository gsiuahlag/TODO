#include "Task.h"
#include <QDebug>
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



//利用新的task作为自己的值
void Task::pasteFromNewtask(Task* newtask)
{
	this->Name = newtask->getName();
	this->Deadline = newtask->getDeadline();
	this->Repeat = newtask->getRepeat();
	this->RepeatTime = newtask->getRepeatTime();

	//修改不涉及任务和步骤之间的关系，子节点列表指针和父节点指针 浅拷贝
	//this->procedureTask = newtask->getprocedureTask();
	//this->FatherTask = newtask->getFatherTask();
	emit ChangeTask(TodoArray, this);
}