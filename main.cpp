#include "mainwindow.h"
#include "sqldatabase.h"
#include <QApplication>
#include "shortestpath.h"
#include "pathutils.h"

int main(int argc, char *argv[])
{

    SQLDatabase db;
    db.createDatabase();
//    db.readFileCities();
//    db.readFileDistances();
//    db.readFileFoods();

//    cityGraph graph = createCityGraphFromDatabase();
//    vector<string> path = ShortestPath(graph, {"Berlin"}, {"Prague"});
//    printShortestPath(path);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}

