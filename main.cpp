#include "mainwindow.h"
#include "sqldatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
<<<<<<< HEAD
=======


>>>>>>> a596f8d2cde4996e4819504bf506aae065b0a520
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
<<<<<<< HEAD
=======

>>>>>>> a596f8d2cde4996e4819504bf506aae065b0a520

    SQLDatabase db;
    db.createDatabase();
    db.readFileCities();
    db.readFileDistances();
    db.readFileFoods();
}
