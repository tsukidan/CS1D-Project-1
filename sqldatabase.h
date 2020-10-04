
#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include "Header.h"

class SQLDatabase
{
public:
    SQLDatabase();

~SQLDatabase();

void createDatabase();

void readFileCities();

void readFileDistances();

void readFileFoods();

QSqlDatabase GetDatabase() const;

void editFoods(int foodID, double newPrice);

//void addCity(City& newCity);

//void addDistance(Distance &newDistance);


//void addFood(Food &newFood);

private:
QSqlDatabase database;
};
#endif
