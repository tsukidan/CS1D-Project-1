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
    sqlModel->setQuery("SELECT Name FROM Cities");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_food_Button_clicked()
{
    sqlModel->setQuery("SELECT FoodName, Price FROM Foods");
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

void Administrator::on_delete_Food_clicked()
{
    QSqlQuery q;
    QSqlQuery p;
    qDebug() << "ID: " << queryVal;
    q.prepare("DELETE FROM Foods WHERE FoodName='"+queryVal+"'");
    p.prepare("DELETE FROM Foods WHERE Price='"+queryVal+"'");
    if(!q.exec()||!p.exec())
        qDebug() << "Failed: " << q.lastError() << p.lastError();

    sqlModel->setQuery("SELECT FoodName, Price FROM Foods");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_delete_City_clicked()
{
    QSqlQuery q;
    qDebug() << "ID: " << queryVal;
    q.prepare("DELETE FROM Cities WHERE Name='"+queryVal+"'");
    if(!q.exec())
        qDebug() << "Failed: " << q.lastError();

    sqlModel->setQuery("SELECT Name FROM Cities");
    ui->databaseView->setModel(sqlModel);
}


void Administrator::on_databaseView_pressed(const QModelIndex &index)
{
    queryVal = ui->databaseView->model()->data(index).toString();
    qDebug() << queryVal;
}
