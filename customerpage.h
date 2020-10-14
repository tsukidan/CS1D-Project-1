#ifndef CUSTOMERPAGE_H
#define CUSTOMERPAGE_H

#include "Header.h"
#include "foodshoppingcartitem.h"

namespace Ui {
class CustomerPage;
}

class CustomerPage : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerPage(QWidget *parent = nullptr);
    ~CustomerPage();

private slots:
    /*!
     * \brief on_CitySelect_activated
     * Runs a query and displays the cities for the customer to select.
     * Takes the selection from a drop-down box and displays a separate box of
     * the selected city's foods and prices.
     */
    void on_CityFoodSelect_activated(const QString &selectedCity);

    void on_returnButton_clicked();

    void on_pushButton_clicked();

<<<<<<< HEAD
    void on_pushButton_2_clicked();

    void on_filterCartBox_activated(const QString &arg1);

    void on_updateCartButton_clicked();
=======
    void on_ParisGo_clicked();
>>>>>>> a28f9f2c01dbae2334f91a5b432e4b70d94c970a

private:
    QList<int> route;
    int totalDistance;
    Ui::CustomerPage *ui;
    // this was moved to display foods for city
    //    QSqlTableModel *sqlModel;
    QSqlQueryModel *sqlModel;
    QString queryVal;
    QList<QCheckBox*> cityCheckBoxes;    // List of all cities to visit
    double cartTotal = 0.0;

    QList<int> shortestPath(int startingCity, QList<int>selectedCities, int numCities);

    int nearestCity(int currentCity, QList<int>visitedCities);

};

#endif // CUSTOMERPAGE_H
