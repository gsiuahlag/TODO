#ifndef TODO_H 
#define TODO_H


#include <QtWidgets/QMainWindow>
#include "ui_TODO.h"
#include "Task.h"
#include <list>
#include <QListWidgetItem>
#include "DataSource.h"
#include <QCheckBox>
#include <QStack>

class TODO : public QMainWindow
{
    Q_OBJECT

public:
    TODO(QWidget *parent = Q_NULLPTR);
private:
    //����task����һ��item������item���뵽treewidget��,����task״̬�����ø�ѡ��״̬,������ӽڵ��ṩ���ڵ�ָ��
    QTreeWidgetItem* insertItem(Task* task, QTreeWidget* widget, TaskState state, TaskPlace = Father, QTreeWidgetItem* = NULL);
    QTreeWidgetItem* newItem(int, Task* = NULL);
    QCheckBox* newQCheckbox(QTreeWidgetItem*, Task*, TaskState);
    QTreeWidgetItem* getTaskItemById(int taskId, QTreeWidget* widget);
    QTreeWidgetItem* getPlanItemById(int planId);
    void setItemColor(QColor, QTreeWidgetItem*);
    void setQcheckBoxState(QCheckBox* qcb, TaskState state);
    bool sortItem;
    QTimer* qtimer;
    DataSource datasource;

    QMenu* rightMenu;
    QAction* taskDelete;
    QAction* taskUpdate;
    QAction* taskNewChild;
    QAction* planDelete;
public slots:
    //ÿ�ռƻ�
    int DailyChange(QDate);
    int AddPlanItem(Plan*);
    int DeletePlanItem(Plan*, bool);
    int ChangePlanItem(Plan*);
    //��ť
	int OnBtnAddplan();
    int OnBtnAddtask();
    int OnBtnSorttask();
    //��ѡ��
    int Changetaskstate(bool);
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
    int RightClickedInDaily(QTreeWidgetItem*);
    //�Ҽ��˵�
    int DeleteTask();
    int UpdateTask();
    int NewChildTask();
    int DeletePlan();
private:
   // QAbstractItemModel* tasklistmodel;
    Ui::TODOClass ui;
};
#endif // 