#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"
#include "Task.h"
#include <list>
#include <QListWidgetItem>
#include "DataSource.h"


class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);
private:
    //����task����һ��item������item���뵽treewidget��,����task״̬�����ø�ѡ��״̬
    void insertItem(Task* task, QTreeWidget* widget, TaskState state);
    bool sortItem;
    QTimer* qtimer;
    DataSource datasource;
    const QFont taskTimeout;
    const QFont taskFinish;
    const QFont taskTodo;

public slots:
    int OnBtnAddtask();
    int OnBtnDeltask();
    int OnBtnSorttask();
    int Changetaskstate(int);
    int DoubleClickInwidget(QTreeWidgetItem*);
    int TimeOut();
    int AddTask(TaskArray, Task*);
    int DeleteTask(TaskArray, Task*, QTreeWidgetItem*);
	int ChangeTask(TaskArray, Task*, QTreeWidgetItem*);

private:
   // QAbstractItemModel* tasklistmodel;
    Ui::TODOClass ui;
};
