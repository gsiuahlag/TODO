#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <list>

#include "AfSocket.h"
#include "AfThread.h"
#include "Task.h"
#include <QObject>
#include <QTreeWidgetItem>
#include <QQueue>

class DataSource :public QObject
{
	Q_OBJECT
public:
	DataSource();
	~DataSource();
	Task* getFromTodoArray(int taskId);
	Task* getFromFinishArray(int taskId);
	Task* getFromArray(int taskId);
	void deleteFromTodoArray(int taskId);
	void deleteFromFinishArray(int taskId);
	void AddTodoArray(Task* task, TaskPlace place = Father, Task* farther = NULL);
	void AddFinishArray(Task* task, TaskPlace place, Task* father);
	Task* Translate(TaskArray a, TaskArray b, int taskId);
	Task* TaskFinishOnce(int taskId);
	void deleteTask(Task*, Task*);
signals:
	void DeleteTask(TaskArray, Task*);
	void AddTask(TaskArray, Task* son, TaskPlace = Father, Task* = NULL);
	void ChangeTask(TaskArray, Task*);
private:
	AfTcpSocket client_sock;
	std::list <Task*> taskTodoArray;
	std::list <Task*> taskFinishArray;
};







#endif // DATASOURCE_H
