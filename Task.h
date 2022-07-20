#ifndef _TASK_H
#define _TASK_H
#include <QDate>
#include <QObject>
enum MyUnit
{
	Year,Month,Week,Day,Hour
};
enum TaskState
{
	Finish,Todo
};
enum TaskArray
{
	TodoArray, FinishArray
};
class Task :public QObject
{
	Q_OBJECT
public:
	struct MyTime
	{
		int Length;
		int Unit;
	};
	Task();
	Task::Task(QString name, QDateTime deadline, bool repeat = false, int length = 0, MyUnit unit = Year)
	{
		this->Id = ++taskNumber;
		this->Name = name;
		this->Deadline = deadline;
		this->Repeat = repeat;
		this->RepeatTime.Length = length;
		this->RepeatTime.Unit = unit;
	}
	QString getName();
	void setName(QString name);
	int getId();
	QDateTime getDeadline();
	void setDeadline(QDateTime deadline);
	bool getRepeat();
	void setRepeat(bool repeat);
	MyTime getRepeatTime()
	{
		return RepeatTime;
	}
	void setRepeatTime(MyTime repeattime)
	{
		this->RepeatTime = repeattime;
	}
	//产生一个新的task副本，返回指针
	static Task* Task::copyFromOldtask(Task* oldtask)
	{
		Task* newtask = new Task();
		newtask->setDeadline(oldtask->getDeadline());
		newtask->setName(oldtask->getName());
		newtask->setRepeat(oldtask->getRepeat());
		newtask->setRepeatTime(oldtask->getRepeatTime());
		return newtask;
	}
	//利用新的task作为自己的值
	void parseFromNewtask(Task* newtask);
	static int taskNumber;
signals:
	void ChangeTask(TaskArray, Task*);
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	MyTime RepeatTime;
};

#endif // _TASK_H
