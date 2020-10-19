
#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include "Header.h"

struct food
{
    QString cityName;
    QString foodName;
    float   price;
};

struct cityDistance
{
    QString cityName;
    int     distance;
};

class SQLDatabase
{
public:
/// SQLDatabase Constructor
SQLDatabase();

/// ~SQLDatabase Destructor
~SQLDatabase();

/// createDatabase
///
/// Creates the Database.  Checks to see if the database has already
/// been created, and if so doesn't re-create it. If not, it creates the
/// databases and calls the readfile function.
/// @return void
void createDatabase();

/// readFileCities
///
/// Reads the Cities file and inserts them into the CitiesTable.
/// @param const QString &
/// @return void
void readFileCities(const QString &filename);

///readFileDistances
///
/// Reads the Distance files and inserts them into the Distance table.
/// @param cosnt QString &
/// @return void
void readFileDistances(const QString &filename);


/// readFileFoods
///
/// Reads from the foods file and adds to the database.
/// @param const QString &
/// @return void
void readFileFoods(const QString &filename);


/// GetDatabase
///
/// Returns the Database.
/// @return QSQLDatabase
QSqlDatabase GetDatabase() const;

/// editFoods
///
/// Edits selected food price.
/// @param { int, double }
/// @return void
void editFoods(int foodID, double newPrice);

/// GetCityIdByName
///
/// This function reads the database to return a specific
/// city ID from the *cities db*.
/// @param QString
/// @return int
static int GetCityIdByName(QString cityName);

/// GetCityNameById
///
/// This function reads the database to return a specific
/// city name from the *cities db*.
/// @param int
/// @return QString
static QString GetCityNameById(int id);

/// GetDistance
///
/// Returns the distance between two cities
/// @param { int, int }
/// @return int
static int GetDistance(int fromID, int toID);

/// GetDistancesFromCity
///
/// Gets distance from a specific city using a city ID.
/// @param int
/// @return QList<cityDistance>
static QList<cityDistance> GetDistancesFromCity(int cityID);

/// GetFoodsForCity
///
/// Returns a list of foods for a specific city.
/// @param int
/// @return QList<food>
static QList<food> GetFoodsForCity(int cityID);

/// GetSize
///
/// Returns the size of the database.
/// @return int
static int GetSize();

private:
QSqlDatabase database;
};
#endif
