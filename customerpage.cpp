#include "customerpage.h"
#include "ui_customerpage.h"
#include "displayfoodsforcity.h"
#include "mainwindow.h"
#include "routedisplayer.h"
#include "purchasefoods.h"
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
        ui->CityFoodSelect->addItem(city);
        ui->filterCartBox->addItem(city);
        ui->StartingCitySelect->addItem(city);

        // Checkboxes for cities to be visited
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setText(city);
        checkBox->setChecked(true);
        ui->CitySelectFrame->layout()->addWidget(checkBox);
        cityCheckBoxes.append(checkBox);
    }

    // Adjust combo box to size of contents
    ui->CityFoodSelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );
    ui->filterCartBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->StartingCitySelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );

    // sets index to nothing - won't show on dropdown
    ui->CityFoodSelect->setCurrentIndex(-1);
    ui->filterCartBox->setCurrentIndex(-1);
//    ui->StartingCitySelect->setCurrentIndex(-1);
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
void CustomerPage::on_CityFoodSelect_activated(const QString &selectedCity)
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

void CustomerPage::on_pushButton_clicked()
{
    // Create list of cities based on which ones were checked
    QList<int> selectedCities;
    for (QCheckBox * checkBox: cityCheckBoxes)
    {
        if (checkBox->isChecked())
            selectedCities.append(
                        SQLDatabase::GetCityIdByName(
                            checkBox->text()));
    }

    QString startingCity = ui->StartingCitySelect->currentText();

    int startingCityID = SQLDatabase::GetCityIdByName(startingCity);

    qDebug() << "Starting shortest path";

    QList<int> path = shortestPath(startingCityID, selectedCities);
    qDebug() << "Finished shortest path";

    // FIXME find a better way to get totalDistance from shortestPath
    // Takes totalDistance from shortestPath
    int totalDistance = path.back();
    // removes from path
    path.pop_back();

    QDialog * routeDisplay = new RouteDisplayer(this, path, totalDistance);
    routeDisplay->show();
}

QList<int> CustomerPage::shortestPath(int startingCity, QList<int>selectedCities)
{
    int totalDistance = 0;
    QList<int> visitedCities;
    QList<int> route;
    route << (startingCity);
    int currentCity = startingCity;
    bool canContinue = true;

    while (!selectedCities.isEmpty() && canContinue)
    {
        int nextCity = nearestCity(currentCity, visitedCities);
        qDebug() << "Looking at next city. " << " " << nextCity;
        qDebug() << "Visited cities " << visitedCities;
        qDebug() << "Selected cities " << selectedCities;
        qDebug() << "Route " << route;
        if (nextCity == 0)
            canContinue = false;
        else if (selectedCities.contains(nextCity))
        {
            int distance = SQLDatabase::GetDistance(currentCity, nextCity);
            qDebug() << "Total distance " << totalDistance << " + " << distance;
            totalDistance += distance;
            selectedCities.removeOne(nextCity);
            visitedCities << currentCity;
            route << nextCity;
            currentCity = nextCity;
        }
        else
        {
            // don't care about this city; query will ignore it
            visitedCities.append(nextCity);
        }
    }
    // inserting totalDistance into the route (same as append)
    // FIXME: find a better way to return the totalDistance to the caller
    route << totalDistance;
    return route;
}

// Looks for the nearest city and returns the toCity ID #
int CustomerPage::nearestCity(int currentCity, QList<int>visitedCities)
{
    QSqlQuery query;
    QString const queryText = "select toCity from Distances "
                              "where fromCity = (?) "
                              "and toCity not in (%1) "
                              "order by Distance limit 1";

    QVector<QString> placeholders(visitedCities.size(), "?");
    query.prepare(queryText.arg(QStringList::fromVector(placeholders).join(", ")));

    query.addBindValue(currentCity);
    for (auto const & i : visitedCities)
        query.addBindValue(i);

    qDebug() << "Running query for " << currentCity;

    if(!query.exec())
        qDebug() << "Failed: " << query.executedQuery() << " " << query.lastError();
    qDebug() << query.executedQuery();

    query.next();
    return query.value(0).toInt();
}

void CustomerPage::on_pushButton_2_clicked()
{
    //PROC - Create new window of purchase dialogue.
}

void CustomerPage::on_filterCartBox_activated(const QString &selectedCity)
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

        sqlModel->setQuery(query);
        ui->cartCityView->setModel(sqlModel);
    }

}

void CustomerPage::on_updateCart_clicked()
{
    // Begins new query
    QSqlQuery query;
    query.prepare("SELECT FROM Cities WHERE Name='"+queryVal+"'");

    // Tell the query which city we're looking for
    query.bindValue(":FoodName", queryVal);

    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();

    sqlModel->setQuery(query);
    ui->tableView->setModel(sqlModel);
}

void CustomerPage::on_cartCityView_activated(const QModelIndex &index)
{
       qDebug() << "activated " << index;
}

void CustomerPage::on_cartCityView_pressed(const QModelIndex &index)
{
        queryVal = ui->cartCityView->model()->data(index).toString();
        qDebug() << queryVal;
}
