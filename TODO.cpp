#include "TODO.h"
#include "Task.h"
#include "MyItemDrawer.h"
#include <QMessageBox>
TODO::TODO(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //����ʱ�䷶Χ
    ui.DteTaskdeadline->setMinimumDate(QDate::currentDate().addDays(-365));
    ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
    //Ĭ��չʾ��ǰʱ��
    ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());

	
    tasklistmodel = ui.LwgTasklist->model();
    //�Զ�������б�չʾ
    ui.LwgTasklist->setItemDelegate(new MyItemDrawer(ui.LwgTasklist));

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));



}
int TODO::OnBtnAddtask()
{
    if (ui.LinTaskname->text().isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������Ϊ��"));
        return 0;
    }
    //����task����
    QString taskname;
    QDateTime taskdeadline;
    taskname = ui.LinTaskname->text();
    taskdeadline = ui.DteTaskdeadline->dateTime();
    Task task(taskname, taskdeadline);
    //����һ������
    tasklistmodel->insertRow(0);
    tasklistmodel->setData(tasklistmodel->index(0, 0), QVariant::fromValue(task), Qt::UserRole);
	//������ı����ÿ�
    ui.LinTaskname->setText("");
    //QMessageBox::information(this, QString::fromLocal8Bit("�ɹ�"), QString::fromLocal8Bit("����ɹ�"));
    return 0;
}