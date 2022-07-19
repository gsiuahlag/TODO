#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"
#include "Task.h"
#include "MyItemDrawer.h"
#include <list>
#include <QListWidgetItem>
class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);
private:
    std::list <Task*> taskTodoArray;
    std::list <Task*> taskFinishArray;
    int taskNumber;
    //����task����һ��item������item���뵽treewidget��
    QTreeWidgetItem* insertItem(Task* task, QTreeWidget* widget);
    bool sortItem;
public slots:
    int OnBtnAddtask();
    int OnBtnDeltask();
    int OnBtnSorttask();
    int Changetaskstate(int);
    int DoubleClickInwidget(QTreeWidgetItem*);
private:
   // QAbstractItemModel* tasklistmodel;
    Ui::TODOClass ui;
};
