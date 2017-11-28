#include "calibtool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalibTool w;
    w.show();

    return a.exec();
}
