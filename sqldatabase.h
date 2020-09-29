
#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QSqlDatabase>

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


//void addCity(City& newCity);

//void addDistance(Distance &newDistance);


//void addFood(Food &newFood);

private:
QSqlDatabase database;
};
#endif
