#include "MyItemDrawer.h"
#include "Task.h"
#include <QPainter>
MyItemDrawer::MyItemDrawer(QObject* parent) :QStyledItemDelegate(parent)
{}
MyItemDrawer::~MyItemDrawer() {}
void MyItemDrawer::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//Ŀ�����
	QRect rect = option.rect;
	rect.adjust(2, 2, -2, -2);

	Task* task = index.data(Qt::UserRole).value<Task*>();

	//����ѡ����ʽ
	if (option.state & QStyle::State_Selected)
	{
		//���ñ�ˢ��ɫ
		painter->setBrush(QColor(0xcc, 0xaa, 0xaa));
		//���Ʊ���ɫ
		painter->drawRoundedRect(rect, 2, 2);
		painter->setBrush(Qt::NoBrush);
	}
	//������ɰ�ť
	if (1)
	{
		QRect dst = rect;
		dst.adjust(2, 2, -2, -2);
		dst.setRight(rect.left() + 10);
		painter->drawEllipse(dst);

	}
	//������������
	if (1)
	{
		QRect dst = rect;
		dst.setLeft(rect.left() + 20);
		dst.setRight(rect.left() + 80);
		painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, task->getName());
	}
	//�����������ʱ��
	if (1)
	{
		QRect dst = rect;
		dst.setLeft(rect.left() + 80);
		painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, task->getDeadline().toString(QString::fromLocal8Bit("yyyy��MM��dd�� hh:mm")));
	}


}
QSize MyItemDrawer::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//��ȡԭʼ��С
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	//�޸ĸ߶�
	size.setHeight(20);
	return size;
}
