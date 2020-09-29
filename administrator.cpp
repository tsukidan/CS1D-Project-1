#include "administrator.h"
#include "ui_administrator.h"
#include "sqldatabase.h"

#include <QSqlQuery>
#include<QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Administrator::Administrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Administrator)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DB_PATH");

    db.open();

}

Administrator::~Administrator()
{
    delete ui;
}

void Administrator::on_cities_Button_clicked()
{
    QSqlQuery query;

    if(!query.exec("SELECT * FROM Cities"))
         qDebug() << "Failed: " << query.lastError();

    int tempCity = query.record().indexOf("Name");

    while(query.next()) {

        QString cityName = query.value(tempCity).toString();
        qDebug() << cityName;
    }
}

void Administrator::on_food_Button_clicked()
{

}

void Administrator::on_distances_Button_clicked()
{

}
