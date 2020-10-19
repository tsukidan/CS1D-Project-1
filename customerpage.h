#ifndef CUSTOMERPAGE_H
#define CUSTOMERPAGE_H

#include "Header.h"

namespace Ui {
class CustomerPage;
}

class CustomerPage : public QDialog
{
    Q_OBJECT

public:
    /// CustomerPage constructor
    ///
    /// Populates the page with data from the Citites and Food database,
    /// and adds buttons to manipulate the data.
    explicit CustomerPage(QWidget *parent = nullptr);

    /// ~CustomerPage destructor.
    ///
    /// Deletes the ui when the page is out of scope.
    ~CustomerPage();

private slots:
    /// on_CitySelect_activated.
    ///
    /// Runs a query and displays the cities for the customer to select.
    /// Takes the selection from a drop-down box and displays a separate box of
    /// the selected city's foods and prices.
    /// @param const QString &
    /// @return void
    void on_CityFoodSelect_activated(const QString &selectedCity);

    /// on_returnButton_clicked
    ///
    /// This function will return the user back to the home page.
    /// @return void
    void on_returnButton_clicked();

    /// on_pushButton_clicked
    ///
    /// This button will take the user to a customized city route.
    /// @return void
    void on_pushButton_clicked();

    /// on_ParisGo_clicked
    ///
    /// This function will connect value chanaged from ParisCitySelect to Go button
    /// Then run shortest path, and display the list of cities as well as
    /// the distance.
    /// @see ParisCitySelect()
    /// @return void
    void on_ParisGo_clicked();

    /// on_BerlinView_clicked
    ///
    /// This function will display the routes for the shortest distance starting
    /// at Berlin.
    /// @return void
    void on_BerlinView_clicked();

private:
    Ui::CustomerPage *ui;
    QSqlQueryModel *sqlModel;
    QList<QCheckBox*> cityCheckBoxes;    // List of all cities to visit

    QList<int> shortestPath(int startingCity, QList<int>selectedCities, int numCities);

    int nearestCity(int currentCity, QList<int>visitedCities);

};

#endif // CUSTOMERPAGE_H
