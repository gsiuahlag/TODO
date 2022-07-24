#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <list>
#include <stack>
#include "AfSocket.h"
#include "AfThread.h"
#include "Task.h"
#include "Plan.h"
#include <QObject>
#include <QDebug>
class DataSource :public QObject
{
	Q_OBJECT
public:
	DataSource();
	~DataSource();
	Plan* getFromDailyArray(int planId);
	Task* getFromTodoArray(int taskId);
	Task* getFromFinishArray(int taskId);
	Task* getFromArray(int taskId);
	Task* GetTaskTopFather(int taskId);
	Task* GetTaskTopFather(Task* task);

	void deleteFromTodoArray(int taskId);
	void deleteFromFinishArray(int taskId);
	void AddTodoArray(Task* task, TaskPlace place = Father, Task* farther = NULL);
	void AddDailyArray(Plan*);
	void AddFinishArray(Task* task, TaskPlace place = Father, Task* farther = NULL);
	Task* Translate(TaskArray a, TaskArray b, int taskId);
	Task* Translate(TaskArray a, TaskArray b, Task* task);
	Task* TaskFinishOnce(int taskId);
	Task* TaskFinishOnce(Task* task);
	Task* TaskRedo(int taskId, TaskArray array);
	Plan* PlanRedo(int planId);
	bool ifTopTaskInDailyArray(int taskId);

	void deletePlan(int planId);
	void finishPlan(int planId);
	void deleteTask(Task*, Task*);
	void getTaskData();
	void getDailyDataFromServer(QDate);
	void PushDailyDataToServer(QDate);
	QDate getdailyDate();
	void setdailyDate(QDate);
	std::list<Plan*> getplanDailyArray();
	void setplanDailyArray(std::list<Plan*>);
public slots:
	void getTaskEmitChange(TaskArray, Task*);
signals:
	void DeleteTask(TaskArray, Task*);
	void AddTask(TaskArray, Task* son, TaskPlace = Father, Task* = NULL);
	void ChangeTask(TaskArray, Task*);
	void AddPlan(Plan*);
	void DeletePlan(Plan*, bool = false);
	void ChangePlan(Plan*);

private:
	QDate dailyDate;
	std::list <Plan*> planDailyArray;
	AfTcpSocket client_sock;
	std::list <Task*> taskTodoArray;
	std::list <Task*> taskFinishArray;
	//计划和任务的对照表
	//计划i对应根任务j
	std::map<int, int>  PlanAndTask;
};







#endif // DATASOURCE_H
