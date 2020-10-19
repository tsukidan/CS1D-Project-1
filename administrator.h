#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Header.h"
#include "sqldatabase.h"

namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = nullptr);
    ~Administrator();

private slots:

    /*************************************************************************
     * void Administrator::on_cities_Button_clicked()
     * ----------------------------------------------------------------------
     * This button will display all the cities on the datable
     ************************************************************************/
    void on_cities_Button_clicked();

    /*************************************************************************
     * void Administrator::on_food_Button_clicked()
     * ----------------------------------------------------------------------
     * This will display the City ID, City Name, Food Name, and price of all
     * food items in the database.
     ************************************************************************/
    void on_food_Button_clicked();

    /*************************************************************************
     * void Administrator::on_distances_Button_clicked()
     * ----------------------------------------------------------------------
     * This button will display the to and from distances of each city as
     * well the to city's name and the from city's name
     ************************************************************************/
    void on_distances_Button_clicked();

    /*************************************************************************
     * void Administrator::on_returnFromAdminUI_clicked()
     * ----------------------------------------------------------------------
     * This button will take the user from the administrator page to the login
     * page. It essentially acts as a back button.
     ************************************************************************/
    void on_returnFromAdminUI_clicked();

    /*************************************************************************
     * void Administrator::on_DeleteFoodPushButton_clicked()
     * ----------------------------------------------------------------------
     * When clicking the delete food button, the food item that was selected
     * by the user will be deleted from the database.
     ************************************************************************/
    void on_DeleteFoodPushButton_clicked();

    ///Print the database view
    void on_databaseView_pressed(const QModelIndex &index);

    /*************************************************************************
     * void Administrator::on_AddFoodPushButton_clicked()
     * ----------------------------------------------------------------------
     * When clicking the button, the user will be prompted to enter a city name
     * as well as the name and price of a food item that will be added to the
     * database.
     ************************************************************************/
    void on_AddFoodPushButton_clicked();

    /*************************************************************************
     * void Administrator::on_UpdateFoodPushButton_clicked()
     * ----------------------------------------------------------------------
     * When clicking the update food button, the program will query the database
     * to update the price of the item whose name was entered to the new
     * price that was entered.
     ************************************************************************/
    void on_UpdateFoodPushButton_clicked();

    /*************************************************************************
     * void Administrator::on_AddFromFilePushButton_clicked()
     * ----------------------------------------------------------------------
     * When clicking the button, the program will read in additional data added
     * to an input file. It will add the informstion to the database.
     ************************************************************************/
    void on_AddFromFilePushButton_clicked();

private:
    Ui::Administrator *ui;
    QSqlQueryModel *sqlModel;
    QString queryVal;

    void resetDatabaseView(QAbstractItemModel* model);
    QSqlRelationalTableModel* rebuildQuery();
};


#endif // ADMINISTRATOR_H
