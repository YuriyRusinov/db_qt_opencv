#include <QApplication>
#include <QMainWindow>
#include <QTranslator>
#include <QDir>
#include <QtDebug>

#include "imagetestmainwindow.h"

int main (int argc, char * argv[])
{
    QApplication * app = new QApplication (argc, argv);

    QMainWindow * rw = new ImageTestMainWindow;

    rw->show ();

    int retVal = app->exec();

    if(rw)
        delete rw;

    return retVal;
}
