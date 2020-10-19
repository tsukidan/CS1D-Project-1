#include "administrator.h"
#include "ui_administrator.h"
#include "sqldatabase.h"
#include "Header.h"
#include "login.h"

/********************************
 *** CONSTRUCTOR / DESTRUCTOR ***
 ********************************/
Administrator::Administrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Administrator),
    sqlModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    sqlModel->setQuery("SELECT Name FROM Cities");
    ui->databaseView->setModel(sqlModel);

    /*************************************************************************
     * Change color of:
     *      - AddFoodPushButton
     *      - UpdateFoodPushButton
     *      - DeleteFoodPushButton
     *
     * To make it easier on the user to select each one
     ************************************************************************/
    ui -> AddFoodPushButton->setStyleSheet(
                QString("#%1 { background-color: cyan; }")
                .arg(ui -> AddFoodPushButton->objectName()));

    ui -> UpdateFoodPushButton->setStyleSheet(
                QString("#%1 { background-color: green; }")
                .arg(ui -> UpdateFoodPushButton->objectName()));

    ui -> DeleteFoodPushButton->setStyleSheet(
                QString("#%1 { background-color: pink; }")
                .arg(ui -> DeleteFoodPushButton->objectName()));
}

Administrator::~Administrator()
{
    delete ui;
}

void Administrator::on_cities_Button_clicked()
{
    QSqlRelationalTableModel *sqlTableModel = rebuildQuery();
    sqlTableModel->setTable("Cities");
    sqlTableModel->select();	// this runs the select query

    resetDatabaseView(sqlTableModel);
    ui->databaseView->hideColumn(0);
}

/*************************************************************************
 * void Administrator::on_food_Button_clicked()
 * ----------------------------------------------------------------------
 * This will display the City ID, City Name, Food Name, and price of all
 * food items in the database.
 ************************************************************************/

void Administrator::on_food_Button_clicked()
{
    QSqlRelationalTableModel *sqlTableModel; //PROC & PROC - Create query to change column names

    /*******************
     * INITIALIZATIONS *
     *******************/
    sqlTableModel = rebuildQuery();

    /*************************************************************************
     * PROCESSING - Run query to display information on each food item
     ************************************************************************/
    sqlTableModel->setHeaderData(0, Qt::Horizontal, tr("City ID"));
    sqlTableModel->setHeaderData(1, Qt::Horizontal, tr("City Name"));

    /*************************************************************************
     * PROCESSING - Run query to display information on each food item
     ************************************************************************/
    sqlModel->setQuery("SELECT Cities.CityID, Cities.Name, Foods.FoodName, "
                       "Foods.Price FROM Foods, Cities WHERE Cities.CityID = Foods.CityID");
    ui->databaseView->setModel(sqlModel);
}

void Administrator::on_distances_Button_clicked()
{
    QSqlRelationalTableModel *sqlTableModel = rebuildQuery();
    sqlTableModel->setTable("Distances");
    sqlTableModel->setRelation(0, QSqlRelation("Cities", "CityID", "Name"));
    sqlTableModel->setRelation(1, QSqlRelation("Cities", "CityID", "Name"));
    sqlTableModel->select();
    sqlTableModel->setHeaderData(0, Qt::Horizontal, tr("From"));
    sqlTableModel->setHeaderData(1, Qt::Horizontal, tr("To"));

    resetDatabaseView(sqlTableModel);
}

/*************************************************************************
 * void Administrator::on_returnFromAdminUI_clicked()
 * ----------------------------------------------------------------------
 * This button will take the user from the administrator page to the login
 * page. It essentially acts as a back button.
 ************************************************************************/

void Administrator::on_returnFromAdminUI_clicked()
{
    Login *loginUi; //PROC & PROC - Pointer to login interface

    /*******************
     * INITIALIZATIONS *
     *******************/
    loginUi = new Login(this);

    /*************************************************************************
     * PROCESSING - Hide Admin page and show the login page
     ************************************************************************/
    hide();
    loginUi->show();
}


void Administrator::on_databaseView_pressed(const QModelIndex &index)
{
    queryVal = ui->databaseView->model()->data(index).toString();
    qDebug() << queryVal;
}

/*************************************************************************
 * void Administrator::on_AddFoodPushButton_clicked()
 * ----------------------------------------------------------------------
 * When clicking the button, the user will be prompted to enter a city name
 * as well as the name and price of a food item that will be added to the
 * database.
 ************************************************************************/
void Administrator::on_AddFoodPushButton_clicked()
{
    QString foodName = ui -> FoodNameLineEdit -> text();
    //IN & PROC - Name of food item that will be added
    QString price    = ui -> PriceLineEdit    -> text();
    //IN & PROC - Price of food item will be added
    QSqlQuery query;

    /*************************************************************************
     * PROCESSING - Load datatable to show information
     ************************************************************************/
    sqlModel->setQuery("SELECT Cities.CityID, Cities.Name, "
                       "Foods.FoodName, Foods.Price "
                       "FROM Foods, Cities "
                       "WHERE Cities.CityID = Foods.CityID");
    ui->databaseView->setModel(sqlModel);

    /*************************************************************************
     * PROCESSING - Add to the database if a country was selected
     ************************************************************************/
    if(queryVal != 0)
    {//begin if

        /*************************************************************************
         * PROCESSING - Query the database to add the item
         ************************************************************************/
         query.prepare("INSERT OR IGNORE INTO Foods(FoodName, CityID, Price) "
                      "VALUES(:name, :city, :price)");

         //bind values
         query.bindValue(":name", foodName);
         query.bindValue(":city", queryVal.toInt());
         query.bindValue(":price", price);

        /*************************************************************************
         * OUTPUT - Error message if query fails
         ************************************************************************/
        if(!query.exec())
            qDebug() << "Failed: " << query.lastError() << " (" << queryVal << ")";

        qDebug() << "cityID:    " << queryVal;
        qDebug() << "cityID int:" << queryVal.toInt();

        qDebug() << ":name      " << foodName;
        qDebug() << ":price     " << price;
        qDebug() << "";

        /*************************************************************************
         * PROCESSING - Reload datatable to show updated information
         ************************************************************************/
        sqlModel->setQuery("SELECT Cities.CityID, Cities.Name, Foods.FoodName, Foods.Price FROM Foods, Cities WHERE Cities.CityID = Foods.CityID");
        ui->databaseView->setModel(sqlModel);

        //Clear the line edits
        ui -> FoodNameLineEdit -> clear();
        ui -> PriceLineEdit    -> clear();

    }//end if

}
   /*************************************************************************
    * void Administrator::on_DeleteFoodPushButton_clicked()
    * ----------------------------------------------------------------------
    * When clicking the delete food button, the food item that was selected
    * by the user will be deleted from the database.
    ************************************************************************/
    void Administrator::on_DeleteFoodPushButton_clicked()
{
    QSqlQuery queryName;  //PROC & PROC - Used to query database to delete name
    QSqlQuery queryPrice; //PROC & PROC - Used to query database to delete the price

    qDebug() << "ID: " << queryVal;

    /*************************************************************************
     * PROCESSING - Query the database to Delete the item and the price
     ************************************************************************/
    queryName.prepare ("DELETE FROM Foods WHERE FoodName='"+queryVal+"'");
    queryPrice.prepare("DELETE FROM Foods WHERE Price='"+queryVal+"'");

    /*************************************************************************
     * OUTPUT - Error message if query fails
     ************************************************************************/
    if(!queryName.exec()||!queryPrice.exec())
        qDebug() << "Failed: " << queryName.lastError() << queryPrice.lastError();

    /*************************************************************************
     * PROCESSING - Reload datatable to show updated information
     ************************************************************************/
    sqlModel->setQuery("SELECT Cities.CityID, Cities.Name, Foods.FoodName, Foods.Price FROM Foods, Cities WHERE Cities.CityID = Foods.CityID");
    ui->databaseView->setModel(sqlModel);
}

/*************************************************************************
 * void Administrator::on_UpdateFoodPushButton_clicked()
 * ----------------------------------------------------------------------
 * When clicking the update food button, the program will query the database
 * to update the price of the item whose name was entered to the new
 * price that was entered.
 ************************************************************************/
void Administrator::on_UpdateFoodPushButton_clicked()
{
    QString newPrice;                       //IN   & PROC - The new price of the food item that will be changed
    QString foodName;                       //IN   & PROC - The name of the food item that will be changed

    /*******************
     * INITIALIZATIONS *
     *******************/
    newPrice = ui -> PriceLineEdit    -> text();
    foodName = ui -> FoodNameLineEdit -> text();

    /*************************************************************************
     * PROCESSING - Query the database to set the new price of the food item
     ************************************************************************/
    QSqlQuery query;
    query.prepare("UPDATE Foods SET Price = (:newPrice)"
                  "WHERE FoodName = (:foodName)");

    query.bindValue(":newPrice", newPrice);
    query.bindValue(":foodName", foodName);

    /*************************************************************************
     * OUTPUT - Error message if query fails
     ************************************************************************/
    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();

    /*************************************************************************
     * PROCESSING - Reload datatable to show updated information
     ************************************************************************/
    sqlModel->setQuery("SELECT Cities.CityID, Cities.Name, Foods.FoodName, Foods.Price FROM Foods, Cities WHERE Cities.CityID = Foods.CityID");
    ui->databaseView->setModel(sqlModel);

    //Clear the line edits
    ui -> FoodNameLineEdit -> clear();
    ui -> PriceLineEdit    -> clear();
}

// this resets the tableview to show all the columns & resize them
void Administrator::resetDatabaseView(QAbstractItemModel* model)
{
    ui->databaseView->reset();
    ui->databaseView->horizontalHeader()->reset();
    ui->databaseView->setModel(model);
    for (int i = 0; i < model->columnCount(); i++)
    {
        ui->databaseView->showColumn(i);
    }
    ui->databaseView->resizeColumnsToContents();
}

// This creates a new query model by deleting the sqlModel and replacing it
// with a relationalTableModel so that we can do the the column names and
// foreign key relations
QSqlRelationalTableModel *Administrator::rebuildQuery()
{
    delete sqlModel;
    QSqlRelationalTableModel *sqlTableModel = new QSqlRelationalTableModel(this);
    this->sqlModel = sqlTableModel;
    return sqlTableModel;
}

/*************************************************************************
 * void Administrator::on_AddFromFilePushButton_clicked()
 * ----------------------------------------------------------------------
 * When clicking the button, the program will read in additional data added
 * to an input file. It will add the informstion to the database.
 ************************************************************************/
void Administrator::on_AddFromFilePushButton_clicked()
{
    SQLDatabase fileRead; //PROC & PROC - Used to read from files into database
    QSqlQuery   query;

    /*******************************************************************
     * PROCESSING - Read the files passed in into the database
     ******************************************************************/


    fileRead.readFileCities   (":/Database/NewCities.txt");
    fileRead.readFileDistances(":/Database/NewDistances.json");
    fileRead.readFileFoods    (":/Database/NewFoods.json");


    QSqlRelationalTableModel *sqlTableModel = rebuildQuery();
    sqlTableModel->setTable("Cities");
    sqlTableModel->select();	// this runs the select query

    resetDatabaseView(sqlTableModel);
    ui->databaseView->hideColumn(0);
}
