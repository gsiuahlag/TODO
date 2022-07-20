#include "TODO.h"
#include "Task.h"
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <QTimer>
#include "ChangeTaskInfomation.h"

TODO::TODO(QWidget *parent)
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


    //插入数据方便测试
    
	Task* task = new Task("test1", QDateTime::currentDateTime(), true, 5);
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test2", QDateTime::currentDateTime().addSecs(10), true, 2);
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test3", QDateTime::currentDateTime().addSecs(15), true, 5);
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	task = new Task("test4", QDateTime::currentDateTime().addSecs(20), true, 4);
	datasource.AddTodoArray(task);
	insertItem(task, ui.TrewTodoTasklist, Todo);

	qtimer = new QTimer(this);
	qtimer->start(1000);

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnDeltask, SIGNAL(clicked()), this, SLOT(OnBtnDeltask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));
	connect(qtimer, SIGNAL(timeout()), this, SLOT(TimeOut()));
	connect(&datasource, SIGNAL(AddTask(TaskArray, Task*)), this, SLOT(AddTask(TaskArray, Task*)));
	connect(&datasource, SIGNAL(DeleteTask(TaskArray, Task*, QTreeWidgetItem*)), this, SLOT(DeleteTask(TaskArray, Task*, QTreeWidgetItem*)));
	connect(&datasource, SIGNAL(ChangeTask(TaskArray, Task*, QTreeWidgetItem*)), this, SLOT(ChangeTask(TaskArray, Task*, QTreeWidgetItem*)));

}
int TODO::AddTask(TaskArray taskarray, Task* task)
{
	if (taskarray == TodoArray)
	{
		insertItem(task, ui.TrewTodoTasklist, Todo);
	}
	else if (taskarray == FinishArray)
	{
		insertItem(task, ui.TrewFinishTasklist, Finish);
	}
	return 0;
}
int TODO::DeleteTask(TaskArray taskarray, Task* task, QTreeWidgetItem* t)
{
	delete t;
	return 0;
}
int TODO::ChangeTask(TaskArray taskarray, Task* task, QTreeWidgetItem* t)
{
	t->setText(3, QString::number(task->getLastRepeatCount()));
	QCheckBox* qcb = (QCheckBox*)ui.TrewTodoTasklist->itemWidget(t, 0);
	qcb->setCheckState(Qt::Unchecked);
	return 0;
}

int TODO::TimeOut()
{
	QDateTime Nowtime = QDateTime::currentDateTime();
	ui.DteNow->setDateTime(Nowtime);
	int Counts = ui.TrewTodoTasklist->topLevelItemCount();
	QTreeWidgetItem* var = ui.TrewTodoTasklist->topLevelItem(0);
	for (int i = 0; i < Counts; i++)
	{
		Task* task = datasource.getFromTodoArray(var->data(0, Qt::UserRole).toInt());
		if (task->getDeadline() < Nowtime)
		{
			var->setTextColor(1, Qt::red);
			var->setTextColor(2, Qt::red);
			var->setTextColor(3, Qt::red);
		}
		else
		{
			var->setTextColor(1, Qt::black);
			var->setTextColor(2, Qt::black);
			var->setTextColor(3, Qt::black);
		}
		var = ui.TrewTodoTasklist->itemBelow(var);
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
		var->parseFromNewtask(changetaskinfomation.getTask());
		//更新显示
		item->setText(1, var->getName());
		item->setText(2, var->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));

	}
	return 0;

}
void TODO::insertItem(Task* task, QTreeWidget* widget, TaskState state)
{
	//新建item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//设置显示数据和task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM月dd日 hh:mm:ss")));
	item->setText(3, QString::number(task->getLastRepeatCount()));

	//添加item
	widget->addTopLevelItem(item);
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
		}
	}
	//关联复选框信号和槽
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	widget->setItemWidget(item, 0, qcb);
	
}
int TODO::Changetaskstate(int id)
{
    if (id == 2)
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
		Task* var = datasource.TaskFinishOnce(taskId,t);		
    }
    else if (id == 0)
    {
		//qDebug() << QString::fromLocal8Bit("未选中");
		//取消选中
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//删除原item
		delete t;
		//修改数组数据,并保存要转移的数据
		Task* var = datasource.Translate(FinishArray, TodoArray, taskId, t);
		//构造并插入item
		insertItem(var, ui.TrewTodoTasklist, Todo);
		
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
	datasource.deleteFromFinishArray(taskId,t);
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