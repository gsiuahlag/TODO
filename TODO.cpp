#include "TODO.h"
#include "Task.h"
#include "MyItemDrawer.h"
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
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
    
    Task* task = new Task(++taskNumber, "test1", QDateTime(QDate::currentDate()));
    taskTodoArray.push_back(task);
    QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
    item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));
    ui.TrewTodoTasklist->addTopLevelItem(item);
    QCheckBox* qcb = new QCheckBox();
    connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
    ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test2", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test3", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test4", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	
   
    //自定义绘制列表展示
    //ui.TrewTodoTasklist->setItemDelegate(new MyItemDrawer(ui.TrewTodoTasklist));

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnDeltask, SIGNAL(clicked()), this, SLOT(OnBtnDeltask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));


}
QTreeWidgetItem* TODO::insertItem(Task* task, QTreeWidget* widget)
{
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));

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


    return 0;
}