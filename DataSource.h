#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <list>

#include "AfSocket.h"
#include "AfThread.h"
#include "Task.h"
#include <QObject>
#include <QTreeWidgetItem>
enum TaskArray
{
	TodoArray, FinishArray
};
class DataSource :public QObject
{
	Q_OBJECT
public:
	DataSource();
	~DataSource();
	Task* getFromTodoArray(int taskId);
	Task* getFromFinishArray(int taskId);
	void deleteFromTodoArray(int taskId, QTreeWidgetItem* t);
	void deleteFromFinishArray(int taskId, QTreeWidgetItem* t);
	void AddTodoArray(Task* task);
	void AddFinishArray(Task* task);
	Task* Translate(TaskArray a, TaskArray b, int taskId, QTreeWidgetItem* t);
	Task* TaskFinishOnce(int taskId , QTreeWidgetItem* t);
signals:
	void DeleteTask(TaskArray, Task*, QTreeWidgetItem*);
	void AddTask(TaskArray, Task*);
	void ChangeTask(TaskArray, Task*, QTreeWidgetItem*);
private:
	AfTcpSocket client_sock;
	std::list <Task*> taskTodoArray;
	std::list <Task*> taskFinishArray;
};







#endif // DATASOURCE_H
