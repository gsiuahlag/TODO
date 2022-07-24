#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"
#include "Task.h"
#include <list>
#include <QListWidgetItem>
#include "DataSource.h"
#include <QCheckBox>
#include <QStack>
#include "SetQCheckBox.h"

class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);
private:
    //利用task创建一个item，并将item插入到treewidget中,根据task状态，设置复选框状态,如果是子节点提供父节点指针
    QTreeWidgetItem* insertItem(Task* task, QTreeWidget* widget, TaskState state, TaskPlace = Father, QTreeWidgetItem* = NULL);
    QTreeWidgetItem* newItem(int, Task* = NULL);
    QCheckBox* newQCheckbox(QTreeWidgetItem*, Task*, TaskState);
    QTreeWidgetItem* getTaskItemById(int taskId, QTreeWidget* widget);
    void setItemColor(QColor, QTreeWidgetItem*);
    void setQcheckBoxState(QCheckBox* qcb, TaskState state);

    bool sortItem;
    QTimer* qtimer;
    DataSource datasource;
    const QFont taskTimeout;
    const QFont taskFinish;
    const QFont taskTodo;
    QMenu* rightMenu;
    QAction* taskDelete;
    QAction* taskUpdate;
    QAction* taskNewChild;
public slots:
    //按钮
    int OnBtnAddtask();
    int OnBtnDeltask();
    int OnBtnSorttask();
    //复选框
    int Changetaskstate(bool);
    //双击
    int DoubleClickInwidget(QTreeWidgetItem*);
    //定时器
    int TimeOut();
    //界面更新
    int AddTaskItem(TaskArray taskarray, Task* task, TaskPlace place, Task* father);
    int DeleteTaskItem(TaskArray, Task*);
	int ChangeTaskItem(TaskArray, Task*);
    //右键
    int RightClickedInTodo(QTreeWidgetItem*);
	int RightClickedInFinish(QTreeWidgetItem*);
    //右键菜单
    int DeleteTask();
    int UpdateTask();
    int NewChildTask();
private:
   // QAbstractItemModel* tasklistmodel;
    Ui::TODOClass ui;
};
