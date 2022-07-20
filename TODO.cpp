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


    //�������ݷ������
    
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
		var->parseFromNewtask(changetaskinfomation.getTask());
		//������ʾ
		item->setText(1, var->getName());
		item->setText(2, var->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));

	}
	return 0;

}
void TODO::insertItem(Task* task, QTreeWidget* widget, TaskState state)
{
	//�½�item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//������ʾ���ݺ�task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("MM��dd�� hh:mm:ss")));
	item->setText(3, QString::number(task->getLastRepeatCount()));

	//���item
	widget->addTopLevelItem(item);
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
		}
	}
	//������ѡ���źźͲ�
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	widget->setItemWidget(item, 0, qcb);
	
}
int TODO::Changetaskstate(int id)
{
    if (id == 2)
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
		Task* var = datasource.TaskFinishOnce(taskId,t);		
    }
    else if (id == 0)
    {
		//qDebug() << QString::fromLocal8Bit("δѡ��");
		//ȡ��ѡ��
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		if (t == NULL)
		{
			return 0;
		}
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//ɾ��ԭitem
		delete t;
		//�޸���������,������Ҫת�Ƶ�����
		Task* var = datasource.Translate(FinishArray, TodoArray, taskId, t);
		//���첢����item
		insertItem(var, ui.TrewTodoTasklist, Todo);
		
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
	datasource.deleteFromFinishArray(taskId,t);
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