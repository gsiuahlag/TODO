#include "SetQCheckBox.h"
#include <QDebug>
SetQCheckBox::SetQCheckBox(QCheckBox* a,bool b)
{
	this->qcb = a;
	this->state = b;
}
SetQCheckBox::~SetQCheckBox()
{
	qDebug() << QString::fromLocal8Bit("Ïß³ÌÏú»Ù");
}

int SetQCheckBox::ThreadMain()
{
	if (this->state)
	{
		qcb->setChecked(true);
	}
	else
	{
		qcb->setChecked(false);
	}
	return 0;
}