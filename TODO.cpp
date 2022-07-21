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

	//设置时间范围
	ui.DteTaskdeadline->setMinimumDate(QDate::currentDate().addDays(-365));
	ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
	//默认展示当前时间
	ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());
	ui.DteNow->setDateTime(QDateTime::currentDateTime());
	//修改表头宽度
	ui.TrewTodoTasklist->setColumnWidth(0, 40);
	ui.TrewTodoTasklist->setColumnWidth(1, 60);
	ui.TrewTodoTasklist->setColumnWidth(2, 120);
	ui.TrewFinishTasklist->setColumnWidth(0, 40);
	ui.TrewFinishTasklist->setColumnWidth(1, 60);
	ui.TrewFinishTasklist->setColumnWidth(2, 120);
	//tasklistmodel = ui.TrewTodoTasklist->model();

	//设置字体数据
	// 
	//设置右键菜单
	taskDelete = new QAction(QString::fromLocal8Bit("删除任务"));
	taskUpdate = new QAction(QString::fromLocal8Bit("更新任务"));
	taskNewChild = new QAction(QString::fromLocal8Bit("新建步骤"));

	//插入数据方便测试

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
	//双击事件
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));
	//定时器
	connect(qtimer, SIGNAL(timeout()), this, SLOT(TimeOut()));
	//界面更新
	connect(&datasource, SIGNAL(AddTask(TaskArray, Task*, TaskPlace, Task*)), this, SLOT(AddTaskItem(TaskArray, Task*, TaskPlace, Task*)));
	connect(&datasource, SIGNAL(DeleteTask(TaskArray, Task*)), this, SLOT(DeleteTaskItem(TaskArray, Task*)));
	connect(&datasource, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	//右键菜单
	connect(ui.TrewTodoTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInTodo(QTreeWidgetItem*)));
	connect(ui.TrewFinishTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInFinish(QTreeWidgetItem*)));
	connect(taskDelete, SIGNAL(triggered()), this, SLOT(DeleteTask()));
	connect(taskUpdate, SIGNAL(triggered()), this, SLOT(UpdateTask()));
	connect(taskNewChild, SIGNAL(triggered()), this, SLOT(NewChildTask()));

}

int TODO::DeleteTask()
{
	//尝试在两个地方删除数据
	datasource.deleteFromTodoArray(taskDelete->data().toInt());
	datasource.deleteFromFinishArray(taskDelete->data().toInt());
	return 0;
}
int TODO::UpdateTask()
{
	//获取当前选中item的id
	int taskId = taskUpdate->data().toUInt();
	//只有在Todo中的任务可以修改
	Task* var = datasource.getFromTodoArray(taskId);
	//将选中的对象传给对话框
	ChangeTaskInfomation changetaskinfomation(var, this);
	//获取对话框返回值
	int ret = changetaskinfomation.exec();
	if (ret == QDialog::Accepted)
	{
		//复制从对话框中修改的数据项
		var->pasteFromNewtask(changetaskinfomation.Ttask);
	}
	return 0;
}
int TODO::NewChildTask()
{
	//获取当前选中item的id
	int taskId = taskNewChild->data().toUInt();
	Task* var = datasource.getFromTodoArray(taskId);
	//将选中的对象传给对话框
	AddTaskChild addtaskchild(var, this);
	//获取对话框返回值
	int ret = addtaskchild.exec();
	if (ret == QDialog::Accepted)
	{
		//关联task的更新
		connect(addtaskchild.Newprocedure, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
		datasource.AddTodoArray(addtaskchild.Newprocedure, Son, var);
	}

	return 0;
}
int TODO::RightClickedInTodo(QTreeWidgetItem* item)
{
	//判断是否为右键
	if (qApp->mouseButtons() == Qt::RightButton)  
	{
		rightMenu = new QMenu(ui.TrewTodoTasklist);
		//把数据传递给选项
		taskDelete->setData(item->data(0, Qt::UserRole));
		taskUpdate->setData(item->data(0, Qt::UserRole));
		taskNewChild->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(taskDelete);
		rightMenu->addAction(taskUpdate);
		rightMenu->addAction(taskNewChild);
		rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
	}
	return 0;
}
int TODO::RightClickedInFinish(QTreeWidgetItem* item)
{
	//判断是否为右键
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		rightMenu = new QMenu(ui.TrewFinishTasklist);
		taskDelete->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(taskDelete);
		rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
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
			//Son在Todo中，那father也在Todo中
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
		var->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM月dd日 hh:mm:ss")));
		if (task->getRepeat())
		{
			var->setText(3, QString::fromLocal8Bit("是"));
		}
		else
		{
			var->setText(3, QString::fromLocal8Bit("否"));
		}
		return 0;
	}
	else if (taskarray == FinishArray)
	{
		QTreeWidgetItem* var = getTaskItemById(task->getId(), ui.TrewFinishTasklist);
		var->setText(1, task->getName());
		var->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM月dd日 hh:mm:ss")));
		if (task->getRepeat())
		{
			var->setText(3, QString::fromLocal8Bit("是"));
		}
		else
		{
			var->setText(3, QString::fromLocal8Bit("否"));
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
	//获取当前时间
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
		//处理下一个根节点
		father = ui.TrewTodoTasklist->topLevelItem(++i);
	}
	return 0;
}
int TODO::DoubleClickInwidget(QTreeWidgetItem* item)
{
	//获取当前选中item的id
	int taskId = item->data(0, Qt::UserRole).toUInt();
	Task* var = datasource.getFromTodoArray(taskId);
	//将选中的对象传给对话框
	ChangeTaskInfomation changetaskinfomation(var, this);
	//获取对话框返回值
	int ret = changetaskinfomation.exec();
	if (ret == QDialog::Accepted)
	{
		//复制从对话框中修改的数据项
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
	//新建item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//设置显示数据和task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM月dd日 hh:mm:ss")));
	if (task->getRepeat())
	{
		item->setText(3, QString::fromLocal8Bit("是"));
	}
	else
	{
		item->setText(3, QString::fromLocal8Bit("否"));
	}
	return item;
}
QCheckBox* TODO::newQCheckbox(QTreeWidgetItem* item, Task* task, TaskState state)
{
	//构造复选框
	QCheckBox* qcb = new QCheckBox();
	//先设置状态再关联信号和槽
	if (state == Finish)
	{
		//任务完成颜色变化
		item->setTextColor(1, Qt::green);
		item->setTextColor(2, Qt::green);
		qcb->setCheckState(Qt::Checked);
	}
	//判断要插入的任务是否过期
	else if (state == Todo)
	{
		if (task->getDeadline() < QDateTime::currentDateTime())
		{
			item->setTextColor(1, Qt::red);
			item->setTextColor(2, Qt::red);
			item->setTextColor(3, Qt::red);
		}
	}
	//关联复选框信号和槽
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	return qcb;
}
QTreeWidgetItem* TODO::getTaskItemById(int taskId, QTreeWidget* widget)
{
	//获取根节点个数
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
		//新建item
		QTreeWidgetItem* item = newItem(task->getId(), task);
		//添加item
		widget->addTopLevelItem(item);
		//生成复选框
		QCheckBox* qcb = newQCheckbox(item, task, state);
		//添加复选框
		widget->setItemWidget(item, 0, qcb);
	}
	else if (place == Son)
	{
		//新建item
		QTreeWidgetItem* son = newItem(task->getId(), task);
		//添加item
		father->addChild(son);
		//构造复选框
		QCheckBox* qcb = newQCheckbox(son, task, Todo);
		//添加复选框
		widget->setItemWidget(son, 0, qcb);
	}
}
int TODO::Changetaskstate(int id)
{
	//完成任务只能在Todo中
	if (id == 2 && !ui.TrewTodoTasklist->hasFocus())
	{
		//qDebug() << QString::fromLocal8Bit("选中");
		//获取选中item
		QTreeWidgetItem* t = ui.TrewTodoTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//任务完成一次
		Task* var = datasource.TaskFinishOnce(taskId);
		if (var->getRepeat())
		{
			QCheckBox* qcb = (QCheckBox*)ui.TrewTodoTasklist->itemWidget(t, 0);
			qcb->setCheckState(Qt::Unchecked);
		}
	}
	//重做任务只能在Finish中
	else if (id == 0 && !ui.TrewFinishTasklist->hasFocus())
	{
		//qDebug() << QString::fromLocal8Bit("未选中");
		//取消选中
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//修改数组数据
		datasource.Translate(FinishArray, TodoArray, taskId);
	}
	return 0;
}

int TODO::OnBtnAddtask()
{
	if (ui.LinTaskname->text().isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("任务名不能为空"));
		return 0;
	}
	//构造task对象
	Task* task = new Task(ui.LinTaskname->text(), ui.DteTaskdeadline->dateTime());
	//加入数据源
	datasource.AddTodoArray(task);
	//关联信号
	connect(task, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	//文本框置空
	ui.LinTaskname->setText("");
	//QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("插入成功"));
	return 0;
}
int TODO::OnBtnDeltask()
{
	//只能删除完成任务
	QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
	//如果没有选中元素
	if (t == NULL)
	{
		return 0;
	}
	//获取选中的taskId
	int taskId = t->data(0, Qt::UserRole).toUInt();
	//删除task
	datasource.deleteFromFinishArray(taskId);
	return 0;
}
int TODO::OnBtnSorttask()
{
	//按照日期降序或升序排列
	if (sortItem)
	{
		ui.TrewTodoTasklist->sortItems(2, Qt::AscendingOrder);
		ui.LabSorttips->setText(QString::fromLocal8Bit("从近到远"));
		sortItem = !sortItem;
	}
	else
	{
		ui.TrewTodoTasklist->sortItems(2, Qt::DescendingOrder);
		ui.LabSorttips->setText(QString::fromLocal8Bit("从远到近"));
		sortItem = !sortItem;

	}

	return 0;
}