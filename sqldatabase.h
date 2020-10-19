
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
/*!
 * \brief SQLDatabase Constructor
 */
SQLDatabase();

/*!
 * \brief SQLDatabase Destructor
 */
~SQLDatabase();

/*!
 * \brief createDatabase
 * Creates the Database.  Checks to see if the database has already
 * been created, and if so doesn't re-create it. If not, it creates the
 * databases and calls the readfile function.
 */
void createDatabase();

/*!
 * \brief readFileCities
 * Reads the Cities file and inserts them into the CitiesTable
 */
void readFileCities(const QString &filename);

/*!
 * \brief readFileDistances
 * Reads the Distance files and inserts them into the Distance table
 */
void readFileDistances(const QString &filename);

/*!
 * \brief readFileFoods
 * Reads from the foods file and adds to the database
 */
void readFileFoods(const QString &filename);


/*!
 * \brief Returns the Database
 */
QSqlDatabase GetDatabase() const;

/*!
 * \brief editFoods
 * Edits selected food price.
 */
void editFoods(int foodID, double newPrice);

/*!
 * \brief Returns the CityID #
 */
static int GetCityIdByName(QString cityName);

/*!
 * \brief Returns the CityID #
 */
static QString GetCityNameById(int id);

/*!
 * \brief Returns the distance between two cities
 */
static int GetDistance(int fromID, int toID);

/*******************************************************************************
 * QList<cityDistance> SQLDatabase::GetDistancesFromCity(int cityID)
 * -----------------------------------------------------------------------------
 * THis function will query the SQL database to pull distancs from a city as
 * well as the distance to a city. It will use an integer city ID passed into
 * the function.
 ******************************************************************************/
static QList<cityDistance> GetDistancesFromCity(int cityID);

/*!
 * \brief Return a list of foods for city
 */
static QList<food> GetFoodsForCity(int cityID);

/*!
 * \brief Returns the size of the database
 */
static int GetSize();

private:
QSqlDatabase database;
};
#endif
