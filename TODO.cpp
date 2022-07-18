#include "TODO.h"
#include "Task.h"
#include "MyItemDrawer.h"
#include <QMessageBox>
TODO::TODO(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //设置时间范围
    ui.DteTaskdeadline->setMinimumDate(QDate::currentDate().addDays(-365));
    ui.DteTaskdeadline->setMaximumDate(QDate::currentDate().addDays(365));
    //默认展示当前时间
    ui.DteTaskdeadline->setDateTime(QDateTime::currentDateTime());

	
    tasklistmodel = ui.LwgTasklist->model();
    //自定义绘制列表展示
    ui.LwgTasklist->setItemDelegate(new MyItemDrawer(ui.LwgTasklist));

    connect(ui.BtnAddtask, SIGNAL(clicked()), this, SLOT(OnBtnAddtask()));



}
int TODO::OnBtnAddtask()
{
    if (ui.LinTaskname->text().isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("任务名不能为空"));
        return 0;
    }
    //构造task对象
    QString taskname;
    QDateTime taskdeadline;
    taskname = ui.LinTaskname->text();
    taskdeadline = ui.DteTaskdeadline->dateTime();
    Task task(taskname, taskdeadline);
    //插入一行数据
    tasklistmodel->insertRow(0);
    tasklistmodel->setData(tasklistmodel->index(0, 0), QVariant::fromValue(task), Qt::UserRole);
	//插入后文本框置空
    ui.LinTaskname->setText("");
    //QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("插入成功"));
    return 0;
}