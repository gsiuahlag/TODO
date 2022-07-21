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



//�����µ�task��Ϊ�Լ���ֵ
void Task::pasteFromNewtask(Task* newtask)
{
	this->Name = newtask->getName();
	this->Deadline = newtask->getDeadline();
	this->Repeat = newtask->getRepeat();
	this->RepeatTime = newtask->getRepeatTime();

	//�޸Ĳ��漰����Ͳ���֮��Ĺ�ϵ���ӽڵ��б�ָ��͸��ڵ�ָ�� ǳ����
	//this->procedureTask = newtask->getprocedureTask();
	//this->FatherTask = newtask->getFatherTask();
	emit ChangeTask(TodoArray, this);
}