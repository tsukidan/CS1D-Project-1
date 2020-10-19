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
    /// Administrator constructor function.
    ///
    /// Constructor is automatically called
    /// when object(instance of class) create,
    /// It is special member function of the class.
    explicit Administrator(QWidget *parent = nullptr);
    /// Administrator destructor.
    ///
    /// Deletes the ui when it's scope is left.
    ~Administrator();

private slots:
    /// on_cities_Button_clicked Imports the data from cities database.
    ///
    /// Creates a call to the database to retrun all cities.
    /// @return void
    void on_cities_Button_clicked();

    /// on_food_Button_clicked creates a query to the database.
    ///
    /// Creates a call to the database to retrun all food items belonging
    /// to each city. Updates the ui to display all food items.
    /// @return void
    void on_food_Button_clicked();

    /// on_distance_Button_clicked creates a query to the database.
    ///
    /// Creates a call to the database to retrun all distances from starting
    /// city to ending city. Updates the ui to display all food items.
    /// @return void
    void on_distances_Button_clicked();

    /// on_returnFromAdminUI_clicked returns to the previous ui.
    ///
    /// This button will take the user from the administrator page
    /// to the login page.
    /// @return void
    void on_returnFromAdminUI_clicked();

    /// on_DeleteFoodPushButton_clicked deletes a single item from database.
    ///
    /// When clicking the delete food button, the food item that was selected
    /// by the user will be deleted from the database.
    /// @return void
    void on_DeleteFoodPushButton_clicked();

    /// on_databaseView_pressed returns items clicked in QTableView.
    ///
    /// When clicked will delete a single item from the food database.
    /// @return const QModelIndex &index
    void on_databaseView_pressed(const QModelIndex &index);

    /// on_AddFoodPushButton_clicked
    ///
    /// When clicking the button, the user will be prompted to enter a city name
    /// as well as the name and price of a food item that will be added to the
    /// database.
    /// @return void
    void on_AddFoodPushButton_clicked();

    /// on_UpdateFoodPushButton_clicked
    ///
    /// When clicking the update food button, the program will query the database
    /// to update the price of the item whose name was entered to the new
    /// price that was entered.
    /// @return void
    void on_UpdateFoodPushButton_clicked();

    /// on_AddFromFilePushButton_clicked
    ///
    /// When clicking the button, the program will read in additional data added
    /// to an input file. It will add the informstion to the database.
    /// @return void
    void on_AddFromFilePushButton_clicked();

private:
    Ui::Administrator *ui;
    QSqlQueryModel *sqlModel;
    QString queryVal;

    void resetDatabaseView(QAbstractItemModel* model);
    QSqlRelationalTableModel* rebuildQuery();
};


#endif // ADMINISTRATOR_H
