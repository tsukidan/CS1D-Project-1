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
        readFileCities();
    if (initDistances)
        readFileDistances();
    if (initFoods)
        readFileFoods();
}

/*!
 * \brief readFileCities
 * Reads the Cities file and inserts them into the CitiesTable
 */
void SQLDatabase::readFileCities()
{
    QFile file(":/Database/Cities.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream inFile(&file);
    bool doesExist = false;
    int exists = 0;

    if(file.isOpen())
    {
        qDebug() << "Opened File";
        while(!inFile.atEnd())
        {
            // Get next city name
            QString newCityname;
            newCityname = inFile.readLine();

            // Check to see if city already exists
            QSqlQuery check;
            check.prepare("SELECT count(*) FROM Cities WHERE Name = (:name)");
            exists = check.value(0).toInt();
            qDebug() << "Check Cities number: " << exists;

            QSqlQuery query;
            query.prepare("INSERT OR IGNORE INTO Cities(name) "
                          "SELECT (:name) "
                          "WHERE NOT EXISTS "
                          "(Select 1 FROM Cities WHERE Name = (:name))");

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
 * \brief readFileDistances
 * Reads the Distance files and inserts them into the Distance table
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

/*!
 * \brief readFileFoods
 * Reads from the foods file and adds to the database
 */
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

QList<cityDistance> SQLDatabase::GetDistancesFromCity(int cityID)
{
    QList<cityDistance> list;
    QSqlQuery query;
    query.prepare("SELECT toCity, distance FROM Distances "
                  "WHERE fromCity = (:cityID) ");

    query.bindValue(":cityID", cityID);

    if(!query.exec())
        qDebug() << "GetDistancesFromCity Failed: "<< cityID << query.lastError() << " " << query.executedQuery() ;
    // fetch row
    while (query.next())
        list.append({GetCityNameById(query.value(0).toInt()),
                     query.value(1).toInt()});

    return list;
}

/*!
 * \brief Return a list of foods for city
 */
QList<food> SQLDatabase::GetFoodsForCity(int cityID)
{
    QList<food> list;
    QSqlQuery query;
    query.prepare("SELECT FoodName, Price FROM Foods "
                  "WHERE cityID = (:cityID) ");

    query.bindValue(":cityID", cityID);

    if(!query.exec())
        qDebug() << "GetFoodsForCity Failed: "<< cityID << query.lastError() << " " << query.executedQuery() ;
    // fetch row
    while (query.next())
        list.append({GetCityNameById(cityID),
                     query.value(0).toString(),
                     query.value(1).toFloat()});

    return list;
}

/*!
 * \brief Returns the size of the database
 */
int SQLDatabase::GetSize()
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM Cities ");

    if(!query.exec())
        qDebug() << "GetSize Failed: " << query.lastError() << " " << query.executedQuery() ;
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
