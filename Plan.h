#ifndef PLAN_H
#define PLAN_H
#include <QObject>
#include <QTime>
#include "Task.h"
class Plan :public QObject
{
	Q_OBJECT
public:
	static int planNumber;
private:
	int Id;
	QString Name;
	QTime Todotime;
	int taskId;
	bool finish;
	
public:
	Plan(QString name, QTime todotime, bool f = false, int taskid = 0);
	Plan(Task*);

	~Plan();

	bool isFinish();
	void setFinish(bool);

	int gettaskId();
	void settaskId(int id);

	int getId();
	void setId(int id);

    QString getName();
    void setName(QString Name);

	QTime getTodotime();
    void setTodotime(QTime Todotime);

};





#endif // PLAN_H
