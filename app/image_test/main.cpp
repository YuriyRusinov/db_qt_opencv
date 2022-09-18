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

    return app->exec ();
}
