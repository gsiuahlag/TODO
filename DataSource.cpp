#include "DataSource.h"
DataSource::DataSource()
{}
DataSource::~DataSource()
{}
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
QList<Task*>* DataSource::GetTaskAllChild(Task* task)
{
	return NULL;
}
QList<Task*>* DataSource::GetTaskAllChild(int taskId)
{
	return NULL;
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
	/*//task是根节点
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
