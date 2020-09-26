#include "sqldatabase.h"

#include <QFileInfo>
#include <QDebug>
#include <QSqlQuery>
#include<QSqlError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*!
 * \brief Constructor for sqlDatabase
 * \param parent
 */

SQLDatabase::SQLDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QFileInfo db("database.sqlite");
    QString DB_PATH = db.absoluteFilePath();
    qDebug() << DB_PATH;
    database.setDatabaseName(DB_PATH);
    database.open();
}

/*!
 * \brief Destructor for sqlDatabase
 */
SQLDatabase::~SQLDatabase()
{

}
/*!
 * \brief Creates the Database
 */
void SQLDatabase::createDatabase()
{
    QSqlQuery query;

    if(!query.exec("CREATE TABLE    Cities("
                   "CityID          INTEGER NOT NULL PRIMARY KEY,"
                   "Name            VARCHAR(50)"
                   ");"))
        qDebug() << "Failed: " << query.lastError();

   if(!query.exec("CREATE TABLE            Distances("
                  "FromCity                INTEGER DEFAULT 0 NOT NULL,"
                  "ToCity                  INTEGER DEFAULT 0 NOT NULL,"
                  "Distance                INTEGER DEFAULT 0 NOT NULL,"
                  "foreign key(FromCity)   references City(CityID),"
                  "foreign key(ToCity)     references City(CityID)"
                  ");"))
       qDebug() << "Failed: " << query.lastError();

    if(!query.exec("CREATE TABLE        Foods("
                   "FoodID              INTEGER NOT NULL PRIMARY KEY,"
                   "FoodName            VARCHAR(50),"
                   "CityID              INTEGER DEFAULT 0 NOT NULL,"
                   "Price               REAL DEFAULT 0 NOT NULL,"
                   "foreign key(CityID)   references City(CityID)"
                   ");"))
    qDebug() << "Failed: " << query.lastError();
 }

/*!
 * \brief Reads the Shopper.txt file and inserts them into the CustomerTable
 */
void SQLDatabase::readFileCities()
{
    QFile file(":/Database/Cities.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);

    if(file.isOpen())
    {
        qDebug() << "Opened File";
        while(!inFile.atEnd())
        {
            QString newCityname;
            newCityname = inFile.readLine();
//            addCity(newCity);
            QSqlQuery query;
            query.prepare("INSERT OR IGNORE INTO Cities(name) "
                          "VALUES(:name)");

            query.bindValue(":name", newCityname);

            if(!query.exec())
                qDebug() << "Failed: " << query.lastError();
        }

        file.close();
    }

    else
        qDebug() << "Cannot open file thats used to Read File from Cities list";
}

/*!
 * \brief Reads the days.txt files and inserts them into the Sales table
 */
void SQLDatabase::readFileDistances()
{
    QFile file(":/Database/Distances.json");
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);
    if(file.isOpen())
    {
        qDebug() << "Opened File";
        QJsonDocument doc = QJsonDocument::fromJson(inFile.readAll().toUtf8());
        QJsonArray distances = doc.array();
//        qDebug() << doc;

        for(auto distanceValue: distances)
        {
            QJsonObject distanceObj = distanceValue.toObject();
            int distance = distanceObj["Distance"].toInt();
            QString fromCity = distanceObj["FromCity"].toString();
            QString toCity = distanceObj["ToCity"].toString();

            qDebug() << "From: " << fromCity << " to " << toCity << " is " << distance;

            qDebug() << "Getting fromCity";
            QSqlQuery query;
            query.prepare("SELECT CityID FROM Cities "
                          "WHERE Name = (:name)");
            query.bindValue(":name", fromCity);
            if(!query.exec())
                qDebug() << "Failed: " << query.lastError();
            query.next();
            int fromID = query.value(0).toInt();

            qDebug() << "Getting toCity";
            query.prepare("SELECT CityID FROM Cities "
                          "WHERE Name = (:name)");
            query.bindValue(":name", toCity);
            if(!query.exec())
                qDebug() << "Failed: " << query.lastError();
            query.next();
            int toID = query.value(0).toInt();

            qDebug() << "Inserting distance";
            query.prepare("INSERT OR IGNORE INTO Distances(FromCity, ToCity, Distance) "
                          "VALUES(:FromCity, :ToCity, :Distance)");

            query.bindValue(":FromCity", fromID);
            query.bindValue(":ToCity", toID);
            query.bindValue(":Distance", distance);

            if(!query.exec())
                qDebug() << "Failed: " << query.lastError();
        }

        file.close();
    }

    else
        qDebug() << "Cannot open file thats used to Read File from Distances list";

}

void SQLDatabase::readFileFoods()
{
    QFile file(":/Database/Foods.json");
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);
    if(file.isOpen())
    {
        qDebug() << "Opened File";
        QJsonDocument doc = QJsonDocument::fromJson(inFile.readAll().toUtf8());
        QJsonArray foods= doc.array();
//        qDebug() << doc;

        for(auto foodValue: foods)
        {
            QJsonObject foodObj = foodValue.toObject();
            QString cityName = foodObj["City"].toString();

            qDebug() << "Getting fromCity: " << cityName;
            QSqlQuery query;
            query.prepare("SELECT CityID FROM Cities "
                          "WHERE Name = (:name)");
            query.bindValue(":name", cityName);
            if(!query.exec())
                qDebug() << "Failed: " << query.lastError() << " (" << cityName << ")";
            query.next();
            int cityID = query.value(0).toInt();

            QJsonArray pricesArray = foodObj["Foods"].toArray();

            for (auto priceValue: pricesArray)
            {
                QJsonObject priceObj = priceValue.toObject();
                QString foodName = priceObj["Food"].toString();
                double foodPrice = priceObj["Price"].toDouble();
                qDebug() << "Inserting price";

                query.prepare("INSERT OR IGNORE INTO Foods(FoodName, CityID, Price) "
                              "VALUES(:name, :city, :price)");

                query.bindValue(":name", foodName);
                query.bindValue(":city", cityID);
                query.bindValue(":price", foodPrice);

                if(!query.exec())
                    qDebug() << "Failed: " << query.lastError();
            }

        }

        file.close();
    }

    else
        qDebug() << "Cannot open file thats used to Read File from Distances list";


}
/*!
 * \brief Returns the Database
 */
QSqlDatabase SQLDatabase::GetDatabase() const
{
    return database;
}

/*!
 * \brief Inserts the customers into the Customer Table
 */

/*
void SQLDatabase::addCity(City& newCity)
{

}

void SQLDatabase::addDistance(Distance &newDistance)
{

}

void SQLDatabase::addFood(Food &newFood)
{

}
*/

/*!
 * \brief Inserts the sales info into the SalesTable
 */

/*
void SQLDatabase::addSalesIntoTable(salesTableInfo& salesData)
{

    QSqlQuery query;

    query.prepare("INSERT INTO SalesTable(PurchaseDate, CustomID, ItemName, ItemPrice, Quantity)"
                  "VALUES(:date, :ID, :itemName, :itemPrice, :quantity)");

    query.bindValue(":date", salesData.purchaseDate);
    query.bindValue(":ID", salesData.customerID);
    query.bindValue(":itemName", salesData.itemName);
    query.bindValue(":itemPrice", salesData.itemPrice);
    query.bindValue(":quantity", salesData.quantity);

    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();

    checkInventory();
}

/*!
 * \brief Inserts the Inventory into the InvenotryTable
 *
void SQLDatabase::handleInventory()
{
    QSqlQuery query;

    query.prepare("INSERT INTO InventoryTable(ItemName, ItemPrice, Quantity, InStock,Revenue)"
                  "VALUES(:name, :price, :quant, :stock,:rev)");
    query.bindValue(":name", inventoryData.itemName);
    query.bindValue(":price", inventoryData.itemPrice);
    query.bindValue(":quant", inventoryData.quantityPurchased);
    query.bindValue(":stock", inventoryData.inStock);
    query.bindValue(":rev", inventoryData.revenue);

    if(!query.exec()){
        qDebug() << "Failed: " << query.lastError();
    }
}
/*!
 * \brief Checks the InventoryTable and adds the Quantity if there is an existing item
 *
void SQLDatabase::checkInventory(){

    QSqlQuery query;
    query.prepare("SELECT * FROM InventoryTable"
                  " WHERE ItemName       LIKE '%" + salesData.itemName + "%'");

    query.bindValue(":searchingFor", salesData.itemName);

    if(!query.exec()) {
        qDebug() << query.lastError();
    }

    if (query.next()) {

        double itemPrice = query.value(1).toDouble();
        int quantFromDB = query.value(2).toInt();
        double totalRevenue = query.value(4).toDouble();
        int quantToInput = salesData.quantity.toInt();
        int newQuantForDb =  quantFromDB + quantToInput;
        int newStockForDb = 500 - newQuantForDb;
        totalRevenue = newQuantForDb * itemPrice;
        double dec = query.value(1).toString().toDouble();

       if(newStockForDb <= 0 || newStockForDb < 0){
            updateDB(0,quantFromDB,dec,query.value(4).toDouble());
       }
       else{
            updateDB(newStockForDb,newQuantForDb,dec,totalRevenue);
           }
       }
    else{
        inventoryData.itemName = salesData.itemName;
        inventoryData.itemPrice = salesData.itemPrice;
        inventoryData.quantityPurchased = salesData.quantity;
        int newStock = 500 - salesData.quantity.toInt();
        QString m;
        inventoryData.inStock = m.number(newStock);
        double totalRev = salesData.quantity.toInt() * inventoryData.itemPrice.toDouble();
        inventoryData.revenue = totalRev;
        handleInventory();
        }
}
/*!
 * \brief Updates the Inventory Table with the new or current Items
 *
void SQLDatabase::updateDB(int stock,int quant,double dec,double totalRevenue){
    QSqlQuery query;
    query.prepare("UPDATE InventoryTable "
                  "SET    ItemName = :name, "
                  "       ItemPrice = :price, "
                  "       Quantity = :quant, "
                  "       InStock = :stock, "
                  "       Revenue = :rev "
                  "WHERE  ItemName = :c;");

       QString price = price.number(dec,'f',2);
       QString rev = rev.number(totalRevenue,'f',2);
       qDebug() << price << " " << rev;

       query.bindValue(":name", salesData.itemName);
       query.bindValue(":price", price);
       query.bindValue(":quant", quant);
       query.bindValue(":stock", stock);
       query.bindValue(":rev", rev);
       query.bindValue(":c",salesData.itemName);

       if(!query.exec())
            qDebug() << query.lastError();
}
*/

