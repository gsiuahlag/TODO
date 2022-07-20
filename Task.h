#ifndef _TASK_H
#define _TASK_H
#include <QDate>

enum MyUnit
{
	Year,Month,Week,Day,Hour
};
enum TaskState
{
	Finish,Todo
};
class Task
{
public:
	struct MyTime
	{
		int Length;
		int Unit;
	};
	Task();
	Task::Task(QString name, QDateTime deadline, bool repeat = false, int repeatcount = 0, int length = 0, MyUnit unit = Year)
	{
		this->Id = ++taskNumber;
		this->Name = name;
		this->Deadline = deadline;
		this->Repeat = repeat;
		this->RepeatCount = repeatcount;
		this->RepeatTime.Length = length;
		this->RepeatTime.Unit = unit;
		this->LastRepeatCount = RepeatCount;
	}
	QString getName();
	void setName(QString name);
	int getId();
	QDateTime getDeadline();
	void setDeadline(QDateTime deadline);
	bool getRepeat();
	void setRepeat(bool repeat);
	int getRepeatCount();
	void setRepeatCount(int repeatcount);
	MyTime getRepeatTime()
	{
		return RepeatTime;
	}
	void setRepeatTime(MyTime repeattime)
	{
		this->RepeatTime = repeattime;
	}
	int getLastRepeatCount();
	void setLastRepeatCount(int lastrepeatcount);
	//产生一个新的task副本，返回指针
	static Task* Task::copyFromOldtask(Task* oldtask)
	{
		Task* newtask = new Task();
		newtask->setDeadline(oldtask->getDeadline());
		newtask->setName(oldtask->getName());
		newtask->setRepeat(oldtask->getRepeat());
		newtask->setRepeatCount(oldtask->getRepeatCount());
		newtask->setRepeatTime(oldtask->getRepeatTime());
		newtask->setLastRepeatCount(oldtask->getLastRepeatCount());
		return newtask;
	}
	//利用新的task作为自己的值
	void parseFromNewtask(Task* newtask);
	static int taskNumber;
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	int RepeatCount;
	MyTime RepeatTime;
	int LastRepeatCount;
	
};

#endif // _TASK_H
