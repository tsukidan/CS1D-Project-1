#include "displayfoodsforcity.h"
#include "ui_displayfoodsforcity.h"
#include "mainwindow.h"
#include "Header.h"

/*!
 * \brief DisplayFoodsForCity constructor
 */

DisplayFoodsForCity::DisplayFoodsForCity(QWidget *parent, QSqlQuery query) :
    QDialog(parent),
    ui(new Ui::DisplayFoodsForCity),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    model->setQuery(query);
    ui->tableView->setModel(model);
}

/*!
 * \brief DisplayFoodsForCity destructor
 */
DisplayFoodsForCity::~DisplayFoodsForCity()
{
    delete ui;
}
