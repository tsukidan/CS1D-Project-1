#include "customerpage.h"
#include "ui_customerpage.h"
#include "displayfoodsforcity.h"

CustomerPage::CustomerPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerPage),
    sqlModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    //    sqlModel->setTable("Foods");
    //    sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //    sqlModel->select();

    sqlModel->setQuery("SELECT FoodName, Price FROM Foods");

    // to show table
    ui->tableView->setModel(sqlModel);

    QSqlQuery query;
    query.exec("SELECT Name FROM Cities");
    while (query.next()) {
        QString city = query.value(0).toString();
        ui->CitySelect->addItem(city);
    }

}

CustomerPage::~CustomerPage()
{
    delete ui;
}

void CustomerPage::on_CitySelect_currentIndexChanged(const QString &selectedCity)
{
    // Begins new query
    QSqlQuery query;
    // Prepared statement: Get every food for the current city.
    query.prepare("SELECT FoodName, Price FROM Foods "
                  "INNER JOIN Cities on cities.CityID = foods.CityID "
                  "WHERE cities.Name = (:City)");
    // Tell the query which city we're looking for
    query.bindValue(":City", selectedCity);
    // runs & cks for errors
    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();
    // Tells the model (that displays on the gui) to update it's state
    // Model will tell the GUI to update itself as well.
//    sqlModel->setQuery(query);

    QDialog *displayfoodsforcity = new DisplayFoodsForCity(this, query);
    displayfoodsforcity->show();
}
