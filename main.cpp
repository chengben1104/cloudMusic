#include "musicinterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicInterface w;
    w.show();

    return a.exec();
}
