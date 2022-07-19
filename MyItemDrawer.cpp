#include "MyItemDrawer.h"
#include "Task.h"
#include <QPainter>
MyItemDrawer::MyItemDrawer(QObject* parent) :QStyledItemDelegate(parent)
{}
MyItemDrawer::~MyItemDrawer() {}
void MyItemDrawer::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//目标矩形
	QRect rect = option.rect;
	rect.adjust(2, 2, -2, -2);

	Task* task = index.data(Qt::UserRole).value<Task*>();

	//绘制选中样式
	if (option.state & QStyle::State_Selected)
	{
		//设置笔刷颜色
		painter->setBrush(QColor(0xcc, 0xaa, 0xaa));
		//绘制背景色
		painter->drawRoundedRect(rect, 2, 2);
		painter->setBrush(Qt::NoBrush);
	}
	//绘制完成按钮
	if (1)
	{
		QRect dst = rect;
		dst.adjust(2, 2, -2, -2);
		dst.setRight(rect.left() + 10);
		painter->drawEllipse(dst);

	}
	//绘制任务名字
	if (1)
	{
		QRect dst = rect;
		dst.setLeft(rect.left() + 20);
		dst.setRight(rect.left() + 80);
		painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, task->getName());
	}
	//绘制任务截至时间
	if (1)
	{
		QRect dst = rect;
		dst.setLeft(rect.left() + 80);
		painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, task->getDeadline().toString(QString::fromLocal8Bit("yyyy年MM月dd日 hh:mm")));
	}


}
QSize MyItemDrawer::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//获取原始大小
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	//修改高度
	size.setHeight(20);
	return size;
}
