#include "administrator.h"
#include "ui_administrator.h"
#include "sqldatabase.h"

#include "login.h"
#include <QSqlQuery>
#include<QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

Administrator::Administrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Administrator),
    sqlModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    sqlModel->setQuery("SELECT Name FROM Cities");
    ui->databaseView->setModel(sqlModel);
}

Administrator::~Administrator()
{
    delete ui;
}

void Administrator::on_cities_Button_clicked()
{
    sqlModel->setQuery("SELECT CityID,Name FROM Cities");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_food_Button_clicked()
{
    sqlModel->setQuery("SELECT FoodID,FoodName, Price FROM Foods");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_distances_Button_clicked()
{
    sqlModel->setQuery("SELECT Distance, FromCity, ToCity FROM Distances");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_returnFromAdminUI_clicked()
{
    Login *loginUi;
    loginUi = new Login(this);
    hide();
    loginUi->show();
}

void Administrator::on_databaseView_clicked(const QModelIndex &index)
{
}
