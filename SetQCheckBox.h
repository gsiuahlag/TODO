#ifndef SETQCHECKBOX_H
#define SETQCHECKBOX_H
#include <QCheckBox>
#include "AfThread.h"
class SetQCheckBox : public AfThread
{
public:
	SetQCheckBox(QCheckBox*,bool);
	~SetQCheckBox();
	virtual int ThreadMain();
public:
	QCheckBox* qcb;
	bool state;
};










#endif // SETQCHECKBOX_H

