#include "pathutils.h"

vector<cityNode> createCitiesFromDatabase()
{
    vector<cityNode> cityNodeList;
    QSqlQuery query;
    query.exec("SELECT Name FROM Cities");
    while (query.next())
    {
        // for the value in column 0 of the current row, turn it to a string
        // then create a QString value because it's QtSql
        QString city = query.value(0).toString();
        // converts Qstring to a standard std::string then builds a city node
        // w/ that & puts it in the vector
        cityNodeList.push_back({city.toStdString()});
    }

    return cityNodeList;

}

vector<cityConnection> createConnectionsFromDatabase()
{

    vector<cityConnection> ConnectionNodeList;
    QSqlQuery query;
    query.exec("SELECT fromCity, toCity, Distance FROM Distances " );
    while (query.next())
    {
        // for the value in column 0 of the current row, turn it to an int
        int fromCityID = query.value(0).toInt();
        int toCityID = query.value(1).toInt();
        int Distance = query.value(2).toInt();

        QSqlQuery query;
        query.prepare("SELECT Name FROM Cities "
                  "WHERE CityID = (:ID)");
        query.bindValue(":ID", fromCityID);
        if(!query.exec())
            qDebug() << "Failed: " << query.lastError();
        query.next();	// Gets the first row
        string fromCity = query.value(0).toString().toStdString();

        query.prepare("SELECT Name FROM Cities "
                  "WHERE CityID = (:ID)");
        query.bindValue(":ID", toCityID);
        if(!query.exec())
            qDebug() << "Failed: " << query.lastError();
        query.next();
        string toCity = query.value(0).toString().toStdString();

        // Creating the CityConnection and adding it to the vector
        ConnectionNodeList.push_back({{fromCity},	// Creating cityNode fromCity
                                      {toCity},		// crating cityNode toCity
                                      Distance });	// creating distance
    }

    return ConnectionNodeList;

}

cityGraph createCityGraphFromDatabase()
{
    vector<cityConnection> edgeList = createConnectionsFromDatabase();
    map<cityNode, vector<cityConnection>> adjacencyList;

    // Adjacency list from the edge list
    for(auto edge : edgeList)
    {
        string fromCity = edge.fromCity.name;
        string toCity = edge.toCity.name;
        int distance = edge.distance;

        // Creates adjacencyList if it doesn't already exist
        adjacencyList[{fromCity}].push_back(edge);
        // creating the reverse edge
        adjacencyList[{toCity}].push_back({
                              {toCity},		// crating cityNode toCity
                              {fromCity},	// Creating cityNode fromCity
                              distance });
    }
    return {createCitiesFromDatabase(), adjacencyList};
}
