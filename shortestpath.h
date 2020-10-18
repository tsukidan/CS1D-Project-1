#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "Header.h"
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

// Node of a city
struct cityNode
{
    string name;
};

bool operator< (const cityNode& lhs, const cityNode& rhs);

// Connection between two cities
struct cityConnection
{
    cityNode fromCity;
    cityNode toCity;
    int 	 distance;
};

// Graph of cities and their connections
struct cityGraph
{
    vector<cityNode> cities;
    map<cityNode, vector<cityConnection>> adjacencyList;
};

vector<string> ShortestPath(const cityGraph& graph, cityNode start, cityNode endLocation);

void printShortestPath(vector<string> path);

#endif // SHORTESTPATH_H
