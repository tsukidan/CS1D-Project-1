#include "mainwindow.h"
#include "sqldatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    SQLDatabase db;
    db.createDatabase();
//    db.readFileCities();
//    db.readFileDistances();
//    db.readFileFoods();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}

