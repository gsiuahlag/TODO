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

void DataSource::AddTodoArray(Task* task, TaskPlace place, Task* father)
{
	if (place == Father)
	{
		taskTodoArray.push_back(task);
		emit AddTask(TodoArray, task);
	}
	else if (place == Son)
	{
		father->getprocedureTask()->push_back(task);
		emit AddTask(TodoArray, task, Son, father);
	}
	
}
void DataSource::AddFinishArray(Task* task, TaskPlace place, Task* father)
{
	if (place == Father)
	{
		taskFinishArray.push_back(task);
		emit AddTask(TodoArray, task);
	}
	else if (place == Son)
	{
		father->getprocedureTask()->push_back(task);
		emit AddTask(TodoArray, task, Son, father);
	}
}
//Ҫɾ����task�����ĸ���
void DataSource::deleteTask(Task* task, Task* father)
{
	//���û�ж��ӽڵ㣬������Լ���������Ƴ����飬�ͷ��ڴ�
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
		//�������ӽڵ�󣬴����Լ�
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
	//û�ҵ�
	if (task == NULL)
	{
		return;
	}
	//�Ȱ��źŷ���ȥ,�޸�ҳ��
	emit DeleteTask(TodoArray, task);
	deleteTask(task, task->getFatherTask());
	
}
void DataSource::deleteFromFinishArray(int taskId)
{
	Task* task = getFromFinishArray(taskId);
	//û�ҵ�
	if (task == NULL)
	{
		return;
	}
	//�Ȱ��źŷ���ȥ,�޸�ҳ��
	emit DeleteTask(FinishArray, task);
	deleteTask(task, task->getFatherTask());
	/*//task�Ǹ��ڵ�
	if (task->getFatherTask() == NULL)
	{
		taskTodoArray.remove(task);
		delete task;
	}
	else
	{
		task->getFatherTask()->getprocedureTask()->removeAll(task);
		delete task;
	}*/
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
