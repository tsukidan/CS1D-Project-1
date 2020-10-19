#include "displayfoodsforcity.h"
#include "ui_displayfoodsforcity.h"
#include "mainwindow.h"
#include "Header.h"

DisplayFoodsForCity::DisplayFoodsForCity(QWidget *parent, QSqlQuery query) :
    QDialog(parent),
    ui(new Ui::DisplayFoodsForCity),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    model->setQuery(query);
    ui->tableView->setModel(model);
}

DisplayFoodsForCity::~DisplayFoodsForCity()
{
    delete ui;
}
