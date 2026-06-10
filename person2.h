#pragma once
// ================================================================
//  person2.h  —  PERSON 2
//  Module 3: Rider Dispatch & Assignment  (Dynamic Array)
//  Module 4: Routing & Infrastructure     (Adjacency List + Dijkstra)
// ================================================================

#include "common.h"

// Forward declare HistoryTracker so RiderManager can use it
class HistoryTracker;

// ================================================================
//  MODULE 4: ROUTING & INFRASTRUCTURE  (Adjacency List + Dijkstra)
//  Defined before RiderManager because RiderManager depends on it
// ================================================================
class EdgeNode {
public:
    int       destination;
    int       distance;
    bool      isBlocked;
    EdgeNode* next;
    EdgeNode(int dest, int dist)
        : destination(dest), distance(dist), isBlocked(false), next(nullptr) {}
};

class LocationNode {
public:
    string    name;
    EdgeNode* head;
    LocationNode() : head(nullptr) {}
};

class CityMap {
private:
    LocationNode* locations;
    int           capacity;
    int           count;

    void resize();

public:
    CityMap();
    ~CityMap();

    int    getCount();
    string getName(int i);
    int    getIndex(string name);
    void   addLocation(string name);
    void   addRoute(string src, string dest, int distance);
    void   blockRoad(string src, string dest);
    void   unblockRoad(string src, string dest);
    int    getShortestDistance(int src, int dest);   // Dijkstra O(V^2)
    void   optimizeRoute(string start, string end);
    void   compareRoutes(string a1, string a2, string dest);
    void   displayMap();
};

// ================================================================
//  MODULE 3: RIDER DISPATCH & ASSIGNMENT  (Dynamic Array)
// ================================================================
class RiderManager {
private:
    Rider* riders;
    int    capacity, count;

    void resize();

public:
    RiderManager();
    ~RiderManager();

    void addRider(string name, int zone, int cap = 2);
    void assignRider(int orderId, int restaurantZone,
                     HistoryTracker& history, CityMap& city);
    void releaseRider(string name);
    void displayRiders();
    int  getCount();
};
