#include "mainwindow.h"
#include <QApplication>

#include "shopstate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ShopState::pShop = new ShopState();

    MainWindow w;
    w.show();

    return a.exec();
}
