#include "customerpage.h"
#include "ui_customerpage.h"
#include "displayfoodsforcity.h"
#include "mainwindow.h"
#include "routedisplayer.h"
#include "berlincitydisplay.h"
#include <QComboBox>

/****************************************************************************
 * CLASS - class for Customer page. Includes constructor, destructor, method
 * for “home button” and method for data related to user’s drop-down menu
 * choices
 ****************************************************************************/

/****************************************************************************
 * METHOD: CUSTOMER PAGE CONSTRUCTOR
****************************************************************************/
CustomerPage::CustomerPage(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CustomerPage),
   sqlModel(new QSqlQueryModel(this))
{///begin constructor
   ui->setupUi(this);

/**********************************************************************
* PROCESSING - set correct number to display for ParisCitySelect
* spinbox
**********************************************************************/
   int max = SQLDatabase::GetSize() - 1; ///find maximum number, adjust w/ -1
   QSpinBox *parisSpinBox = ui->ParisCitySelect;
   parisSpinBox->setMaximum(max);
   parisSpinBox->setWrapping(true); ///wrap text for output

    sqlModel->setQuery("SELECT FoodName, Price FROM Foods");

   QSqlQuery query;
   query.exec("SELECT Name FROM Cities");
   while (query.next())
   {///begin while loop that handles city selection
/*********************************************************************
 * PROCESSING - use city choice to process food selections and
 * starting cities
 *********************************************************************/
       QString city = query.value(0).toString();
       ui->CityFoodSelect->addItem(city); ///add food selections based on city
       ui->StartingCitySelect->addItem(city); ///add starting city

    /*********************************************************************
     * PROCESSING - handle checkboxes for OUTPUT
     *********************************************************************/
       QCheckBox* checkBox = new QCheckBox();
       checkBox->setText(city); ///sets text next to checkbox display
       checkBox->setChecked(true);   ///check box in display
       ui->CitySelectFrame->layout()->addWidget(checkBox);
       cityCheckBoxes.append(checkBox); ///add to list of boxes to check
   }///end while loop

/**********************************************************************
* PROCESSING - Adjust combo box to size of contents
**********************************************************************/
   ui->CityFoodSelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );
   ui->StartingCitySelect->setSizeAdjustPolicy( QComboBox::AdjustToContents );

   /*************************************************************************
    * PROCESSING -sets index to nothing so that won't show in dropdown OUTPUT
   *************************************************************************/
   ui->CityFoodSelect->setCurrentIndex(-1);
   ui->StartingCitySelect->setCurrentIndex(-1);

}///end constructor

/****************************************************************************
* METHOD: CUSTOMER PAGE DESTRUCTOR - deletes pointer entirely
****************************************************************************/
CustomerPage::~CustomerPage()
{
   delete ui;
} ///end destructor

/****************************************************************************
* METHOD: CITYFOODSELECT_ACTIVATED - (void return)
* Takes the selection from a drop-down box and displays a separate box of
* the selected city's foods and prices.
****************************************************************************/
void CustomerPage::on_CityFoodSelect_activated(const QString &selectedCity)
{
   if (selectedCity != "") ///ensures the selected city is not a null entry
   {

       /// Begins new query
       QSqlQuery query;

/**********************************************************************
* PROCESSING - Prepared statement, retrieve every food selection for
* the current city.
**********************************************************************/
       query.prepare("SELECT FoodName, Price FROM Foods "
                     "INNER JOIN Cities on cities.CityID = foods.CityID "
                     "WHERE cities.Name = (:City)");

       /// Tell the query which city we're looking for
       query.bindValue(":City", selectedCity);

/**********************************************************************
 * PROCESSING - Runs to check for errors in the previous statement.
 * Output the error to qDebug
 **********************************************************************/

       if(!query.exec())
           qDebug() << "Failed: " << query.lastError();

/**********************************************************************
* OUTPUT - Tells the model (that displays on the GUI) to update its
* state, which in turn tells the GUI to update itself as well.
* Accomplished via sqlModel -> setQuery(query);
* Set window title and show it for selected city
**********************************************************************/

       QDialog *displayfoodsforcity = new DisplayFoodsForCity(this, query);
       /// selected name of city becomes window title
       displayfoodsforcity -> setWindowTitle(selectedCity);
       // display window for user
       displayfoodsforcity->show();
   } ///end if statement which checked against null entry for city

}///end method CITYFOODSELECT_ACTIVATED()

/****************************************************************************
* METHOD: on_returnButton_clicked() - (void return)
* Handles returning “Home” to the main window. Runs once the main program
* receives an entry which calls it, so no input is needed within method
****************************************************************************/
void CustomerPage::on_returnButton_clicked()
{
   MainWindow *mainWindow;
   mainWindow = new MainWindow(this); ///creates new main window
   hide(); ///hides previous window
   mainWindow->show(); ///displays new window
}

/****************************************************************************
* METHOD: on_pushButton_clicked() - (void return)
* Creates a customized city route based on which cities were selected.
* DOES NOT OUTPUT THE CUSTOMIZED CITY ROUTE but saves it to variables
****************************************************************************/

void CustomerPage::on_pushButton_clicked()
{
   /****************************************************************************
 * PROCESSING - Create list of cities based on user selection (drop down menu
 * earlier)
****************************************************************************/
   QList<int> selectedCities;
   for (QCheckBox * checkBox: cityCheckBoxes)
   {///begin for loop, runs through cities which were checked off
       if (checkBox->isChecked()) ///check if city is selected
           selectedCities.append(
                       SQLDatabase::GetCityIdByName(
                           checkBox->text())); ///append ID if checked
   }
   /*************************************************************************
    * PROCESSING - calculate shortest path given the list created above.
   *************************************************************************/
   QString startingCity = ui->StartingCitySelect->currentText();

   int startingCityID = SQLDatabase::GetCityIdByName(startingCity);

   qDebug() << "Starting shortest path";

   ///CALCULATE SHORTEST PATH
   QList<int> path = shortestPath(startingCityID, selectedCities, 0);
   qDebug() << "Finished shortest path";

    /// Takes totalDistance from shortestPath
   int totalDistance = path.back();
   /// removes remaining distance from path
   path.pop_back();

   /*************************************************************************
    * PROCESSING - set variables for shortest path length to output next
   *************************************************************************/

   QDialog * routeDisplay = new RouteDisplayer(this, path, totalDistance);
   routeDisplay->setWindowTitle(startingCity);
   routeDisplay->show();
}

/****************************************************************************
* METHOD: shortestPath - QList<int> return
* This method accepts the user’s starting city, the cities they have selected
* to go to, and the number of cities they wish to travel to. It then finds
* the shortest path between the cities.
****************************************************************************/
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

   // If using the Paris to selected # of cities
   if (numCities >= 1)
   {
       while (numCities > 0 && canContinue)
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
   }
   // If using selection boxes for different cities
   else
   {
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
   }
   // inserting totalDistance into the route (same as append)
    route << totalDistance;
   return route;
}


/****************************************************************************
* METHOD: shortestPath - int return (city ID#)
* This method accepts the user’s current city, the cities they have selected
* to go to, and the cities they have already visited. It calculates the
* nearest city, and makes sure the user has not already traveled there.
* It returns the city ID# of the closest new city! ****************************************************************************/

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


