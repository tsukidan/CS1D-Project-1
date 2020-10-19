#include "berlincitydisplay.h"
#include "sqldatabase.h"
#include "ui_berlincitydisplay.h"

/********************************
 *** CONSTRUCTOR / DESTRUCTOR ***
 ********************************/
BerlinCityDisplay::BerlinCityDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BerlinCityDisplay)
{
    ui->setupUi(this);


    int berlinID = SQLDatabase::GetCityIdByName("Berlin");
    QList<cityDistance> cityDistances = SQLDatabase::GetDistancesFromCity(berlinID);

    ui->berlinDisplay->setRowCount(cityDistances.size());

    for (int i = 0; i < cityDistances.size(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(cityDistances[i].cityName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->berlinDisplay->setItem(i, 0, item);

        item = new QTableWidgetItem(tr("%1").arg(cityDistances[i].distance));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->berlinDisplay->setItem(i, 1, item);
    }
}

// destructor
BerlinCityDisplay::~BerlinCityDisplay()
{
    delete ui;
}
