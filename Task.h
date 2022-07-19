#ifndef _TASK_H
#define _TASK_H
#include <QDate>
enum MyUnit
{
	Year,Month,Week,Day,Hour
};
class Task
{
	struct MyTime
	{
		int Length;
		int Unit;
	};
public:
	Task()
	{
		this->Name = "NULL";
		this->Deadline = QDateTime(QDate::currentDate());
		this->Repeat = false;
		this->RepeatCount = 0;
		this->RepeatTime.Length = 0;
		this->RepeatTime.Unit = Year;
	}
	Task(int id, QString name, QDateTime deadline, bool repeat = false, int repeatcount = 0, int length = 0, MyUnit unit = Year)
	{
		this->Id = id;
		this->Name = name;
		this->Deadline = deadline;
		this->Repeat = repeat;
		this->RepeatCount = repeatcount;
		this->RepeatTime.Length = length;
		this->RepeatTime.Unit = unit;
	}
	QString getName()
	{
		return Name;
	}
	void setName(QString name)
	{
		this->Name = name;
	}
	int getId()
	{
		return Id;
	}
	QDateTime getDeadline()
	{
		return Deadline;
	}
	void setDeadline(QDateTime deadline)
	{
		this->Deadline = deadline;
	}
	bool getRepeat()
	{
		return Repeat;
	}
	void setRepeat(bool repeat)
	{
		this->Repeat = repeat;
	}
	int getRepeatCount()
	{
		return RepeatCount;
	}
	void setRepeatCount(int repeatcount)
	{
		this->RepeatCount = repeatcount;
	}
	MyTime getRepeatTime()
	{
		return RepeatTime;
	}
	void setRepeatTime(MyTime repeattime)
	{
		this->RepeatTime = repeattime;
	}
	//产生一个新的task副本，返回指针
	static Task* copyFromOldtask(Task* oldtask)
	{
		Task* newtask = new Task();
		newtask->setDeadline(oldtask->getDeadline());
		newtask->setName(oldtask->getName());
		newtask->setRepeat(oldtask->getRepeat());
		newtask->setRepeatCount(oldtask->getRepeatCount());
		newtask->setRepeatTime(oldtask->getRepeatTime());
		return newtask;
	}
	//利用新的task作为自己的值
	void parseFromNewtask(Task* newtask)
	{
		this->Name = newtask->getName();
		this->Deadline = newtask->getDeadline();
		this->Repeat = newtask->getRepeat();
		this->RepeatCount = newtask->getRepeatCount();
		this->RepeatTime = newtask->getRepeatTime();
	}
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	int RepeatCount;
	MyTime RepeatTime;

	
};
Q_DECLARE_METATYPE(Task*)



#endif // _TASK_H
