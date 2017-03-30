#include "trhlogger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrhLogger w;
    w.show();

    return a.exec();
}
