#include "customerpage.h"
#include "ui_customerpage.h"
#include "displayfoodsforcity.h"
#include "mainwindow.h"
#include "routedisplayer.h"
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

    // Sets correct number to display for the ParisCitySelect spinbox
    int max = SQLDatabase::GetSize() - 1;
    QSpinBox *parisSpinBox = ui->ParisCitySelect;
    parisSpinBox->setMaximum(max);
    parisSpinBox->setWrapping(true);


    QSqlQuery query;
    query.exec("SELECT Name FROM Cities");
    while (query.next())
    {
        QString city = query.value(0).toString();
        ui->CityFoodSelect->addItem(city);
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
    ui->StartingCitySelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );

    // sets index to nothing - won't show on dropdown
    ui->CityFoodSelect->setCurrentIndex(-1);

    ui->StartingCitySelect->setCurrentIndex(-1);
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

// Return "Home" button
void CustomerPage::on_returnButton_clicked()
{
    MainWindow *mainWindow;
    mainWindow = new MainWindow(this);
    hide();
    mainWindow->show();
}

// Button for customized city route
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

     // Takes totalDistance from shortestPath
    int totalDistance = path.back();
    // removes from path
    path.pop_back();

    QDialog * routeDisplay = new RouteDisplayer(this, path, totalDistance);
    routeDisplay->show();
}


// Shortest path for route display
QList<int> CustomerPage::shortestPath(int startingCity,
                                      QList<int>selectedCities, int numCities)
{
    int totalDistance = 0;
    QList<int> visitedCities;
    QList<int> route;
    route << (startingCity);
    int currentCity = startingCity;
    int totNumCities = numCities;
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

            int nextCity = nearestCity(currentCity, visitedCities);
            qDebug() << "*** PARIS *** Looking at next city. " << " " << nextCity;
            qDebug() << "Visited cities " << visitedCities;
            qDebug() << "Selected cities " << selectedCities;
            qDebug() << "Route " << route;
            if (nextCity == 0)
            {
                canContinue = false;
                qDebug() << "nextCity == 0 canContinue " << canContinue;
            }
            else if (numCities > 1)
            {
                int distance = SQLDatabase::GetDistance(currentCity, nextCity);
                qDebug() << "Total distance " << totalDistance << " + " << distance;
                totalDistance += distance;
                qDebug() << "NumCities before -- " << numCities;
                numCities--;
                qDebug() << "NumCities after " << numCities;
                visitedCities << currentCity;
                route << nextCity;
                currentCity = nextCity;
                if (route.size() - 1 > totNumCities)
                {
                     qDebug() << "Route size = " << route.size();
                     canContinue = false;
                     qDebug() << "canContinue " << canContinue;
                }
            }
            else
            {
                // don't care about this city; query will ignore it
                visitedCities.append(nextCity);
            }
        }
        else
        {
            // don't care about this city; query will ignore it
            visitedCities.append(nextCity);
        }
    }
    // inserting totalDistance into the route (same as append)
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

// Paris trip button
// Connect value chanaged from ParisCitySelect to Go button
// Then run shortest path, and display the list of cities as well as
// the distance.
void CustomerPage::on_ParisGo_clicked()
{
     // Get the number of cities selected
    int numCities;
    numCities = ui->ParisCitySelect->value() + 1;
    qDebug() << "numCities " << numCities;

    // Create list of cities
    QList<int> numSelectedCities;

    // Starting City
    QString startingCity = "Paris";
    int startingCityID = SQLDatabase::GetCityIdByName(startingCity);

    qDebug() << "Starting shortest path";

    QList<int> path = shortestPath(startingCityID, numSelectedCities, numCities);
    qDebug() << "Finished shortest path";

    // Takes totalDistance from shortestPath
    int totalDistance = path.back();
    // removes from path
    path.pop_back();

    QDialog * routeDisplay = new RouteDisplayer(this, path, totalDistance);
    routeDisplay->setWindowTitle(startingCity);
    routeDisplay->show();

}

// Berlin trip button
void CustomerPage::on_BerlinView_clicked()
{

    QDialog * berlinCityDisplay = new BerlinCityDisplay(this);
    berlinCityDisplay->setWindowTitle("Cities from Berlin");
    berlinCityDisplay->show();

}
