#pragma once
// Author: Misbahshk22 <2025sse73@uet.edu.pk>
// ================================================================
//  person2.h  —  PERSON  2
//  Module 3: Rider Dispatch & Assignment  (Dynamic Array)
//  Module 4: Routing & Infrastructure     (Adjacency List + Dijkstra)
// ================================================================

#include "common.h"

// Forward declare HistoryTracker so RiderManager can use it
class HistoryTracker;

// ================================================================
//  MODULE 4: ROUTING & INFRASTRUCTURE  (Adjacency List + Dijkstra)
// ================================================================
class EdgeNode {
public:
    int destination;
    int distance;
    bool isBlocked;
    EdgeNode* next;
    EdgeNode(int dest, int dist) : destination(dest), distance(dist), isBlocked(false), next(nullptr) {}
};

class LocationNode {
public:
    std::string name;
    EdgeNode* head;
    LocationNode() : head(nullptr) {}
};

class CityMap {
private:
    LocationNode* locations;
    int capacity;
    int count;
    void resize();
public:
    CityMap();
    ~CityMap();
    int getCount();
    std::string getName(int i);
    int getIndex(std::string name);
    void addLocation(std::string name);
    void addRoute(std::string src, std::string dest, int distance);
    void blockRoad(std::string src, std::string dest);
    void unblockRoad(std::string src, std::string dest);
    int getShortestDistance(int src, int dest);
    void optimizeRoute(std::string start, std::string end);
    void compareRoutes(std::string a1, std::string a2, std::string dest);
    void displayMap();
};

// ================================================================
//  MODULE 3: RIDER DISPATCH & ASSIGNMENT  (Dynamic Array)
// ================================================================
class RiderManager {
private:
    Rider* riders;
    int capacity;
    int count;
    void resize();
public:
    RiderManager();
    ~RiderManager();
    void addRider(std::string name, int zone, int cap = 2);
    void assignRider(int orderId, int restaurantZone, HistoryTracker& history, CityMap& city);
    void releaseRider(std::string name);
    void displayRiders();
    int getCount();
};
