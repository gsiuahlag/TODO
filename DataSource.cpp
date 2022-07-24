#include "DataSource.h"
DataSource::DataSource()
{
}
DataSource::~DataSource()
{}
QDate DataSource::getdailyDate()
{
	return this->dailyDate;
}
void DataSource::setdailyDate(QDate dailyDate)
{
	//保存数据到服务器
	this->PushDailyDataToServer(this->dailyDate);
	//清空现有数据
	for each (Plan * var in planDailyArray)
	{
		delete var;
	}
	planDailyArray.clear();
	DeletePlan(NULL, true);
	//日期修改以后，需要把每日任务的数组换掉
	this->dailyDate = dailyDate;
	this->getDailyDataFromServer(dailyDate);
	
}/*
std::list<Plan*> DataSource::getplanDailyArray()
{
	return planDailyArray;
}*/
void DataSource::setplanDailyArray(std::list<Plan*> planDailyArray)
{
	this->planDailyArray = planDailyArray;
}
void DataSource::getDailyDataFromServer(QDate date)
{
	//测试时间7.23
	QDate d1 = QDate::currentDate();
	//测试数据
	std::list<Plan*> p1;
	p1.push_back(new Plan("p1.plan1", QTime::currentTime()));
	p1.push_back(new Plan("p1.plan2", QTime::currentTime()));
	p1.push_back(new Plan("p1.plan3", QTime::currentTime()));
	std::list<Plan*> p2;
	p2.push_back(new Plan("p2.plan1", QTime::currentTime()));
	p2.push_back(new Plan("p2.plan2", QTime::currentTime()));
	p2.push_back(new Plan("p2.plan3", QTime::currentTime()));
	std::list<Plan*> p3;
	p3.push_back(new Plan("p3.plan1", QTime::currentTime()));
	p3.push_back(new Plan("p3.plan2", QTime::currentTime()));
	p3.push_back(new Plan("p3.plan3", QTime::currentTime()));
	std::list<Plan*> p4;
	p4.push_back(new Plan("p4.plan1", QTime::currentTime()));
	p4.push_back(new Plan("p4.plan2", QTime::currentTime()));
	p4.push_back(new Plan("p4.plan3", QTime::currentTime()));
	std::list<Plan*> p;
	if (date == d1)
	{
		for each (Plan * plan in p1)
		{
			AddDailyArray(plan);
		}
	}
	else if (date == d1.addDays(1))
	{
		for each (Plan * plan in p2)
		{
			AddDailyArray(plan);
		}
	}
	else if (date == d1.addDays(2))
	{
		for each (Plan * plan in p3)
		{
			AddDailyArray(plan);
		}
	}
	else if (date == d1.addDays(3))
	{
		for each (Plan * plan in p4)
		{
			AddDailyArray(plan);
		}
	}
	else
	{
		for each (Plan * plan in p)
		{
			AddDailyArray(plan);
		}
	}

}
void DataSource::PushDailyDataToServer(QDate date)
{
	//保存当前日期的数据
}
Task* DataSource::GetTaskTopFather(int taskId)
{
	Task* var = getFromArray(taskId);
	while (var->getFatherTask())
	{
		var = var->getFatherTask();
	}
	return var;
}
Task* DataSource::GetTaskTopFather(Task* task)
{
	while (task->getFatherTask())
	{
		task = task->getFatherTask();
	}
	return task;
}
Task* DataSource::TaskFinishOnce(int taskId)
{
	Task* var = NULL;
	Task* ret = getFromTodoArray(taskId);
	var = ret;
	//任务或步骤完成，任务或步骤的子节点全部完成
	QQueue<Task*> q;
	q.push_back(var);
	while (!q.isEmpty())
	{
		var = q.front();
		q.pop_front();
		var->setIsFinish(true);
		emit ChangeTask(TodoArray, var);
		QListIterator<Task*> itor(*var->getprocedureTask());
		for (itor.toFront(); itor.hasNext();)
		{
			q.push_back(itor.next());
		}
	}
	//如果是根节点则需要移动到finish中
	if (ret->getFatherTask() == NULL)
	{
		if (ret->getRepeat() == false)
		{
			//根节点完成要转移到完成列表
			Translate(TodoArray, FinishArray, ret);
		}
		else
		{
			ret->repeatOnce();
		}
	}
	return ret;
}
Task* DataSource::TaskFinishOnce(Task* task)
{
	Task* var = NULL;
	Task* ret = task;
	var = ret;
	//任务或步骤完成，任务或步骤的子节点全部完成
	QQueue<Task*> q;
	q.push_back(var);
	while (!q.isEmpty())
	{
		var = q.front();
		q.pop_front();
		var->setIsFinish(true);
		emit ChangeTask(TodoArray, var);
		QListIterator<Task*> itor(*var->getprocedureTask());
		for (itor.toFront(); itor.hasNext();)
		{
			q.push_back(itor.next());
		}
	}
	//如果是根节点则需要移动到finish中
	if (ret->getFatherTask() == NULL)
	{
		if (ret->getRepeat() == false)
		{
			//根节点完成要转移到完成列表
			Translate(TodoArray, FinishArray, ret);
		}
		else
		{
			ret->repeatOnce();
		}
	}
	return ret;
}
Task* DataSource::TaskRedo(int taskId, TaskArray array)
{
	Task* var = NULL;
	if (array == TodoArray)
	{
		//在TodoArray中重做，一定是步骤,此时不需要转移数据
		var = getFromTodoArray(taskId);
		//重做步骤步骤的所有子步骤全部重做
		var->redoTask(TodoArray);
		//步骤包括步骤的父节点也得重做		
	}
	else if (array == FinishArray)
	{
		//在FinishArray中重做，无论是步骤重做还是任务重做，都需要把根任务重做
		var = getFromFinishArray(taskId);
		var->redoTask(FinishArray);
		if (var->getFatherTask() == NULL)//如果自己本身就是根节点，直接转移自己就好了
		{
			Translate(FinishArray, TodoArray, var);
		}
		else
		{
			var = GetTaskTopFather(var);//获取它的最高根节点
			Translate(FinishArray, TodoArray, var);//转移
		}
	}
	return var;
}
Plan* DataSource::PlanRedo(int planId)
{
	Plan* plan = getFromDailyArray(planId);
	plan->setFinish(false);
	emit ChangePlan(plan);
	return NULL;
}
bool DataSource::ifTopTaskInDailyArray(int taskId)
{
	for each (Plan* var in planDailyArray)
	{
		if (var->gettaskId() == taskId)
		{
			return true;
		}
	}
	return false;
}

void DataSource::getTaskData()
{
	//从服务器获取数据
	std::list<Task* > todo;
	std::list<Task* > finish;
	//临时数据
	Task* task = new Task("a1", QDateTime::currentDateTime().addDays(10), NULL, false);
	todo.push_back(task);
	Task* task2 = new Task("a1.2", QDateTime::currentDateTime().addDays(8), task, false);
	Task* task3 = new Task("a1.3", QDateTime::currentDateTime().addDays(7), task, false);
	task->getprocedureTask()->push_back(new Task("a1.1", QDateTime::currentDateTime(), task, false));
	task->getprocedureTask()->push_back(task2);
	task->getprocedureTask()->push_back(task3);
	task2->getprocedureTask()->push_back(new Task("a1.2.1", QDateTime::currentDateTime().addDays(5), task2, false));
	task2->getprocedureTask()->push_back(new Task("a1.2.2", QDateTime::currentDateTime().addDays(4), task2, false));
	task3->getprocedureTask()->push_back(new Task("a1.3.1", QDateTime::currentDateTime().addDays(3), task3, false));
	task = new Task("test1", QDateTime::currentDateTime().addSecs(50), NULL, true, 10, Hour);
	todo.push_back(task);
	task = new Task("test2", QDateTime::currentDateTime().addSecs(30), NULL, false);
	todo.push_back(task);
	task = new Task("test3", QDateTime::currentDateTime().addSecs(15), NULL, true, 5, Day);
	todo.push_back(task);

	task = new Task("b1", QDateTime::currentDateTime(), NULL, false);
	finish.push_back(task);
	task2 = new Task("b1.2", QDateTime::currentDateTime(), task, false);
	task3 = new Task("b1.3", QDateTime::currentDateTime(), task, false);
	task->getprocedureTask()->push_back(new Task("b1.1", QDateTime::currentDateTime(), task, false));
	task->getprocedureTask()->push_back(task2);
	task->getprocedureTask()->push_back(task3);
	task2->getprocedureTask()->push_back(new Task("b1.2.1", QDateTime::currentDateTime(), task2, false));
	task2->getprocedureTask()->push_back(new Task("b1.2.2", QDateTime::currentDateTime(), task2, false));
	task3->getprocedureTask()->push_back(new Task("b1.3.1", QDateTime::currentDateTime(), task3, false));

	for (std::list<Task*>::iterator it = todo.begin(); it != todo.end(); ++it)
	{
		std::stack<Task*> st;
		Task* p;
		st.push(*it);
		this->AddTodoArray(*it);
		while (!st.empty())
		{
			p = st.top();
			st.pop();
			QListIterator<Task*> itor(*p->getprocedureTask());
			for (itor.toBack(); itor.hasPrevious();)
			{
				Task* t = itor.previous();
				connect(t, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
				st.push(t);
			}
		}
	}
	for (std::list<Task*>::iterator it = finish.begin(); it != finish.end(); ++it)
	{
		std::stack<Task*> st;
		Task* p;
		st.push(*it);
		this->AddFinishArray(*it);
		while (!st.empty())
		{
			p = st.top();
			st.pop();
			QListIterator<Task*> itor(*p->getprocedureTask());
			for (itor.toBack(); itor.hasPrevious();)
			{
				Task* t = itor.previous();
				connect(t, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
				st.push(t);
			}
		}
	}
}
Task* DataSource::getFromArray(int taskId)
{
	QQueue<Task*> q;
	Task* son = NULL;
	Task* task = NULL;
	for each (Task * father in taskTodoArray)
	{
		q.push_back(father);
		while (!q.isEmpty())
		{
			task = q.front();
			q.pop_front();
			if (task->getId() == taskId)
			{
				return task;
			}
			QListIterator<Task*> itor(*task->getprocedureTask());
			for (itor.toFront(); itor.hasNext();)
			{
				q.push_back(itor.next());
			}
		}
	}
	for each (Task * father in taskFinishArray)
	{
		q.push_back(father);
		while (!q.isEmpty())
		{
			task = q.front();
			q.pop_front();
			if (task->getId() == taskId)
			{
				return task;
			}
			QListIterator<Task*> itor(*task->getprocedureTask());
			for (itor.toFront(); itor.hasNext();)
			{
				q.push_back(itor.next());
			}
		}
	}
	return NULL;
}
Task* DataSource::getFromTodoArray(int taskId)
{
	QQueue<Task*> q;
	Task* son = NULL;
	Task* task = NULL;
	for each (Task * father in taskTodoArray)
	{
		q.push_back(father);
		while (!q.isEmpty())
		{
			task = q.front();
			q.pop_front();
			if (task->getId() == taskId)
			{
				return task;
			}
			QListIterator<Task*> itor(*task->getprocedureTask());
			for (itor.toFront(); itor.hasNext();)
			{
				Task* temp = itor.next();
				if (temp->getId() == taskId)
				{
					return temp;
				}
				q.push_back(temp);
			}
		}
	}
	return NULL;
}
Task* DataSource::getFromFinishArray(int taskId)
{
	QQueue<Task*> q;
	Task* son = NULL;
	Task* task = NULL;
	for each (Task * father in taskFinishArray)
	{
		q.push_back(father);
		while (!q.isEmpty())
		{
			task = q.front();
			q.pop_front();
			if (task->getId() == taskId)
			{
				return task;
			}
			QListIterator<Task*> itor(*task->getprocedureTask());
			for (itor.toFront(); itor.hasNext();)
			{
				Task* temp = itor.next();
				if (temp->getId() == taskId)
				{
					return temp;
				}
				q.push_back(temp);
			}
		}
	}
	return NULL;
}
Plan* DataSource::getFromDailyArray(int planId)
{
	for each (Plan * var in planDailyArray)
	{
		if (var->getId() == planId)
		{
			return var;
		}
	}
	return NULL;
}

void DataSource::finishPlan(int planId)
{
	Plan* plan = getFromDailyArray(planId);
	plan->setFinish(true);
	emit ChangePlan(plan);
}
void DataSource::AddDailyArray(Plan* plan)
{
	this->planDailyArray.push_back(plan);
	emit AddPlan(plan);
}
//所有的任务添加会经由这两个函数处理
void DataSource::AddTodoArray(Task* task, TaskPlace place, Task* father)
{
	if (place == Father)
	{
		taskTodoArray.push_back(task);
		//关联任务信号
		connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
		emit AddTask(TodoArray, task);
	}
	else if (place == Son)
	{
		father->getprocedureTask()->push_back(task);
		//关联任务信号
		connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
		emit AddTask(TodoArray, task, Son, father);
	}
	
}
void DataSource::AddFinishArray(Task* task, TaskPlace place, Task* father)
{
	if (place == Father)
	{
		taskFinishArray.push_back(task);
		//关联任务信号
		connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
		emit AddTask(FinishArray, task);
	}
	else if (place == Son)
	{
		father->getprocedureTask()->push_back(task);
		//关联任务信号
		connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(getTaskEmitChange(TaskArray, Task*)));
		emit AddTask(FinishArray, task, Son, father);
	}
}

void DataSource::deletePlan(int planId)
{
	Plan* plan = getFromDailyArray(planId);
	this->planDailyArray.remove(plan);
	emit DeletePlan(plan);
	delete plan;
}
//要删除的task，它的父亲
void DataSource::deleteTask(Task* task, Task* father)
{
	//如果没有儿子节点，则根据自己的情况，移除数组，释放内存
	if (task->getprocedureTask()->isEmpty())
	{
		if (task->getFatherTask() == NULL)
		{
			taskTodoArray.remove(task);
			delete task;
		}
		else
		{
			father->getprocedureTask()->removeAll(task);
			delete task;
		}
	}
	else
	{
		QListIterator<Task*> itor(*task->getprocedureTask());
		for (itor.toFront(); itor.hasNext();)
		{
			deleteTask(itor.next(), task);
		}
		//处理完子节点后，处理自己
		if (task->getFatherTask() == NULL)
		{
			taskTodoArray.remove(task);
			delete task;
		}
		else
		{
			father->getprocedureTask()->removeAll(task);
			delete task;
		}
	}
}
void DataSource::deleteFromTodoArray(int taskId)
{
	Task* task = getFromTodoArray(taskId);
	//没找到
	if (task == NULL)
	{
		return;
	}
	//先把信号发出去,修改页面
	emit DeleteTask(TodoArray, task);
	deleteTask(task, task->getFatherTask());
}
void DataSource::deleteFromFinishArray(int taskId)
{
	Task* task = getFromFinishArray(taskId);
	//没找到
	if (task == NULL)
	{
		return;
	}
	//先把信号发出去,修改页面
	emit DeleteTask(FinishArray, task);
	deleteTask(task, task->getFatherTask());
}
Task* DataSource::Translate(TaskArray a, TaskArray b, Task* task)
{
	if (a == TodoArray)
	{
		if (b == FinishArray)
		{
			//从Todo到Finish一定是任务
			taskTodoArray.remove(task);
			emit DeleteTask(TodoArray, task);
			taskFinishArray.push_back(task);
			emit AddTask(FinishArray, task);
			return task;
		}
	}
	else if (a == FinishArray)
	{
		if (b == TodoArray)
		{
			//从Finish到Todo也一定是任务
			taskFinishArray.remove(task);
			emit DeleteTask(FinishArray, task);
			taskTodoArray.push_back(task);
			emit AddTask(TodoArray, task);
			return task;
		}
	}
	else
	{
		return NULL;
	}
	return NULL;
}

Task* DataSource::Translate(TaskArray a, TaskArray b, int taskId)
{
	if (a == TodoArray)
	{
		if (b == FinishArray)
		{
			for each (Task * var in taskTodoArray)
			{
				if (var->getId() == taskId)
				{
					taskTodoArray.remove(var);
					emit DeleteTask(TodoArray, var);
					taskFinishArray.push_back(var);
					emit AddTask(FinishArray, var);;
					return var;
				}
			}
		}
	}
	else if (a == FinishArray)
	{
		if (b == TodoArray)
		{
			for each (Task * var in taskFinishArray)
			{
				if (var->getId() == taskId)
				{
					taskFinishArray.remove(var);
					emit DeleteTask(FinishArray, var);
					taskTodoArray.push_back(var);
					emit AddTask(TodoArray, var);;
					return var;
				}
			}
		}
	}
	return NULL;
}
//datasource获取到task的信号后把信号原封不动发出
void DataSource::getTaskEmitChange(TaskArray a, Task* t)
{
	emit ChangeTask(a, t);
}