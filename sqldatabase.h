
#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include "Header.h"

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
void readFileCities();

/*!
 * \brief readFileDistances
 * Reads the Distance files and inserts them into the Distance table
 */
void readFileDistances();

/*!
 * \brief readFileFoods
 * Reads from the foods file and adds to the database
 */
void readFileFoods();

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
 * \brief addCity
 * Adds a city to the database
 */
//void addCity(City& newCity);

/*!
 * \brief addDistance
 * Adds a distance to the database
 */
//void addDistance(Distance &newDistance);

/*!
 * \brief addFood
 * Adds a food to the database
 */
//void addFood(Food &newFood);

private:
QSqlDatabase database;
};
#endif
