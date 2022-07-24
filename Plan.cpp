#include "Plan.h"
int Plan::planNumber = 0;
Plan::Plan(QString name, QTime todotime, bool f, int taskid)
{
	this->Id = ++planNumber;
	this->Name = name;
	this->Todotime = todotime;
    this->taskId = taskid;
    this->finish = f;
}
Plan::Plan(Task* task)
{
	this->Id = ++planNumber;
	this->Name = task->getName();
	this->Todotime = task->getDeadline().time();
	this->taskId = task->getId();
	this->finish = false;

}
Plan::~Plan()
{

}

bool Plan::isFinish()
{
    return finish;
}
void Plan::setFinish(bool f)
{
    this->finish = f;
}
int Plan::getId()
{
    return Id;
}
void Plan::setId(int id)
{
    this->Id = id;
}
int Plan::gettaskId()
{
	return taskId;
}
void Plan::settaskId(int id)
{
	this->taskId = id;
}
QString Plan::getName()
{
    return Name;
}

void Plan::setName(QString Name)
{
    this->Name = Name;
}

QTime Plan::getTodotime()
{
    return Todotime;
}

void Plan::setTodotime(QTime Todotime)
{
    this->Todotime = Todotime;
}


