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
    
    Task* task = new Task(++taskNumber, "test1", QDateTime(QDate::currentDate()));
    taskTodoArray.push_back(task);
    QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
    item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));
    ui.TrewTodoTasklist->addTopLevelItem(item);
    QCheckBox* qcb = new QCheckBox();
    connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
    ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test2", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test3", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	task = new Task(++taskNumber, "test4", QDateTime(QDate::currentDate()));
	taskTodoArray.push_back(task);
	item = new QTreeWidgetItem();
	item->setData(0, Qt::UserRole, task->getId());
	item->setText(1, task->getName());
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));
	ui.TrewTodoTasklist->addTopLevelItem(item);
	qcb = new QCheckBox();
	connect(qcb, SIGNAL(stateChanged(int)), this, SLOT(Changetaskstate(int)));
	ui.TrewTodoTasklist->setItemWidget(item, 0, qcb);

	
   
    //�Զ�������б�չʾ
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
	item->setText(2, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));

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


    return 0;
}