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

    void on_ParisGo_clicked();

    void on_BerlinView_clicked();

private:
    Ui::CustomerPage *ui;
// this was moved to display foods for city
//    QSqlTableModel *sqlModel;
    QSqlQueryModel *sqlModel;
    QList<QCheckBox*> cityCheckBoxes;    // List of all cities to visit

    QList<int> shortestPath(int startingCity, QList<int>selectedCities, int numCities);

    int nearestCity(int currentCity, QList<int>visitedCities);

};

#endif // CUSTOMERPAGE_H
