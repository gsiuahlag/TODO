#include "DataSource.h"
DataSource::DataSource()
{}
DataSource::~DataSource()
{}

Task* DataSource::TaskFinishOnce(int taskId, QTreeWidgetItem* t)
{
	Task* var = NULL;
	for each (var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
			//剩余循环次数-1
			var->setLastRepeatCount(var->getLastRepeatCount() - 1);
			emit ChangeTask(TodoArray, var,t);
			//完成之后循环次数为0，则事件完成
			if (var->getLastRepeatCount() <= 0)
			{
				this->Translate(TodoArray, FinishArray, var->getId(), t);
			}
			return var;
		}
	}
}
Task* DataSource::getFromTodoArray(int taskId)
{
	for each (Task * var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
			return var;
		}
	}
	return NULL;
}
Task* DataSource::getFromFinishArray(int taskId)
{
	for each (Task * var in taskFinishArray)
	{
		if (var->getId() == taskId)
		{
			return var;
		}
	}
	return NULL;
}
void DataSource::AddTodoArray(Task* task)
{
	taskTodoArray.push_back(task);
	emit AddTask(TodoArray,task);
}
void DataSource::AddFinishArray(Task* task)
{
	taskFinishArray.push_back(task);
	emit AddTask(FinishArray, task);
}
void DataSource::deleteFromTodoArray(int taskId, QTreeWidgetItem* t)
{
	for each (Task * var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
			taskTodoArray.remove(var);
			emit DeleteTask(TodoArray, var, t);
			delete var;
			break;
		}
	}
}
void DataSource::deleteFromFinishArray(int taskId, QTreeWidgetItem* t)
{
	for each (Task * var in taskFinishArray)
	{
		if (var->getId() == taskId)
		{
			taskFinishArray.remove(var);
			emit DeleteTask(FinishArray, var, t);
			delete var;
			break;
		}
	}
}
Task* DataSource::Translate(TaskArray a, TaskArray b, int taskId,QTreeWidgetItem* t)
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
					emit DeleteTask(TodoArray, var,t);
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
					emit DeleteTask(FinishArray, var,t);
					taskTodoArray.push_back(var);
					emit AddTask(TodoArray, var);;
					return var;
				}
			}
		}
	}
	return NULL;
}