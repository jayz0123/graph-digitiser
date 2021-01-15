#include "graphdigitiser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphDigitiser w;
    w.show();

    return a.exec();
}
