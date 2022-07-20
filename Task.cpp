#include "Task.h"
int Task::taskNumber = 0;
Task::Task()
{
	this->Name = "NULL";
	this->Deadline = QDateTime(QDate::currentDate());
	this->Repeat = false;
	this->RepeatTime.Length = 0;
	this->RepeatTime.Unit = Year;
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


//利用新的task作为自己的值
void Task::parseFromNewtask(Task* newtask)
{
	this->Name = newtask->getName();
	this->Deadline = newtask->getDeadline();
	this->Repeat = newtask->getRepeat();
	this->RepeatTime = newtask->getRepeatTime();
	emit ChangeTask(TodoArray, this);
}