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
enum TaskPlace
{
	Father,Son
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
	Task::Task(QString name, QDateTime deadline, Task* fathertask = NULL, bool repeat = false, int length = 0, MyUnit unit = Year, QList<Task*>* proceduretask = new QList<Task*>())
	{
		this->Id = ++taskNumber;
		this->Name = name;
		this->Deadline = deadline;
		this->Repeat = repeat;
		this->RepeatTime.Length = length;
		this->RepeatTime.Unit = unit;
		this->procedureTask = proceduretask;
		this->FatherTask = fathertask;
	}
	~Task();
	//getter and setter
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
	QList<Task*>* getprocedureTask();
	void setprocedureTask(QList<Task*>* proceduretask);
	Task* getFatherTask();
	void setFatherTask(Task* fathertask);

	//产生一个新的task副本，返回指针
	static Task* Task::copyFromOldtask(Task* oldtask)
	{
		Task* newtask = new Task();
		newtask->setDeadline(oldtask->getDeadline());
		newtask->setName(oldtask->getName());
		newtask->setRepeat(oldtask->getRepeat());
		newtask->setRepeatTime(oldtask->getRepeatTime());
		//修改不涉及任务和步骤之间的关系，子节点列表指针和父节点指针 浅拷贝
		//现阶段的拷贝仅仅进行任务数据修改，父节点数据可以不改
		//newtask->setprocedureTask(oldtask->getprocedureTask());
		//newtask->setFatherTask(oldtask->getFatherTask());
		return newtask;
	}
	//利用新的task作为自己的值
	void pasteFromNewtask(Task* newtask);
	static int taskNumber;
signals:
	void ChangeTask(TaskArray, Task*);
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	MyTime RepeatTime;
	QList <Task* > *procedureTask;
	Task* FatherTask;
};

#endif // _TASK_H
