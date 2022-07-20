#include "DataSource.h"
DataSource::DataSource()
{}
DataSource::~DataSource()
{}

Task* DataSource::TaskFinishOnce(int taskId)
{
	Task* var = NULL;
	for each (var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
			if (var->getRepeat() == false)
			{
				Translate(TodoArray, FinishArray, taskId);
			}
			else
			{
				int length = var->getRepeatTime().Length;
				switch (var->getRepeatTime().Unit)
				{
				case Year:
					var->setDeadline(var->getDeadline().addYears(length));
					break;
				case Month:
					var->setDeadline(var->getDeadline().addMonths(length));
					break;
				case Week:
					var->setDeadline(var->getDeadline().addDays(length));
					break;
				case Day:
					var->setDeadline(var->getDeadline().addSecs(length * 3600));
					break;
				case Hour:
					var->setDeadline(var->getDeadline().addSecs(length));
					break;
				default:
					break;
				}
				emit ChangeTask(TodoArray, var);
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
void DataSource::deleteFromTodoArray(int taskId)
{
	for each (Task * var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
			taskTodoArray.remove(var);
			emit DeleteTask(TodoArray, var);
			delete var;
			break;
		}
	}
}
void DataSource::deleteFromFinishArray(int taskId)
{
	for each (Task * var in taskFinishArray)
	{
		if (var->getId() == taskId)
		{
			taskFinishArray.remove(var);
			emit DeleteTask(FinishArray, var);
			delete var;
			break;
		}
	}
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
