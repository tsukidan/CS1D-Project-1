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
     * \brief on_CitySelect_currentIndexChanged
     * Runs a query and displays the cities for the customer to select.
     * Takes the selection from a drop-down box and displays a separate box of
     * the selected city's foods and prices.
     */
     void on_CitySelect_currentIndexChanged(const QString &arg1);


    void on_returnButton_clicked();

private:
    Ui::CustomerPage *ui;
//    QSqlTableModel *sqlModel;
    QSqlQueryModel *sqlModel;

};

#endif // CUSTOMERPAGE_H
