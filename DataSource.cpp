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
	//���������ɣ����������ӽڵ�ȫ�����
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
	//����Ǹ��ڵ�����Ҫ�ƶ���finish��
	if (ret->getFatherTask() == NULL)
	{
		if (ret->getRepeat() == false)
		{
			//���ڵ����Ҫת�Ƶ�����б�
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
		//��TodoArray��������һ���ǲ���,��ʱ����Ҫת������
		var = getFromTodoArray(taskId);
		//�������貽��������Ӳ���ȫ������
		var->redoTask(TodoArray);
		//�����������ĸ��ڵ�Ҳ������
		
	}
	else if (array == FinishArray)
	{
		//��FinishArray�������������ǲ�������������������������Ҫ�Ѹ���������
		var = getFromFinishArray(taskId);
		var->redoTask(FinishArray);
		if (var->getFatherTask() == NULL)//����Լ�������Ǹ��ڵ㣬ֱ��ת���Լ��ͺ���
		{
			Translate(FinishArray, TodoArray, var);
		}
		else
		{
			var = GetTaskTopFather(var);//��ȡ������߸��ڵ�
			Translate(FinishArray, TodoArray, var);//ת��
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
Task* DataSource::Translate(TaskArray a, TaskArray b, Task* task)
{
	if (a == TodoArray)
	{
		if (b == FinishArray)
		{
			//��Todo��Finishһ��������
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
			//��Finish��TodoҲһ��������
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
