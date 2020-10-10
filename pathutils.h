#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QtSql>
#include <QtCore>
#include "Header.h"
#include "shortestpath.h"


vector<cityNode> createCitiesFromDatabase();

vector<cityConnection> createConnectionsFromDatabase();

cityGraph createCityGraphFromDatabase();

#endif // PATHUTILS_H
