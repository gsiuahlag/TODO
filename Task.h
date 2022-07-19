#ifndef _TASK_H
#define _TASK_H
#include <QDate>
class Task
{
public:
	Task()
	{
		this->Name = "NULL";
		this->Deadline = QDateTime(QDate::currentDate());
	}
	Task(int id, QString name, QDateTime deadline)
	{
		this->Id = id;
		this->Name = name;
		this->Deadline = deadline;
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
private:
	int Id;
	QString Name;
	QDateTime Deadline;
	bool Repeat;
	QTime RepeatTime;	
};
Q_DECLARE_METATYPE(Task*)



#endif // _TASK_H
