#include "customerpage.h"
#include "ui_customerpage.h"
#include "displayfoodsforcity.h"
#include "mainwindow.h"
#include <QComboBox>

/*!
 * \brief CustomerPage constructor
 */
CustomerPage::CustomerPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerPage),
    sqlModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
// this is for the table showing on the customer page
// this has been moved to display foods for city
//    sqlModel->setTable("Foods");
//    sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    sqlModel->select();

    sqlModel->setQuery("SELECT FoodName, Price FROM Foods");

    // to show table
    // ui->tableView->setModel(sqlModel);

    QSqlQuery query;
    query.exec("SELECT Name FROM Cities");
    while (query.next())
    {
        QString city = query.value(0).toString();
        ui->CitySelect->addItem(city);
    }

    // Adjust combo box to size of contents
    ui->CitySelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );

    // sets index to nothing - won't show on dropdown
    ui->CitySelect->setCurrentIndex(-1);
}

/*!
 * \brief CustomerPage destructor
 */
CustomerPage::~CustomerPage()
{
    delete ui;
}

/*!
 * \brief on_CitySelect_activated:
 * Takes the selection from a drop-down box and displays a separate box of
 * the selected city's foods and prices.
 */
void CustomerPage::on_CitySelect_activated(const QString &selectedCity)
{
    if (selectedCity != "")
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
        // sqlModel->setQuery(query);

        QDialog *displayfoodsforcity = new DisplayFoodsForCity(this, query);
        // set window title with the selected name of the city
        displayfoodsforcity -> setWindowTitle(selectedCity);
        // show the window for the selected city
        displayfoodsforcity->show();
    }

}

void CustomerPage::on_returnButton_clicked()
{
    MainWindow *mainWindow;
    mainWindow = new MainWindow(this);
    hide();
    mainWindow->show();
}
