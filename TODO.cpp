#include "TODO.h"
#include "Task.h"
#include <QMessageBox>
#include <QDebug>
#include <QQueue>
#include <QCheckBox>
#include <QTimer>
#include "ChangeTaskInfomation.h"
#include "AddTaskChild.h"
TODO::TODO(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//����ʱ�䷶Χ
	ui.DteTaskdeadline->setMinimumDate(QDate::currentDate().addDays(-365));
	ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
	//Ĭ��չʾ��ǰʱ��
	ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());
	ui.DteNow->setDateTime(QDateTime::currentDateTime());
	//�޸ı�ͷ���
	ui.TrewTodoTasklist->setColumnWidth(0, 40);
	ui.TrewTodoTasklist->setColumnWidth(1, 60);
	ui.TrewTodoTasklist->setColumnWidth(2, 120);
	ui.TrewFinishTasklist->setColumnWidth(0, 40);
	ui.TrewFinishTasklist->setColumnWidth(1, 60);
	ui.TrewFinishTasklist->setColumnWidth(2, 120);
	//tasklistmodel = ui.TrewTodoTasklist->model();

	//������������
	// 
	//�����Ҽ��˵�
	taskDelete = new QAction(QString::fromLocal8Bit("ɾ������"));
	taskUpdate = new QAction(QString::fromLocal8Bit("��������"));
	taskNewChild = new QAction(QString::fromLocal8Bit("�½�����"));

	//�������ݷ������

	Task* task = new Task("test1", QDateTime::currentDateTime(), NULL, true, 10, Hour);
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test2", QDateTime::currentDateTime().addSecs(10), NULL, false);
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test3", QDateTime::currentDateTime().addSecs(15), NULL, true, 5, Day);
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test4", QDateTime::currentDateTime().addSecs(20), NULL, true, 4,Week);
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	qtimer = new QTimer(this);
	qtimer->start(1000);

	connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnDeltask, SIGNAL(clicked()), this, SLOT(OnBtnDeltask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));
	//˫���¼�
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));
	//��ʱ��
	connect(qtimer, SIGNAL(timeout()), this, SLOT(TimeOut()));
	//�������
	connect(&datasource, SIGNAL(AddTask(TaskArray, Task*, TaskPlace, Task*)), this, SLOT(AddTaskItem(TaskArray, Task*, TaskPlace, Task*)));
	connect(&datasource, SIGNAL(DeleteTask(TaskArray, Task*)), this, SLOT(DeleteTaskItem(TaskArray, Task*)));
	connect(&datasource, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	//�Ҽ��˵�
	connect(ui.TrewTodoTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInTodo(QTreeWidgetItem*)));
	connect(ui.TrewFinishTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInFinish(QTreeWidgetItem*)));
	connect(taskDelete, SIGNAL(triggered()), this, SLOT(DeleteTask()));
	connect(taskUpdate, SIGNAL(triggered()), this, SLOT(UpdateTask()));
	connect(taskNewChild, SIGNAL(triggered()), this, SLOT(NewChildTask()));

}

int TODO::DeleteTask()
{
	//�����������ط�ɾ������
	datasource.deleteFromTodoArray(taskDelete->data().toInt());
	datasource.deleteFromFinishArray(taskDelete->data().toInt());
	return 0;
}
int TODO::UpdateTask()
{
	//��ȡ��ǰѡ��item��id
	int taskId = taskUpdate->data().toUInt();
	//ֻ����Todo�е���������޸�
	Task* var = datasource.getFromTodoArray(taskId);
	//��ѡ�еĶ��󴫸��Ի���
	ChangeTaskInfomation changetaskinfomation(var, this);
	//��ȡ�Ի��򷵻�ֵ
	int ret = changetaskinfomation.exec();
	if (ret == QDialog::Accepted)
	{
		//���ƴӶԻ������޸ĵ�������
		var->pasteFromNewtask(changetaskinfomation.Ttask);
	}
	return 0;
}
int TODO::NewChildTask()
{
	//��ȡ��ǰѡ��item��id
	int taskId = taskNewChild->data().toUInt();
	Task* var = datasource.getFromTodoArray(taskId);
	//��ѡ�еĶ��󴫸��Ի���
	AddTaskChild addtaskchild(var, this);
	//��ȡ�Ի��򷵻�ֵ
	int ret = addtaskchild.exec();
	if (ret == QDialog::Accepted)
	{
		//����task�ĸ���
		connect(addtaskchild.Newprocedure, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
		datasource.AddTodoArray(addtaskchild.Newprocedure, Son, var);
	}

	return 0;
}
int TODO::RightClickedInTodo(QTreeWidgetItem* item)
{
	//�ж��Ƿ�Ϊ�Ҽ�
	if (qApp->mouseButtons() == Qt::RightButton)  
	{
		rightMenu = new QMenu(ui.TrewTodoTasklist);
		//�����ݴ��ݸ�ѡ��
		taskDelete->setData(item->data(0, Qt::UserRole));
		taskUpdate->setData(item->data(0, Qt::UserRole));
		taskNewChild->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(taskDelete);
		rightMenu->addAction(taskUpdate);
		rightMenu->addAction(taskNewChild);
		rightMenu->exec(QCursor::pos());   //�˵�����λ��Ϊ�����λ��
	}
	return 0;
}
int TODO::RightClickedInFinish(QTreeWidgetItem* item)
{
	//�ж��Ƿ�Ϊ�Ҽ�
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		rightMenu = new QMenu(ui.TrewFinishTasklist);
		taskDelete->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(taskDelete);
		rightMenu->exec(QCursor::pos());   //�˵�����λ��Ϊ�����λ��
	}
	return 0;
}
int TODO::AddTaskItem(TaskArray taskarray, Task* task, TaskPlace place, Task* father)
{
	if (taskarray == TodoArray)
	{
		if (place == Father)
		{
			insertItem(task, ui.TrewTodoTasklist, Todo);
		}
		else if (place == Son)
		{
			//Son��Todo�У���fatherҲ��Todo��
			QTreeWidgetItem* faitem = getTaskItemById(father->getId(), ui.TrewTodoTasklist);
			insertItem(task, ui.TrewTodoTasklist, Todo, Son, faitem);
		}
		
	}
	else if (taskarray == FinishArray)
	{
		insertItem(task, ui.TrewFinishTasklist, Finish);
	}
	return 0;
}

int TODO::DeleteTaskItem(TaskArray taskarray, Task* task)
{
	if (taskarray == TodoArray)
	{
		QTreeWidgetItem* var = getTaskItemById(task->getId(), ui.TrewTodoTasklist);
		delete var;
		return 0;
	}
	else if (taskarray == FinishArray)
	{
		QTreeWidgetItem* var = getTaskItemById(task->getId(), ui.TrewFinishTasklist);
		delete var;
		return 0;
	}
	else
	{
		return 0;
	}
}
int TODO::ChangeTaskItem(TaskArray taskarray, Task* task)
{
	if (taskarray == TodoArray)
	{
		QTreeWidgetItem* var = getTaskItemById(task->getId(),ui.TrewTodoTasklist);
		var->setText(1, task->getName());
		var->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM��dd�� hh:mm:ss")));
		if (task->getRepeat())
		{
			var->setText(3, QString::fromLocal8Bit("��"));
		}
		else
		{
			var->setText(3, QString::fromLocal8Bit("��"));
		}
		return 0;
	}
	else if (taskarray == FinishArray)
	{
		QTreeWidgetItem* var = getTaskItemById(task->getId(), ui.TrewFinishTasklist);
		var->setText(1, task->getName());
		var->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM��dd�� hh:mm:ss")));
		if (task->getRepeat())
		{
			var->setText(3, QString::fromLocal8Bit("��"));
		}
		else
		{
			var->setText(3, QString::fromLocal8Bit("��"));
		}
		return 0;
	}
	else
	{
		return 0;
	}
}
int TODO::TimeOut()
{
	//��ȡ��ǰʱ��
	QDateTime Nowtime = QDateTime::currentDateTime();
	ui.DteNow->setDateTime(Nowtime);
	int i = 0, j = 0;
	QQueue<QTreeWidgetItem*> q;
	QTreeWidgetItem* father = ui.TrewTodoTasklist->topLevelItem(i);
	QTreeWidgetItem* son = NULL;
	QTreeWidgetItem* item = NULL;
	while (father != NULL)
	{
		q.push_back(father);
		while (!q.isEmpty())
		{
			item = q.front();
			q.pop_front();
			if (datasource.getFromTodoArray(item->data(0, Qt::UserRole).toUInt())->getDeadline() < Nowtime)
			{

				item->setTextColor(1, Qt::red);
				item->setTextColor(2, Qt::red);
				item->setTextColor(3, Qt::red);
			}
			else
			{
				item->setTextColor(1, Qt::black);
				item->setTextColor(2, Qt::black);
				item->setTextColor(3, Qt::black);
			}
			son = item->child(j);
			while (son)
			{
				q.push_back(son);
				son = item->child(++j);
			}
		}
		//������һ�����ڵ�
		father = ui.TrewTodoTasklist->topLevelItem(++i);
	}
	return 0;
}
int TODO::DoubleClickInwidget(QTreeWidgetItem* item)
{
	//��ȡ��ǰѡ��item��id
	int taskId = item->data(0, Qt::UserRole).toUInt();
	Task* var = datasource.getFromTodoArray(taskId);
	//��ѡ�еĶ��󴫸��Ի���
	ChangeTaskInfomation changetaskinfomation(var, this);
	//��ȡ�Ի��򷵻�ֵ
	int ret = changetaskinfomation.exec();
	if (ret == QDialog::Accepted)
	{
		//���ƴӶԻ������޸ĵ�������
		var->pasteFromNewtask(changetaskinfomation.Ttask);
	}
	return 0;
}
QTreeWidgetItem* TODO::newItem(int taskId, Task* task)
{
	if (task == NULL)
	{
		task = datasource.getFromArray(taskId);
		if (task == NULL)
		{
			return NULL;
		}
	}
	//�½�item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//������ʾ���ݺ�task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM��dd�� hh:mm:ss")));
	if (task->getRepeat())
	{
		item->setText(3, QString::fromLocal8Bit("��"));
	}
	else
	{
		item->setText(3, QString::fromLocal8Bit("��"));
	}
	return item;
}
QCheckBox* TODO::newQCheckbox(QTreeWidgetItem* item, Task* task, TaskState state)
{
	//���츴ѡ��
	QCheckBox* qcb = new QCheckBox();
	//������״̬�ٹ����źźͲ�
	if (state == Finish)
	{
		//���������ɫ�仯
		item->setTextColor(1, Qt::green);
		item->setTextColor(2, Qt::green);
		qcb->setCheckState(Qt::Checked);
	}
	//�ж�Ҫ����������Ƿ����
	else if (state == Todo)
	{
		if (task->getDeadline() < QDateTime::currentDateTime())
		{
			item->setTextColor(1, Qt::red);
			item->setTextColor(2, Qt::red);
			item->setTextColor(3, Qt::red);
		}
	}
	//������ѡ���źźͲ�
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	return qcb;
}
QTreeWidgetItem* TODO::getTaskItemById(int taskId, QTreeWidget* widget)
{
	//��ȡ���ڵ����
	int size = widget->topLevelItemCount();
	QQueue<QTreeWidgetItem*> q;
	QTreeWidgetItem* father = NULL;
	QTreeWidgetItem* son = NULL;
	for (int i = 0; i < size; i++)
	{
		father = widget->topLevelItem(i);
		q.push_back(father);
		while (!q.isEmpty())
		{
			son = q.front();
			q.pop_front();
			if (son->data(0, Qt::UserRole).toInt() == taskId)
			{
				return son;
			}
			for (int j = 0; j < son->childCount(); j++)
			{
				if (son->child(j)->data(0, Qt::UserRole).toInt() == taskId)
				{
					return son->child(j);
				}
				q.push_back(son->child(j));
			}
		}
	}
	return NULL;
}
void TODO::insertItem(Task* task, QTreeWidget* widget, TaskState state, TaskPlace place, QTreeWidgetItem* father)
{
	if (place == Father)
	{	
		//�½�item
		QTreeWidgetItem* item = newItem(task->getId(), task);
		//���item
		widget->addTopLevelItem(item);
		//���ɸ�ѡ��
		QCheckBox* qcb = newQCheckbox(item, task, state);
		//��Ӹ�ѡ��
		widget->setItemWidget(item, 0, qcb);
	}
	else if (place == Son)
	{
		//�½�item
		QTreeWidgetItem* son = newItem(task->getId(), task);
		//���item
		father->addChild(son);
		//���츴ѡ��
		QCheckBox* qcb = newQCheckbox(son, task, Todo);
		//��Ӹ�ѡ��
		widget->setItemWidget(son, 0, qcb);
	}
}
int TODO::Changetaskstate(int id)
{
	//�������ֻ����Todo��
	if (id == 2 && !ui.TrewTodoTasklist->hasFocus())
	{
		//qDebug() << QString::fromLocal8Bit("ѡ��");
		//��ȡѡ��item
		QTreeWidgetItem* t = ui.TrewTodoTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//�������һ��
		Task* var = datasource.TaskFinishOnce(taskId);
		if (var->getRepeat())
		{
			QCheckBox* qcb = (QCheckBox*)ui.TrewTodoTasklist->itemWidget(t, 0);
			qcb->setCheckState(Qt::Unchecked);
		}
	}
	//��������ֻ����Finish��
	else if (id == 0 && !ui.TrewFinishTasklist->hasFocus())
	{
		//qDebug() << QString::fromLocal8Bit("δѡ��");
		//ȡ��ѡ��
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//�޸���������
		datasource.Translate(FinishArray, TodoArray, taskId);
	}
	return 0;
}

int TODO::OnBtnAddtask()
{
	if (ui.LinTaskname->text().isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������Ϊ��"));
		return 0;
	}
	//����task����
	Task* task = new Task(ui.LinTaskname->text(), ui.DteTaskdeadline->dateTime());
	//��������Դ
	datasource.AddTodoArray(task);
	//�����ź�
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	//�ı����ÿ�
	ui.LinTaskname->setText("");
	//QMessageBox::information(this, QString::fromLocal8Bit("�ɹ�"), QString::fromLocal8Bit("����ɹ�"));
	return 0;
}
int TODO::OnBtnDeltask()
{
	//ֻ��ɾ���������
	QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
	//���û��ѡ��Ԫ��
	if (t == NULL)
	{
		return 0;
	}
	//��ȡѡ�е�taskId
	int taskId = t->data(0, Qt::UserRole).toUInt();
	//ɾ��task
	datasource.deleteFromFinishArray(taskId);
	return 0;
}
int TODO::OnBtnSorttask()
{
	//�������ڽ������������
	if (sortItem)
	{
		ui.TrewTodoTasklist->sortItems(2, Qt::AscendingOrder);
		ui.LabSorttips->setText(QString::fromLocal8Bit("�ӽ���Զ"));
		sortItem = !sortItem;
	}
	else
	{
		ui.TrewTodoTasklist->sortItems(2, Qt::DescendingOrder);
		ui.LabSorttips->setText(QString::fromLocal8Bit("��Զ����"));
		sortItem = !sortItem;

	}

	return 0;
}