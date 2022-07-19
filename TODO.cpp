#include "TODO.h"
#include "Task.h"
#include "MyItemDrawer.h"
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
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
    //修改表头宽度
    ui.TrewTodoTasklist->setColumnWidth(0, 40);
    ui.TrewTodoTasklist->setColumnWidth(1, 80);
    ui.TrewTodoTasklist->setColumnWidth(2, 100);
	ui.TrewFinishTasklist->setColumnWidth(0, 40);
	ui.TrewFinishTasklist->setColumnWidth(1, 80);
	ui.TrewFinishTasklist->setColumnWidth(2, 100);
    //tasklistmodel = ui.TrewTodoTasklist->model();

    //插入数据方便测试
    
    Task* task = new Task(++taskNumber, "test1", QDateTime::currentDateTime());
    taskTodoArray.push_back(task);
    QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
    item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
    ui.TrewTodoTasklist->addTopLevelItem(item);
    QCheckBox* qcb = new QCheckBox();
    connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
    ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test2", QDateTime::currentDateTime().addSecs(20));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test3", QDateTime::currentDateTime().addSecs(30));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test4", QDateTime::currentDateTime().addSecs(40));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
   
    //自定义绘制列表展示
    //ui.TrewTodoTasklist->setItemDelegate(new MyItemDrawer(ui.TrewTodoTasklist));

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnDeltask, SIGNAL(clicked()), this, SLOT(OnBtnDeltask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));

}
int TODO::DoubleClickInwidget(QTreeWidgetItem* item)
{
	//获取当前选中item的id
	int taskId = item->data(0, Qt::UserRole).toUInt();
	for each (Task * var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
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
			break;
		}
	}
	return 0;

}
QTreeWidgetItem* TODO::insertItem(Task* task, QTreeWidget* widget)
{
	//新建item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//设置显示数据和task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm:ss")));
	//添加item
    widget->addTopLevelItem(item);
    return item;
}
int TODO::Changetaskstate(int id)
{
    if (id == 2)
    {
        //qDebug() << QString::fromLocal8Bit("选中");
        //获取选中item
        QTreeWidgetItem* t = ui.TrewTodoTasklist->currentItem();
		int taskId = t->data(0, Qt::UserRole).toUInt();
        //修改数组数据,并保存要转移的数据
        Task* var=new Task();
		for each (var in taskTodoArray)
		{
			if (var->getId() == taskId)
			{
				taskTodoArray.remove(var);
                taskFinishArray.push_back(var);
				break;
			}
		}
		//构造并插入item
		QTreeWidgetItem* item = insertItem(var, ui.TrewFinishTasklist);
		//构造复选框
		QCheckBox* qcb = new QCheckBox();
        qcb->setCheckState(Qt::Checked);
        //关联复选框信号和槽
		connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
		ui.TrewFinishTasklist->setItemWidget(item, 0, qcb);
        //删除原item
        delete t;

    }
    else if (id == 0)
    {
		//qDebug() << QString::fromLocal8Bit("未选中");
		//取消选中
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//修改数组数据,并保存要转移的数据
		Task* var = new Task();
		for each (var in taskFinishArray)
		{
			if (var->getId() == taskId)
			{
				taskFinishArray.remove(var);
				taskTodoArray.push_back(var);
				break;
			}
		}
		//构造并插入item
		QTreeWidgetItem* item = insertItem(var, ui.TrewTodoTasklist);
		//构造复选框
		QCheckBox* qcb = new QCheckBox();
		//关联复选框信号和槽
		connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
		ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
		//删除原item
		delete t;
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
    Task* task = new Task(++taskNumber, ui.LinTaskname->text(), ui.DteTaskdeadline->dateTime());
    taskTodoArray.push_back(task);
    //插入一行数据
    QTreeWidgetItem* item = insertItem(task,ui.TrewTodoTasklist);
    //构造复选框
	QCheckBox* qcb = new QCheckBox();
    //关联复选框信号和槽
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
	//插入后文本框置空
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
    int taskId = t->data(0, Qt::UserRole).toUInt();
    for each (Task* var in taskFinishArray)
    {
        if (var->getId() == taskId)
        {
			taskFinishArray.remove(var);
            delete var;
            break;
        }
	}
	delete t;
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