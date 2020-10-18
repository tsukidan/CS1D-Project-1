#include "sqldatabase.h"

/*!
 * \brief SQLDatabase Constructor
 */
// creates the database - wrapper around QSQLDatabase which manages the file
SQLDatabase::SQLDatabase()
{
    // creating a SQLite db
    database = QSqlDatabase::addDatabase("QSQLITE");
    // creating the db file
    QFileInfo db("database.sqlite");
    // getting the path as a string
    QString DB_PATH = db.absoluteFilePath();
    // printing debug output onto console
    qDebug() << DB_PATH;
    // tell sqlite to use the file
    database.setDatabaseName(DB_PATH);
    // open the db
    database.open();
}

/*!
 * \brief SQLDatabase Destructor
 */
SQLDatabase::~SQLDatabase()
{

}

/*!
 * \brief createDatabase
 * Creates the Database.  Checks to see if the database has already
 * been created, and if so doesn't re-create it. If not, it creates the
 * databases and calls the readfile function.
 */
void SQLDatabase::createDatabase()
{
    // to execute sql queries
    QSqlQuery query;

    // TODO: rewrite queries to check if the tables exists
    // if they don't then create them & call the readfile function

    bool initCities = false;
    bool initFoods  = false;
    bool initDistances = false;

    QStringList tables = database.tables();
    if (!tables.contains("Cities", Qt::CaseInsensitive))
    {
        initCities = true;
        // creates the 3 tables
        if(!query.exec("CREATE TABLE    IF NOT EXISTS "
                       "Cities("
                       "CityID          INTEGER NOT NULL PRIMARY KEY,"
                       "Name            VARCHAR(50)"
                       ");"))
            qDebug() << "Failed: " << query.lastError();

    }

    if (!tables.contains("Distances", Qt::CaseInsensitive))
    {
        initDistances = true;
        if(!query.exec("CREATE TABLE            IF NOT EXISTS "
                       "Distances("
                       "FromCity                INTEGER DEFAULT 0 NOT NULL,"
                       "ToCity                  INTEGER DEFAULT 0 NOT NULL,"
                       "Distance                INTEGER DEFAULT 0 NOT NULL,"
                       "foreign key(FromCity)   references City(CityID),"
                       "foreign key(ToCity)     references City(CityID)"
                       ");"))
            qDebug() << "Failed: " << query.lastError();
    }

    if (!tables.contains("Foods", Qt::CaseInsensitive))
    {
        initFoods = true;
        if(!query.exec("CREATE TABLE          IF NOT EXISTS "
                       "Foods("
                       "FoodID                INTEGER NOT NULL PRIMARY KEY,"
                       "FoodName              VARCHAR(50),"
                       "CityID                INTEGER DEFAULT 0 NOT NULL,"
                       "Price                 REAL DEFAULT 0 NOT NULL,"
                       "foreign key(CityID)   references City(CityID)"
                       ");"))
            qDebug() << "Failed: " << query.lastError();
    }
    if (initCities)
        readFileCities(":/Database/Cities.txt");
    if (initDistances)
        readFileDistances(":/Database/Distances.json");
    if (initFoods)
        readFileFoods(":/Database/Foods.json");
}

/*!
 * \brief readFileCities
 * Reads the Cities file and inserts them into the CitiesTable
 */
void SQLDatabase::readFileCities(const QString& filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);

    if(file.isOpen())
    {
        qDebug() << "Opened File";
        while(!inFile.atEnd())
        {
            QString newCityname;
            newCityname = inFile.readLine();

            QSqlQuery query;
            query.prepare("INSERT OR IGNORE INTO Cities(name) "
                          "SELECT (:name) "
                          "WHERE NOT EXISTS "
                          "(Select 1 FROM Cities WHERE Name = (:name))");
            query.bindValue(":name", newCityname);

            if(!query.exec())
                qDebug() << "Failed: " << query.lastError() << query.executedQuery();
            file.close();
        }
    }
    else
        qDebug() << "Cannot open file thats used to Read File from Cities list";
}

/*!
 * \brief readFileDistances
 * Reads the Distance files and inserts them into the Distance table
 */
void SQLDatabase::readFileDistances(const QString& filename)
{
    QFile file(filename);
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

            query.prepare("DELETE FROM Distances WHERE "
                          "FromCity = :FromCity AND ToCity = :ToCity");
            query.bindValue(":FromCity", fromID);
            query.bindValue(":ToCity", toID);


            if(!query.exec())
                qDebug() << "Failed: " << query.lastError();

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

/*!
 * \brief readFileFoods
 * Reads from the foods file and adds to the database
 */
void SQLDatabase::readFileFoods(const QString &filename)
{
    qDebug() << "file name for readFileFoods: " << filename;
    QFile file(filename);
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


                query.prepare("DELETE FROM Foods WHERE FoodName = :name");
                query.bindValue(":name", foodName);

                if(!query.exec())
                    qDebug() << "Failed: " << query.lastError();

                query.prepare("INSERT OR IGNORE INTO Foods(FoodName,CityID, Price)"
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
 * \brief editFoods
 * Edits selected food price.
 */
void SQLDatabase::editFoods(int foodID, double newPrice)
{
    QSqlQuery query;
    query.prepare("UPDATE Foods SET Price = (:newPrice)"
                  "WHERE foodID = (:foodID)");
    query.bindValue(":newPrice", newPrice);
    query.bindValue(":foodID", foodID);
    if(!query.exec())
        qDebug() << "Query " << query.executedQuery() << " Failed: " << query.lastError() << " (" << foodID << ")";
}

/*!
 * \brief Returns the CityID #
 */
int SQLDatabase::GetCityIdByName(QString cityName)
{
    QSqlQuery query;
    query.prepare("SELECT CityID FROM Cities "
                  "WHERE Name = (:name)");

    query.bindValue(":name", cityName);

    if(!query.exec())
        qDebug() << "GetCityIdByName Failed: " << query.lastError() << " " << cityName;
// fetch row
    query.next();
    return query.value(0).toInt();
}

/*!
 * \brief Returns the CityID #
 */
QString SQLDatabase::GetCityNameById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT Name FROM Cities "
                  "WHERE cityID = (:id)");

    query.bindValue(":id", id);

    if(!query.exec())
        qDebug() << "GetCityNameById Failed: " << query.lastError() << " " << id;
// fetch row
    query.next();
    return query.value(0).toString();
}

/*!
 * \brief Returns the distance between two cities
 */
int SQLDatabase::GetDistance(int fromID, int toID)
{
    QSqlQuery query;
    query.prepare("SELECT distance FROM Distances "
                  "WHERE fromCity = (:fromID) "
                  "AND toCity = (:toID)");

    query.bindValue(":fromID", fromID);
    query.bindValue(":toID", toID);

    if(!query.exec())
        qDebug() << "GetDistance Failed: "<< fromID << " to " << toID << query.lastError() << " " << query.executedQuery() ;
// fetch row
    query.next();
    return query.value(0).toInt();
}


/*!
 * \brief Returns the Database
 */
QSqlDatabase SQLDatabase::GetDatabase() const
{
    return database;
}

/*!
 * \brief addCity
 * Adds a city to the database
 */
/*
void SQLDatabase::addCity(City& newCity)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO Cities(name) "
                  "VALUES(:name)");

    query.bindValue(":name", newCity.name);

    if(!query.exec())
        qDebug() << "Failed: " << query.lastError();
}
*/

/*!
 * \brief addDistance
 * Adds a distance to the database
 */
/*
void SQLDatabase::addDistance(Distance &newDistance)
{

}
*/

/*!
 * \brief addFood
 * Adds a food to the database
 */
/*
void SQLDatabase::addFood(Food &newFood)
{

}
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

