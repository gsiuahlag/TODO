#include "TODO.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TODO w;
    w.show();
    return a.exec();
}
