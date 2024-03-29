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
	
	//默认展示当前时间,最小时间不能超过当天，最大不能超过一年
	ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());
	ui.DteTaskdeadline->setMinimumDateTime(QDateTime::currentDateTime());
	ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
	ui.DteNow->setDateTime(QDateTime::currentDateTime());
	ui.DteDaily->setDate(QDate::currentDate());
	//修改表头宽度
	ui.TrewTodoTasklist->setColumnWidth(0, 40);
	ui.TrewTodoTasklist->setColumnWidth(1, 60);
	ui.TrewTodoTasklist->setColumnWidth(2, 120);
	ui.TrewFinishTasklist->setColumnWidth(0, 40);
	ui.TrewFinishTasklist->setColumnWidth(1, 60);
	ui.TrewFinishTasklist->setColumnWidth(2, 120);

	//设置字体数据
	// 
	//设置右键菜单
	taskDelete = new QAction(QString::fromLocal8Bit("删除任务"));
	taskUpdate = new QAction(QString::fromLocal8Bit("更新任务"));
	taskNewChild = new QAction(QString::fromLocal8Bit("新建步骤"));
	planDelete = new QAction(QString::fromLocal8Bit("删除计划"));

	//初始化menu
	rightMenu = new QMenu();
	qtimer = new QTimer(this);
	qtimer->start(1000);
	//每日计划
	connect(ui.DteDaily, SIGNAL(dateChanged(QDate)), this, SLOT(DailyChange(QDate)));
	//按钮
	connect(ui.BtnAddplan, SIGNAL(clicked()), this, SLOT(OnBtnAddplan()));
	connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));
	//双击事件
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));
	//定时器
	connect(qtimer, SIGNAL(timeout()), this, SLOT(TimeOut()));
	//界面更新
	connect(&datasource, SIGNAL(AddTask(TaskArray, Task*, TaskPlace, Task*)), this, SLOT(AddTaskItem(TaskArray, Task*, TaskPlace, Task*)));
	connect(&datasource, SIGNAL(DeleteTask(TaskArray, Task*)), this, SLOT(DeleteTaskItem(TaskArray, Task*)));
	connect(&datasource, SIGNAL(ChangeTask(TaskArray, Task*)), this, SLOT(ChangeTaskItem(TaskArray, Task*)));
	connect(&datasource, SIGNAL(AddPlan(Plan*)), this, SLOT(AddPlanItem(Plan*)));
	connect(&datasource, SIGNAL(DeletePlan(Plan*, bool)), this, SLOT(DeletePlanItem(Plan*, bool)));
	connect(&datasource, SIGNAL(ChangePlan(Plan*)), this, SLOT(ChangePlanItem(Plan*)));

	//右键菜单
	connect(ui.MyDailyPlanlist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInDaily(QTreeWidgetItem*)));
	connect(ui.TrewTodoTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInTodo(QTreeWidgetItem*)));
	connect(ui.TrewFinishTasklist, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(RightClickedInFinish(QTreeWidgetItem*)));
	connect(taskDelete, SIGNAL(triggered()), this, SLOT(DeleteTask()));
	connect(taskUpdate, SIGNAL(triggered()), this, SLOT(UpdateTask()));
	connect(taskNewChild, SIGNAL(triggered()), this, SLOT(NewChildTask()));
	connect(planDelete, SIGNAL(triggered()), this, SLOT(DeletePlan()));

	//从服务器获取Task数据
	datasource.getTaskData();
	//从服务器获取Plan数据
	datasource.getDailyDataFromServer(ui.DteDaily->date());
}
//每日计划
int TODO::DailyChange(QDate date)
{
	datasource.setdailyDate(date);
	return 0; 
}

int TODO::DeletePlan()
{
	datasource.deletePlan(planDelete->data().toInt());
	return 0;
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
		//加入产生的Task
		datasource.AddTodoArray(addtaskchild.Newprocedure, Son, var);
	}

	return 0;
}
int TODO::RightClickedInTodo(QTreeWidgetItem* item)
{
	//判断是否为右键
	if (qApp->mouseButtons() == Qt::RightButton)  
	{
		//先把之前的释放
		delete rightMenu;
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
		//先把之前的释放
		delete rightMenu;
		rightMenu = new QMenu(ui.TrewFinishTasklist);
		taskDelete->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(taskDelete);
		rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
	}
	return 0;
}
int TODO::RightClickedInDaily(QTreeWidgetItem* item)
{
	//判断是否为右键
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		//先把之前的释放
		delete rightMenu;
		rightMenu = new QMenu(ui.MyDailyPlanlist);
		planDelete->setData(item->data(0, Qt::UserRole));
		rightMenu->addAction(planDelete);
		rightMenu->exec(QCursor::pos());   //菜单弹出位置为鼠标点击位置
	}
	return 0;
}
//界面更新
int TODO::AddTaskItem(TaskArray taskarray, Task* task, TaskPlace place, Task* father)
{
	if (taskarray == TodoArray)
	{
		if (place == Father)
		{
			//插入一条Father记录，如果有儿子节点要把儿子节点插入
			//QTreeWidgetItem* fa = insertItem(task, ui.TrewTodoTasklist, Todo);
			//QTreeWidgetItem* fa = NULL;
			//一个任务的规模应该不会太大
			//遍历插入儿子,效率？遍历儿子是O（n）,插入后查找父亲也是O（n），整个复杂度为O（n^2）
			QStack<Task*> stack;
			Task* p;
			stack.push(task);
			while (!stack.isEmpty())
			{
				p = stack.top();
				stack.pop();
				if (p->getFatherTask() == NULL)
				{
					insertItem(p, ui.TrewTodoTasklist, Todo);
				}
				else
				{
					AddTaskItem(TodoArray, p, Son, p->getFatherTask());
				}
				QListIterator<Task*> itor(*p->getprocedureTask());
				for (itor.toBack(); itor.hasPrevious();)
				{
					stack.push(itor.previous());
				}
			}
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
		if (place == Father)
		{
			//遍历插入儿子,效率？遍历儿子是O（n）,插入后查找父亲也是O（n），整个复杂度为O（n^2）
			QStack<Task*> stack;
			Task* p;
			stack.push(task);
			while (!stack.isEmpty())
			{
				p = stack.top();
				stack.pop();
				if (p->getFatherTask() == NULL)
				{
					insertItem(p, ui.TrewFinishTasklist, Finish);
				}
				else
				{
					AddTaskItem(FinishArray, p, Son, p->getFatherTask());
				}
				QListIterator<Task*> itor(*p->getprocedureTask());
				for (itor.toBack(); itor.hasPrevious();)
				{
					stack.push(itor.previous());
				}
			}
		}
		else if (place == Son)
		{
			//Son在Finish中，那father也在Finish中
			QTreeWidgetItem* faitem = getTaskItemById(father->getId(), ui.TrewFinishTasklist);
			insertItem(task, ui.TrewFinishTasklist, Finish, Son, faitem);
		}
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
	QTreeWidgetItem* var;
	if (taskarray == TodoArray)
	{
		var = getTaskItemById(task->getId(),ui.TrewTodoTasklist);
		//如果任务/步骤完成
		if (task->getIsFinish())
		{
			setQcheckBoxState((QCheckBox*)ui.TrewTodoTasklist->itemWidget(var, 0), Finish);
			setItemColor(Qt::green, var);
		}
		else
		{
			setQcheckBoxState((QCheckBox*)ui.TrewTodoTasklist->itemWidget(var, 0), Todo);
			setItemColor(Qt::black, var);
		}
	}
	else if (taskarray == FinishArray)
	{
		var = getTaskItemById(task->getId(), ui.TrewFinishTasklist);
		//如果任务/步骤完成
		if (task->getIsFinish())
		{
			setQcheckBoxState((QCheckBox*)ui.TrewFinishTasklist->itemWidget(var, 0), Finish);
			setItemColor(Qt::green, var);
		}
		else
		{
			setQcheckBoxState((QCheckBox*)ui.TrewFinishTasklist->itemWidget(var, 0), Todo);
			setItemColor(Qt::black, var);
		}
	}
	else
	{
		return 0;
	}
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
int TODO::AddPlanItem(Plan* plan)
{
	//新建item并设置数据
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, plan->getId());
	item->setText(1, plan->getName());
	item->setText(2, plan->getTodotime().toString(QString::fromLocal8Bit("hh:mm")));
	//添加item
	ui.MyDailyPlanlist->addTopLevelItem(item);
	//生成复选框
	QCheckBox* qcb = new QCheckBox();
	connect(qcb, SIGNAL(clicked(bool)), this, SLOT(Changetaskstate(bool)));
	//添加复选框
	ui.MyDailyPlanlist->setItemWidget(item, 0, qcb);
	return 0;
}
int TODO::DeletePlanItem(Plan* plan, bool all)
{
	if (all)
	{
		QTreeWidgetItem* var = NULL;
		int count = ui.MyDailyPlanlist->topLevelItemCount();
		for (int i = 0; i < count; i++)
		{
			//删除一个以后索引会变
			var = ui.MyDailyPlanlist->topLevelItem(0);
			delete var;
		}
	}
	else
	{
		QTreeWidgetItem* var = getPlanItemById(plan->getId());
		delete var;
	}
	return 0;
}
int TODO::ChangePlanItem(Plan* plan)
{
	QTreeWidgetItem* var = getPlanItemById(plan->getId());
	if (plan->isFinish())
	{
		setItemColor(Qt::green, var);
	}
	else
	{
		setItemColor(Qt::black, var);
	}
	return 0;
}
	
//定时器
int TODO::TimeOut()
{
	//获取当前时间,检测任务状态和超时
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
			Task* task = datasource.getFromTodoArray(item->data(0, Qt::UserRole).toUInt());
			//在Todo中处理完成事件
			if (task->getIsFinish())
			{
				setQcheckBoxState((QCheckBox*)ui.TrewTodoTasklist->itemWidget(item, 0), Finish);
				setItemColor(Qt::green, item);
			}
			else if (task->getDeadline() < Nowtime)
			{
				setQcheckBoxState((QCheckBox*)ui.TrewTodoTasklist->itemWidget(item, 0), Todo);
				setItemColor(Qt::red, item);
			}
			else
			{
				setQcheckBoxState((QCheckBox*)ui.TrewTodoTasklist->itemWidget(item, 0), Todo);
				setItemColor(Qt::black, item);
			}
			son = item->child(j);
			while (son)
			{
				q.push_back(son);
				son = item->child(++j);
			}
			j = 0;
		}
		//处理下一个根节点
		father = ui.TrewTodoTasklist->topLevelItem(++i);
	}
	return 0;
}
//双击
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
		item->setTextColor(3, Qt::green);
		qcb->setCheckState(Qt::Checked);
	}
	else
	{
		//任务未完成颜色变化
		item->setTextColor(1, Qt::black);
		item->setTextColor(2, Qt::black);
		item->setTextColor(2, Qt::black);
		qcb->setCheckState(Qt::Unchecked);
	}
	//判断要插入的任务是否过期
	if (state == Todo)
	{
		if (task->getDeadline() < QDateTime::currentDateTime())
		{
			item->setTextColor(1, Qt::red);
			item->setTextColor(2, Qt::red);
			item->setTextColor(3, Qt::red);
		}
	}
	//关联复选框信号和槽
	connect(qcb, SIGNAL(clicked(bool)), this, SLOT(Changetaskstate(bool)));
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
QTreeWidgetItem* TODO::getPlanItemById(int planId)
{
	//plan的增删改
	int Counts = ui.MyDailyPlanlist->topLevelItemCount();
	QTreeWidgetItem* var = ui.MyDailyPlanlist->topLevelItem(0);
	for (int i = 0; i < Counts; i++)
	{
		if (var->data(0, Qt::UserRole).toInt() == planId)
		{
			return var;
		}
		var = ui.MyDailyPlanlist->itemBelow(var);
	}
	return NULL;
}
QTreeWidgetItem* TODO::insertItem(Task* task, QTreeWidget* widget, TaskState state, TaskPlace place, QTreeWidgetItem* father)
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
		return item;
	}
	else if (place == Son)
	{
		//新建item
		QTreeWidgetItem* son = newItem(task->getId(), task);
		//添加item
		father->addChild(son);
		//构造复选框
		QCheckBox* qcb = newQCheckbox(son, task, state);
		//添加复选框
		widget->setItemWidget(son, 0, qcb);
		return son;
	}
	else
	{
		return NULL;
	}
}
void TODO::setItemColor(QColor color, QTreeWidgetItem* item)
{
	item->setTextColor(1, color);
	item->setTextColor(2, color);
	item->setTextColor(3, color);
}
void TODO::setQcheckBoxState(QCheckBox* qcb, TaskState state)
{
	if (state == Finish)
		qcb->setCheckState(Qt::Checked);
	else
		qcb->setCheckState(Qt::Unchecked);
}
//复选框
int TODO::Changetaskstate(bool id)
{
	//完成任务只能在Todo中
	if (id && ui.TrewTodoTasklist->geometry().contains(this->mapFromGlobal(QCursor::pos())))
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
		datasource.TaskFinishOnce(taskId);
	}
	//重做任务//如果finish有焦点
	else if (!id && ui.TrewFinishTasklist->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//修改数组数据
		datasource.TaskRedo(taskId, FinishArray);
	}
	//如果todo有焦点
	else if (!id && ui.TrewTodoTasklist->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QTreeWidgetItem* t = ui.TrewTodoTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//修改数组数据
		datasource.TaskRedo(taskId, TodoArray);
	}
	else if (id && ui.MyDailyPlanlist->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QTreeWidgetItem* t = ui.MyDailyPlanlist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int planId = t->data(0, Qt::UserRole).toUInt();
		datasource.finishPlan(planId);
	}
	else if (!id && ui.MyDailyPlanlist->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QTreeWidgetItem* t = ui.MyDailyPlanlist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int planId = t->data(0, Qt::UserRole).toUInt();
		datasource.PlanRedo(planId);
	}
	return 0;
}
//按钮
int TODO::OnBtnAddplan()
{
	if (ui.LinTaskname->text().isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("计划名不能为空"));
		return 0;
	}
	//构造task对象
	Plan* plan = new Plan(ui.LinTaskname->text(), ui.DteTaskdeadline->time());
	//加入数据源,界面会由信号机制自动刷新
	datasource.AddDailyArray(plan);
	//文本框置空
	ui.LinTaskname->setText("");
	//QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("插入成功"));
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
	//加入数据源,界面会由信号机制自动刷新
	datasource.AddTodoArray(task);
	//文本框置空
	ui.LinTaskname->setText("");
	//QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("插入成功"));
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