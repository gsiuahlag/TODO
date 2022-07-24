#ifndef _TASK_H
#define _TASK_H
#include <QDate>
#include <QObject>
#include <QQueue>
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
	Task::Task(QString name, QDateTime deadline, Task* fathertask = NULL, bool repeat = false, int length = 0, MyUnit unit = Year, bool inplan = false, QList<Task*>* proceduretask = new QList<Task*>())
	{
		this->Id = ++taskNumber;
		this->Name = name;
		this->Deadline = deadline;
		this->Repeat = repeat;
		this->RepeatTime.Length = length;
		this->RepeatTime.Unit = unit;
		this->procedureTask = proceduretask;
		this->FatherTask = fathertask;
		this->IsFinish = false;
		this->InPlan = false;
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
	bool getIsFinish();
	void setIsFinish(bool isfinish);
	bool getInPlan();
	void setInPlan(bool inplan);
	
	//����һ���µ�task����������ָ��
	static Task* Task::copyFromOldtask(Task* oldtask)
	{
		Task* newtask = new Task();
		newtask->setDeadline(oldtask->getDeadline());
		newtask->setName(oldtask->getName());
		newtask->setRepeat(oldtask->getRepeat());
		newtask->setRepeatTime(oldtask->getRepeatTime());
		newtask->setIsFinish(oldtask->getIsFinish());
		newtask->setInPlan(oldtask->getInPlan());
		//�޸Ĳ��漰����Ͳ���֮��Ĺ�ϵ���ӽڵ��б�ָ��͸��ڵ�ָ�� ǳ����
		//�ֽ׶εĿ��������������������޸ģ����ڵ����ݿ��Բ���
		//newtask->setprocedureTask(oldtask->getprocedureTask());
		//newtask->setFatherTask(oldtask->getFatherTask());
		return newtask;
	}
	//�����µ�task��Ϊ�Լ���ֵ
	void pasteFromNewtask(Task* newtask);
	void repeatOnce();
	void redoOnce();
	void redoTask(TaskArray);
	static int taskNumber;

signals:
	void ChangeTask(TaskArray, Task*);
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	bool InPlan;
	MyTime RepeatTime;
	QList <Task* > *procedureTask;
	Task* FatherTask;
	bool IsFinish;
};

#endif // _TASK_H
