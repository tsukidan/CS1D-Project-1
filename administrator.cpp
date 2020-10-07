#include "administrator.h"
#include "ui_administrator.h"
#include "sqldatabase.h"
#include "Header.h"
#include "login.h"

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
    ui -> AddFoodPushButton    -> setStyleSheet(QString("#%1 { background-color: cyan; }").arg(ui -> AddFoodPushButton->objectName()));

    ui -> UpdateFoodPushButton -> setStyleSheet(QString("#%1 { background-color: green; }").arg(ui -> UpdateFoodPushButton->objectName()));

    ui -> DeleteFoodPushButton -> setStyleSheet(QString("#%1 { background-color: pink; }").arg(ui -> DeleteFoodPushButton->objectName()));



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

void Administrator::on_food_Button_clicked()
{
    QSqlRelationalTableModel *sqlTableModel = rebuildQuery();
    sqlTableModel->setTable("Foods");
    sqlTableModel->setRelation(2, QSqlRelation("Cities", "CityID", "Name"));
    sqlTableModel->select();	// this runs the select query
    sqlTableModel->setHeaderData(2, Qt::Horizontal, tr("City"));

    resetDatabaseView(sqlTableModel);
    ui->databaseView->hideColumn(0);
    ui->databaseView->horizontalHeader()->moveSection(2, 1);
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

void Administrator::on_returnFromAdminUI_clicked()
{
    Login *loginUi;
    loginUi = new Login(this);
    hide();
    loginUi->show();
}


void Administrator::on_delete_City_clicked()
{
    QSqlQuery q;
    qDebug() << "ID: " << queryVal;
    q.prepare("DELETE FROM Cities WHERE Name='"+queryVal+"'");
    if(!q.exec())
        qDebug() << "Failed: " << q.lastError();

    sqlModel->setQuery("SELECT Name FROM Cities");
    ui->databaseView->setModel(sqlModel);
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
    QString foodName = ui -> FoodNameLineEdit -> text(); //IN & PROC - Name of food item that will be added
    QString price    = ui -> PriceLineEdit    -> text(); //IN & PROC - Price of food item will be added
    QSqlQuery query;

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


    qDebug() << "cityID:   " << queryVal;
    qDebug() << ":name     " << foodName;
    qDebug() << ":price    " << price;
    qDebug() << "";

    /*************************************************************************
     * PROCESSING - Reload datatable to show updated information
     ************************************************************************/
    sqlModel->setQuery("SELECT FoodName, Price, CityID FROM Foods");
    ui->databaseView->setModel(sqlModel);

    //Clear the line edits
    ui -> FoodNameLineEdit -> clear();
    ui -> PriceLineEdit    -> clear();
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
    sqlModel->setQuery("SELECT FoodName, Price, CityID FROM Foods");

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
    QString newPrice = ui -> PriceLineEdit    -> text(); //IN & PROC - The new price of the food item that will be changed
    QString foodName = ui -> FoodNameLineEdit -> text(); //IN & PROC - The name of the food item that will be changed

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
    sqlModel->setQuery("SELECT FoodName, Price, CityID FROM Foods");
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
