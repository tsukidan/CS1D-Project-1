#include "routedisplayer.h"
#include "sqldatabase.h"
#include "ui_routedisplayer.h"

RouteDisplayer::RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance) :
    QDialog(parent),
    ui(new Ui::RouteDisplayer),
    route(route),
    totalDistance(totalDistance)
{
    ui->setupUi(this);

    for (auto cityID: route)
    {
        QString cityName = SQLDatabase::GetCityNameById(cityID);
            ui->routeList->addItem(cityName);

    }


    ui->TotalDistance->setText(QString("%1").arg(totalDistance));
}

RouteDisplayer::~RouteDisplayer()
{
    delete ui;
}
