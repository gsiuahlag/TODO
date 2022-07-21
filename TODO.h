#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"
#include "Task.h"
#include <list>
#include <QListWidgetItem>
#include "DataSource.h"
#include <QCheckBox>

class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);
private:
    //����task����һ��item������item���뵽treewidget��,����task״̬�����ø�ѡ��״̬,������ӽڵ��ṩ���ڵ�ָ��
    void insertItem(Task* task, QTreeWidget* widget, TaskState state, TaskPlace = Father, QTreeWidgetItem* = NULL);
    QTreeWidgetItem* newItem(int, Task* = NULL);
    QCheckBox* newQCheckbox(QTreeWidgetItem*, Task*, TaskState);
    QTreeWidgetItem* getTaskItemById(int taskId, QTreeWidget* widget);
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
    //��ť
    int OnBtnAddtask();
    int OnBtnDeltask();
    int OnBtnSorttask();
    //��ѡ��
    int Changetaskstate(int);
    //˫��
    int DoubleClickInwidget(QTreeWidgetItem*);
    //��ʱ��
    int TimeOut();
    //�������
    int AddTaskItem(TaskArray taskarray, Task* task, TaskPlace place, Task* father);
    int DeleteTaskItem(TaskArray, Task*);
	int ChangeTaskItem(TaskArray, Task*);
    //�Ҽ�
    int RightClickedInTodo(QTreeWidgetItem*);
	int RightClickedInFinish(QTreeWidgetItem*);
    int DeleteTask();
    int UpdateTask();
    int NewChildTask();
private:
   // QAbstractItemModel* tasklistmodel;
    Ui::TODOClass ui;
};
