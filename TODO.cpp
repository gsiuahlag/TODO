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
    //����ʱ�䷶Χ
    ui.DteTaskdeadline->setMinimumDate(QDate::currentDate().addDays(-365));
    ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
    //Ĭ��չʾ��ǰʱ��
    ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());
    //�޸ı�ͷ���
    ui.TrewTodoTasklist->setColumnWidth(0, 40);
    ui.TrewTodoTasklist->setColumnWidth(1, 80);
    ui.TrewTodoTasklist->setColumnWidth(2, 100);
	ui.TrewFinishTasklist->setColumnWidth(0, 40);
	ui.TrewFinishTasklist->setColumnWidth(1, 80);
	ui.TrewFinishTasklist->setColumnWidth(2, 100);
    //tasklistmodel = ui.TrewTodoTasklist->model();

    //�������ݷ������
    
    Task* task = new Task(++taskNumber, "test1", QDateTime::currentDateTime());
    taskTodoArray.push_back(task);
    QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
    item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
    ui.TrewTodoTasklist->addTopLevelItem(item);
    QCheckBox* qcb = new QCheckBox();
    connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
    ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test2", QDateTime::currentDateTime().addSecs(20));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test3", QDateTime::currentDateTime().addSecs(30));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test4", QDateTime::currentDateTime().addSecs(40));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
   
    //�Զ�������б�չʾ
    //ui.TrewTodoTasklist->setItemDelegate(new MyItemDrawer(ui.TrewTodoTasklist));

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));
	connect(ui.BtnDeltask, SIGNAL(clicked()), this, SLOT(OnBtnDeltask()));
	connect(ui.BtnSorttask, SIGNAL(clicked()), this, SLOT(OnBtnSorttask()));
	connect(ui.TrewTodoTasklist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(DoubleClickInwidget(QTreeWidgetItem*)));

}
int TODO::DoubleClickInwidget(QTreeWidgetItem* item)
{
	//��ȡ��ǰѡ��item��id
	int taskId = item->data(0, Qt::UserRole).toUInt();
	for each (Task * var in taskTodoArray)
	{
		if (var->getId() == taskId)
		{
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
			break;
		}
	}
	return 0;

}
QTreeWidgetItem* TODO::insertItem(Task* task, QTreeWidget* widget)
{
	//�½�item
	QTreeWidgetItem* item = new QTreeWidgetItem();
	//������ʾ���ݺ�task
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm:ss")));
	//���item
    widget->addTopLevelItem(item);
    return item;
}
int TODO::Changetaskstate(int id)
{
    if (id == 2)
    {
        //qDebug() << QString::fromLocal8Bit("ѡ��");
        //��ȡѡ��item
        QTreeWidgetItem* t = ui.TrewTodoTasklist->currentItem();
		int taskId = t->data(0, Qt::UserRole).toUInt();
        //�޸���������,������Ҫת�Ƶ�����
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
		//���첢����item
		QTreeWidgetItem* item = insertItem(var, ui.TrewFinishTasklist);
		//���츴ѡ��
		QCheckBox* qcb = new QCheckBox();
        qcb->setCheckState(Qt::Checked);
        //������ѡ���źźͲ�
		connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
		ui.TrewFinishTasklist->setItemWidget(item, 0, qcb);
        //ɾ��ԭitem
        delete t;

    }
    else if (id == 0)
    {
		//qDebug() << QString::fromLocal8Bit("δѡ��");
		//ȡ��ѡ��
		QTreeWidgetItem* t = ui.TrewFinishTasklist->currentItem();
		int taskId = t->data(0, Qt::UserRole).toUInt();
		//�޸���������,������Ҫת�Ƶ�����
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
		//���첢����item
		QTreeWidgetItem* item = insertItem(var, ui.TrewTodoTasklist);
		//���츴ѡ��
		QCheckBox* qcb = new QCheckBox();
		//������ѡ���źźͲ�
		connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
		ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
		//ɾ��ԭitem
		delete t;
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
    Task* task = new Task(++taskNumber, ui.LinTaskname->text(), ui.DteTaskdeadline->dateTime());
    taskTodoArray.push_back(task);
    //����һ������
    QTreeWidgetItem* item = insertItem(task,ui.TrewTodoTasklist);
    //���츴ѡ��
	QCheckBox* qcb = new QCheckBox();
    //������ѡ���źźͲ�
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);
	//������ı����ÿ�
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